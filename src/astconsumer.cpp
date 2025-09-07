// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "astconsumer.h"
#include "config.h"
#include "configmanager.h"
#include "diag.h"
#include "utility.h"
BEFORE_LLVM
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/Version.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/Support/GlobPattern.h>
AFTER_LLVM

struct OrthodoxyASTConsumer::Private
{
    clang::CompilerInstance *M_CI;
    clang::ASTContext *M_AST;
    clang::DiagnosticsEngine *M_DE;
    std::unique_ptr<OrthodoxyConfigManager> M_CM;
    bool M_WarnOnly;
    class PPHooks;
    PPHooks *M_PPhk;
    //
    llvm::StringMap<std::unique_ptr<llvm::GlobPattern>> M_GlobCache;
    //
    class ASTVisitor : public clang::RecursiveASTVisitor<ASTVisitor>
    {
    public:
        Private *M_priv;
        bool shouldVisitTemplateInstantiations() const { return true; }
        bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *UD);
        bool VisitImportDecl(const clang::ImportDecl *ID);
        bool VisitTagDecl(const clang::TagDecl *TD);
        // bool VisitRecordDecl(const clang::RecordDecl *RD);
        bool VisitAccessSpecDecl(const clang::AccessSpecDecl *AD);
        bool VisitFunctionDecl(const clang::FunctionDecl *FD);
        bool VisitParmVarDecl(const clang::ParmVarDecl *VD);
        bool VisitVarDecl(const clang::VarDecl *VD);
        bool VisitFieldDecl(const clang::FieldDecl *FD);
        bool VisitTemplateDecl(const clang::TemplateDecl *TD);
        bool VisitNamespaceDecl(const clang::NamespaceDecl *ND);
        bool VisitLambdaExpr(const clang::LambdaExpr *LE);
        bool VisitCStyleCastExpr(const clang::CStyleCastExpr *CE);
        bool VisitCXXRecordDecl(const clang::CXXRecordDecl *RD);
        bool VisitCXXMethodDecl(const clang::CXXMethodDecl *MD);
        bool VisitCXXThrowExpr(const clang::CXXThrowExpr *TE);
        bool VisitCXXTryStmt(const clang::CXXTryStmt *TS);
        bool VisitCXXNamedCastExpr(const clang::CXXNamedCastExpr *CE);
        bool VisitCXXThisExpr(const clang::CXXThisExpr *TE);
        bool VisitCXXNewExpr(const clang::CXXNewExpr *NE);
        bool VisitCXXDeleteExpr(const clang::CXXDeleteExpr *DE);
        bool VisitCXXForRangeStmt(const clang::CXXForRangeStmt *FS);
    };
    //
    class PPHooks : public clang::PPCallbacks
    {
    public:
        Private *M_priv;

        void InclusionDirective(
            clang::SourceLocation HashLoc,
            const clang::Token &IncludeTok,
            llvm::StringRef FileName,
            bool IsAngled,
            clang::CharSourceRange FilenameRange,
            clang::OptionalFileEntryRef File,
            llvm::StringRef SearchPath,
            llvm::StringRef RelativePath,
#if CLANG_VERSION_MAJOR >= 19
            const clang::Module *SuggestedModule,
            bool ModuleImported,
#else
            const clang::Module *Imported,
#endif
            clang::SrcMgr::CharacteristicKind FileType) override;
    };
    //
    bool IsIncludeAccepted(llvm::StringRef incPath, const OrthodoxyConfig &config);
    llvm::GlobPattern *GetCachedGlobOrNull(llvm::StringRef pattern);
    //
    std::optional<clang::DiagnosticBuilder> Report(clang::SourceLocation loc, const OrthodoxyDiagDesc &diag);
    bool IsSuppressedLocation(clang::SourceLocation loc, const OrthodoxyDiagDesc &diag);
    static bool LineHasSuppressionComment(llvm::StringRef data, unsigned offset, const OrthodoxyDiagDesc &diag);
};

