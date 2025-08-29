// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "configmanager.h"
#include "config.h"
#include "utility.h"
BEFORE_LLVM
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
AFTER_LLVM

struct OrthodoxyConfigManager::Private
{
    clang::CompilerInstance *M_CI;
    llvm::SmallMapVector<clang::FileID, std::unique_ptr<OrthodoxyConfig>, 64> M_configs;
};

static const llvm::StringRef YamlNames[] =
{
    "orthodoxy.yml",
    ".orthodoxy.yml",
};

OrthodoxyConfigManager::OrthodoxyConfigManager(clang::CompilerInstance &CI)
{
    Private *priv = new Private;
    M_priv.reset(priv);
    priv->M_CI = &CI;
}

OrthodoxyConfigManager::~OrthodoxyConfigManager()
{
}

OrthodoxyConfig &OrthodoxyConfigManager::GetDefaultConfig()
{
    static OrthodoxyConfig config;
    return config;
}

OrthodoxyConfig &OrthodoxyConfigManager::GetConfigForDecl(const clang::Decl *D)
{
    clang::SourceLocation location = D->getLocation();
    return GetConfigForSourceLocation(location);
}

OrthodoxyConfig &OrthodoxyConfigManager::GetConfigForExpr(const clang::Expr *E)
{
    clang::SourceLocation location = E->getExprLoc();
    return GetConfigForSourceLocation(location);
}

OrthodoxyConfig &OrthodoxyConfigManager::GetConfigForStmt(const clang::Stmt *S)
{
    clang::SourceLocation location = S->getBeginLoc();
    return GetConfigForSourceLocation(location);
}

OrthodoxyConfig &OrthodoxyConfigManager::GetConfigForSourceLocation(clang::SourceLocation location)
{
    Private *priv = M_priv.get();
    clang::CompilerInstance &CI = *priv->M_CI;
    clang::SourceManager &SM = CI.getSourceManager();

    if (location.isMacroID())
        location = SM.getExpansionLoc(location);

    clang::FileID fileID = SM.getFileID(location);
    if (fileID.isInvalid()) return GetDefaultConfig();

    clang::SrcMgr::CharacteristicKind ckind = SM.getFileCharacteristic(location);
    if (ckind != clang::SrcMgr::C_User) return GetDefaultConfig();

    std::optional<llvm::StringRef> fname = SM.getNonBuiltinFilenameForID(fileID);
    if (!fname) return GetDefaultConfig();

    return GetConfig(fileID, *fname);
}

OrthodoxyConfig &OrthodoxyConfigManager::GetConfig(clang::FileID fileID, llvm::StringRef fileName)
{
    Private *priv = M_priv.get();

    auto itConfig = priv->M_configs.find(fileID);
    if (itConfig != priv->M_configs.end())
    {
        OrthodoxyConfig *cached = itConfig->second.get();
        if (!cached) return GetDefaultConfig();
        return *cached;
    }

    std::unique_ptr<OrthodoxyConfig> *mapSlot = &priv->M_configs[fileID];
    std::string yamlPath = Orthodoxy::FindConfigUncached(fileName, YamlNames);
    if (yamlPath.empty()) return GetDefaultConfig();

    std::unique_ptr<OrthodoxyConfig> config(new OrthodoxyConfig{});
    std::error_code ec = Orthodoxy::RecursiveParseConfig(yamlPath, YamlNames, *config);

    if (ec)
    {
        Orthodoxy::error_message() << ec.message() << '\n';
        return GetDefaultConfig();
    }

    mapSlot->reset(config.get());
    return *config.release();
}