OrthodoxyASTConsumer::OrthodoxyASTConsumer(clang::CompilerInstance &CI, bool warnOnly)
{
    Private *priv = new Private;
    M_priv.reset(priv);
    priv->M_CI = &CI;
    priv->M_AST = nullptr;
    priv->M_CM.reset(new OrthodoxyConfigManager(CI));
    priv->M_WarnOnly = warnOnly;

    Private::PPHooks *PPhk = new Private::PPHooks;
    PPhk->M_priv = priv;
    priv->M_PPhk = PPhk;
    CI.getPreprocessor().addPPCallbacks(
        std::unique_ptr<clang::PPCallbacks>(PPhk));
}

OrthodoxyASTConsumer::~OrthodoxyASTConsumer()
{
    Private *priv = M_priv.get();

    // can't just remove hook from preprocessor
    priv->M_PPhk->M_priv = nullptr;
}

void OrthodoxyASTConsumer::HandleTranslationUnit(clang::ASTContext &AST)
{
    Private *priv = M_priv.get();
    priv->M_AST = &AST;
    priv->M_DE = &AST.getDiagnostics();

    //
    Private::ASTVisitor visitor;
    visitor.M_priv = priv;
    visitor.TraverseDecl(AST.getTranslationUnitDecl());

    //
    priv->M_AST = nullptr;
    priv->M_DE = nullptr;
}

bool OrthodoxyASTConsumer::Private::IsIncludeAccepted(llvm::StringRef incPath, const OrthodoxyConfig &config)
{
    std::string incPathBuf;
    if (llvm::sys::path::is_style_windows(llvm::sys::path::Style::native))
        incPath = incPathBuf = llvm::sys::path::convert_to_slash(incPath);

    bool includeAccept = true;
    for (size_t i = 0; i < config.IncludeForbid.size(); ++i)
    {
        llvm::StringRef pat = config.IncludeForbid[i];
        llvm::GlobPattern *glob = GetCachedGlobOrNull(pat);
        if (!glob) continue;
        if (glob->match(incPath)) includeAccept = false;
    }
    for (size_t i = 0; i < config.IncludeAllow.size(); ++i)
    {
        llvm::StringRef pat = config.IncludeAllow[i];
        llvm::GlobPattern *glob = GetCachedGlobOrNull(pat);
        if (!glob) continue;
        if (glob->match(incPath)) includeAccept = true;
    }

    return includeAccept;
}

llvm::GlobPattern *OrthodoxyASTConsumer::Private::GetCachedGlobOrNull(llvm::StringRef pattern)
{
    llvm::GlobPattern *glob;

    auto it = M_GlobCache.find(pattern);
    if (it != M_GlobCache.end())
    {
        glob = it->second.get();
    }
    else
    {
        std::unique_ptr<llvm::GlobPattern> *slot = &M_GlobCache[pattern];
        llvm::Expected<llvm::GlobPattern> res = llvm::GlobPattern::create(pattern);
        if (!res)
        {
            llvm::errs() << "orthodoxy: invalid glob \"" << pattern << "\"\n";
            return nullptr;
        }
        glob = new llvm::GlobPattern(std::move(*res));
        slot->reset(glob);
    }
    return glob;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitTranslationUnitDecl(const clang::TranslationUnitDecl *UD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;

    if (UD->hasOwningModule())
    {
        clang::Module *mod = UD->getOwningModule();
        OrthodoxyConfig &config = CM.GetConfigForSourceLocation(mod->DefinitionLoc);
        if ((!config.Module || !config.ModuleExport))
            priv->Report(mod->DefinitionLoc, Orthodoxy::diag::ModuleExport());
    }

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitImportDecl(const clang::ImportDecl *ID)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(ID);

    if (!ID->isImplicit())
        // NOTE not sure about the necessity of this check
        // according to clang::ImportDecl documentation:
        // Import declarations can also be implicitly generated from #include/#import directives.
    {
        if ((!config.Module || !config.ModuleImport))
            priv->Report(ID->getLocation(), Orthodoxy::diag::ModuleImport());
    }

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitTagDecl(const clang::TagDecl *TD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(TD);

    if (!config.Class && Orthodoxy::TagIsClass(TD))
        priv->Report(TD->getLocation(), Orthodoxy::diag::Class());
    else if (!config.EnumClass && Orthodoxy::TagIsEnumClass(TD))
        priv->Report(TD->getLocation(), Orthodoxy::diag::EnumClass());

    return true;
}

// bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitRecordDecl(const clang::RecordDecl *RD)
// {
//     Private *priv = M_priv;
//     OrthodoxyConfigManager &CM = *priv->M_CM;
//     OrthodoxyConfig &config = CM.GetConfigForDecl(RD);
//
//     return true;
// }

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitAccessSpecDecl(const clang::AccessSpecDecl *AD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(AD);

    if (!config.Access || !config.MemberAccess)
        priv->Report(AD->getLocation(), Orthodoxy::diag::MemberAccess());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitFunctionDecl(const clang::FunctionDecl *FD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(FD);

    if ((!config.Overload || !config.OperatorOverload) && FD->isOverloadedOperator())
        priv->Report(FD->getLocation(), Orthodoxy::diag::OperatorOverload());
    else if ((!config.Overload || !config.ConversionOverload) && llvm::isa<clang::CXXConversionDecl>(FD))
        priv->Report(FD->getLocation(), Orthodoxy::diag::ConversionOverload());
    else if ((!config.Overload || !config.FunctionOverload) && !FD->getParent()->lookup(FD->getDeclName()).isSingleResult())
        priv->Report(FD->getLocation(), Orthodoxy::diag::FunctionOverload());

    if (!config.UserDefinedLiteral && FD->getDeclName().getNameKind() == clang::DeclarationName::CXXLiteralOperatorName)
        priv->Report(FD->getLocation(), Orthodoxy::diag::UserDefinedLiteral());

    if ((!config.Auto || !config.AutoReturn) && Orthodoxy::TypeIsAuto(FD->getReturnType()))
        priv->Report(FD->getReturnTypeSourceRange().getBegin(), Orthodoxy::diag::AutoReturn());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitParmVarDecl(const clang::ParmVarDecl *VD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(VD);

    if (!config.DefaultArgument && VD->hasDefaultArg())
        priv->Report(VD->getDefaultArg()->getExprLoc(), Orthodoxy::diag::DefaultArgument());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitVarDecl(const clang::VarDecl *VD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(VD);

    if ((!config.Auto || !config.AutoVariable) && Orthodoxy::TypeIsAuto(VD->getType()))
        priv->Report(VD->getLocation(), Orthodoxy::diag::AutoVariable());
    else if ((!config.Reference || !config.RValueReference) && Orthodoxy::TypeIsRvalueReference(VD->getType()))
        priv->Report(VD->getLocation(), Orthodoxy::diag::RValueReference());
    else if ((!config.Reference || !config.LValueReference) && Orthodoxy::TypeIsLvalueReference(VD->getType()))
        priv->Report(VD->getLocation(), Orthodoxy::diag::LValueReference());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitFieldDecl(const clang::FieldDecl *FD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(FD);

    if ((!config.Reference || !config.RValueReference) && Orthodoxy::TypeIsRvalueReference(FD->getType()))
        priv->Report(FD->getLocation(), Orthodoxy::diag::RValueReference());
    else if ((!config.Reference || !config.RValueReference) && Orthodoxy::TypeIsRvalueReference(FD->getType()))
        priv->Report(FD->getLocation(), Orthodoxy::diag::RValueReference());
    else if ((!config.Reference || !config.LValueReference) && Orthodoxy::TypeIsLvalueReference(FD->getType()))
        priv->Report(FD->getLocation(), Orthodoxy::diag::LValueReference());

    if (!config.Mutable && FD->isMutable())
        priv->Report(FD->getLocation(), Orthodoxy::diag::Mutable());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitTemplateDecl(const clang::TemplateDecl *TD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(TD);

    if (!config.Concept && llvm::isa<clang::ConceptDecl>(TD))
        priv->Report(TD->getLocation(), Orthodoxy::diag::Concept());
    else if (!config.Template)
        priv->Report(TD->getLocation(), Orthodoxy::diag::Template());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitNamespaceDecl(const clang::NamespaceDecl *ND)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(ND);

    if (!config.Namespace)
        priv->Report(ND->getLocation(), Orthodoxy::diag::Namespace());
    else if (config.NamespaceDepth && Orthodoxy::NamespaceDepth(ND, false) > config.NamespaceDepth)
        priv->Report(ND->getLocation(), Orthodoxy::diag::NamespaceDepth());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitLambdaExpr(const clang::LambdaExpr *LE)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForExpr(LE);

    if (!config.Lambda)
        priv->Report(LE->getExprLoc(), Orthodoxy::diag::Lambda());
    else if (!config.LambdaCapture && LE->capture_begin() != LE->capture_end())
        priv->Report(LE->getExprLoc(), Orthodoxy::diag::LambdaCapture());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCStyleCastExpr(const clang::CStyleCastExpr *CE)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForExpr(CE);

    if (!config.CStyleCast)
        priv->Report(CE->getExprLoc(), Orthodoxy::diag::CStyleCast());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *RD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(RD);

    if (RD->hasDefinition())
    {
        if (!config.VirtualInheritance && RD->vbases_begin() != RD->vbases_end())
            priv->Report(RD->getBeginLoc(), Orthodoxy::diag::VirtualInheritance());
        else if (!config.Inheritance && RD->bases_begin() != RD->bases_end())
            priv->Report(RD->getBeginLoc(), Orthodoxy::diag::Inheritance());

        for (auto it = RD->bases_begin(); it != RD->bases_end(); ++it)
        {
            const clang::CXXBaseSpecifier &BS = *it;
            if ((!config.Access || !config.InheritanceAccess) &&
                BS.getAccessSpecifierAsWritten() != clang::AS_none)
            {
                priv->Report(BS.getBeginLoc(), Orthodoxy::diag::InheritanceAccess());
            }
        }

        if (!config.NonStandardLayout && !RD->isStandardLayout())
            priv->Report(RD->getBeginLoc(), Orthodoxy::diag::NonStandardLayout());
        else if (!config.NonTrivial && !RD->isTrivial())
            priv->Report(RD->getBeginLoc(), Orthodoxy::diag::NonTrivial());
        else if (!config.NonPOD && !RD->isPOD())
            priv->Report(RD->getBeginLoc(), Orthodoxy::diag::NonPOD());
    }

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXMethodDecl(const clang::CXXMethodDecl *MD)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForDecl(MD);

    if (!MD->isImplicit())
    {
        if ((!config.Constructor || !config.CopyConstructor) && Orthodoxy::FunctionIsCopyConstructor(MD))
            priv->Report(MD->getLocation(), Orthodoxy::diag::CopyConstructor());
        else if ((!config.Constructor || !config.MoveConstructor) && Orthodoxy::FunctionIsMoveConstructor(MD))
            priv->Report(MD->getLocation(), Orthodoxy::diag::MoveConstructor());
        else if ((!config.Constructor || !config.ConversionConstructor) && Orthodoxy::FunctionIsImplicitConversionConstructor(MD))
            priv->Report(MD->getLocation(), Orthodoxy::diag::ConversionConstructor());
        else if (!config.Constructor && llvm::isa<clang::CXXConstructorDecl>(MD))
            priv->Report(MD->getLocation(), Orthodoxy::diag::Constructor());
        else if (!config.Destructor && llvm::isa<clang::CXXDestructorDecl>(MD))
            priv->Report(MD->getLocation(), Orthodoxy::diag::Destructor());
        else if (!config.VirtualMemberFunction && MD->isVirtual())
            priv->Report(MD->getLocation(), Orthodoxy::diag::VirtualMemberFunction());
        else if (!config.MemberFunction)
            priv->Report(MD->getLocation(), Orthodoxy::diag::MemberFunction());
    }

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXThrowExpr(const clang::CXXThrowExpr *TE)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForExpr(TE);

    if (!config.Throw || !config.Exception)
        priv->Report(TE->getExprLoc(), Orthodoxy::diag::Throw());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXTryStmt(const clang::CXXTryStmt *TS)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForStmt(TS);

    if (!config.TryCatch || !config.Exception)
        priv->Report(TS->getBeginLoc(), Orthodoxy::diag::TryCatch());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXNamedCastExpr(const clang::CXXNamedCastExpr *CE)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForExpr(CE);

    if ((!config.DynamicCast || !config.NamedCast) && llvm::isa<clang::CXXDynamicCastExpr>(CE))
        priv->Report(CE->getExprLoc(), Orthodoxy::diag::DynamicCast());
    else if ((!config.StaticCast || !config.NamedCast) && llvm::isa<clang::CXXStaticCastExpr>(CE))
        priv->Report(CE->getExprLoc(), Orthodoxy::diag::StaticCast());
    else if ((!config.ConstCast || !config.NamedCast) && llvm::isa<clang::CXXConstCastExpr>(CE))
        priv->Report(CE->getExprLoc(), Orthodoxy::diag::ConstCast());
    else if ((!config.ReinterpretCast || !config.NamedCast) && llvm::isa<clang::CXXReinterpretCastExpr>(CE))
        priv->Report(CE->getExprLoc(), Orthodoxy::diag::ReinterpretCast());
    else if (!config.NamedCast)
        priv->Report(CE->getExprLoc(), Orthodoxy::diag::NamedCast());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXThisExpr(const clang::CXXThisExpr *TE)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForExpr(TE);

    if (!config.ImplicitThis && TE->isImplicitCXXThis())
        priv->Report(TE->getExprLoc(), Orthodoxy::diag::ImplicitThis());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXNewExpr(const clang::CXXNewExpr *NE)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForExpr(NE);

    if (!config.New)
        priv->Report(NE->getExprLoc(), Orthodoxy::diag::New());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXDeleteExpr(const clang::CXXDeleteExpr *DE)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForExpr(DE);

    if (!config.New)
        priv->Report(DE->getExprLoc(), Orthodoxy::diag::New());

    return true;
}

bool OrthodoxyASTConsumer::Private::ASTVisitor::VisitCXXForRangeStmt(const clang::CXXForRangeStmt *FS)
{
    Private *priv = M_priv;
    OrthodoxyConfigManager &CM = *priv->M_CM;
    OrthodoxyConfig &config = CM.GetConfigForStmt(FS);

    if (!config.RangeBasedLoop)
        priv->Report(FS->getBeginLoc(), Orthodoxy::diag::RangeBasedLoop());

    return true;
}

std::optional<clang::DiagnosticBuilder> OrthodoxyASTConsumer::Private::Report(clang::SourceLocation loc, const OrthodoxyDiagDesc &diag)
{
    clang::DiagnosticsEngine &DE = *M_DE;

    if (IsSuppressedLocation(loc, diag)) return {};

#if defined(USE_CLANG_DIAG_DESC)
    unsigned diagID = DE.getDiagnosticIDs()->getCustomDiagID(diag);
    if (M_WarnOnly)
        DE.setSeverity(diagID, clang::diag::Severity::Warning, loc);
#else
    clang::DiagnosticIDs::Level level = M_WarnOnly ?
        clang::DiagnosticIDs::Level::Warning :
        clang::DiagnosticIDs::Level::Error;
    unsigned diagID = DE.getDiagnosticIDs()->getCustomDiagID(
        level, diag.GetDescription());
#endif
    std::optional<clang::DiagnosticBuilder> DB = DE.Report(loc, diagID);
    return DB;
}

void OrthodoxyASTConsumer::Private::PPHooks::InclusionDirective(
    clang::SourceLocation HashLoc,
    const clang::Token &/*IncludeTok*/,
    llvm::StringRef FileName,
    bool /*IsAngled*/,
    clang::CharSourceRange /*FilenameRange*/,
    clang::OptionalFileEntryRef /*File*/,
    llvm::StringRef /*SearchPath*/,
    llvm::StringRef /*RelativePath*/,
#if CLANG_VERSION_MAJOR >= 19
    const clang::Module */*SuggestedModule*/,
    bool /*ModuleImported*/,
#else
    const clang::Module */*Imported*/,
#endif
    clang::SrcMgr::CharacteristicKind FileType)
{
    Private *priv = M_priv;
    if (!priv) return;

    if (FileType == clang::SrcMgr::C_System ||
        FileType == clang::SrcMgr::C_ExternCSystem)
    {
        OrthodoxyConfigManager &CM = *priv->M_CM;
        OrthodoxyConfig &config = CM.GetConfigForSourceLocation(HashLoc);

        bool accept = priv->IsIncludeAccepted(FileName, config);
        if (!accept)
        {
            priv->M_DE = &priv->M_CI->getPreprocessor().getDiagnostics();
            priv->Report(HashLoc, Orthodoxy::diag::Include());
            priv->M_DE = nullptr;
        }
    }
}

bool OrthodoxyASTConsumer::Private::IsSuppressedLocation(clang::SourceLocation mainLoc, const OrthodoxyDiagDesc &diag)
{
    clang::CompilerInstance &CI = *M_CI;
    clang::SourceManager &SM = CI.getSourceManager();

    bool more = true;
    clang::SourceLocation nextLoc = mainLoc;
    while (more)
    {
        clang::SourceLocation nextSpellLoc = SM.getSpellingLoc(nextLoc);

        llvm::SmallVector<clang::SourceLocation, 2> locations = {nextLoc, nextSpellLoc};
        if (nextSpellLoc == nextLoc) locations.pop_back();

        for (clang::SourceLocation loc : locations)
        {
            clang::SourceLocation fileLoc = loc.isFileID() ?
                loc : SM.getFileLoc(loc);
            clang::FileID fileID = SM.getFileID(fileLoc);

            bool invalid = false;
            llvm::StringRef data = SM.getBufferData(fileID, &invalid);
            if (invalid) continue;

            unsigned offset = SM.getFileOffset(fileLoc);
            if (LineHasSuppressionComment(data, offset, diag))
                return true;
        }

        more = false;
        if (nextLoc.isMacroID())
        {
            nextLoc = SM.getImmediateMacroCallerLoc(nextLoc);
            more = true;
        }
    }

    return false;
}

bool OrthodoxyASTConsumer::Private::LineHasSuppressionComment(llvm::StringRef data, unsigned offset, const OrthodoxyDiagDesc &diag)
{
    llvm::SmallString<64> pattern;
    unsigned start = offset;
    unsigned end = offset;
    char c;

    while (start > 0 && (c = data[start - 1]) != '\n' && c != '\r')
        --start;
    while (end < data.size() && (c = data[end]) != '\n' && c != '\r')
        ++end;

    llvm::StringRef line{data.data() + start, end - start};

    const OrthodoxyAbstractDiagDesc *curr = &diag;
    while (curr)
    {
        pattern = {"HERESY(", curr->GetOrthodoxyID(), ")"};
        if (line.contains(pattern)) return true;
        pattern = {"HERESY(", curr->GetOrthodoxyALT(), ")"};
        if (line.contains(pattern)) return true;
        curr = curr->GetSuper();
    }

    return false;
}
