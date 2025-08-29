// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "astconsumer.h"
#include "utility.h"
BEFORE_LLVM
#include <clang/Frontend/FrontendPluginRegistry.h>
AFTER_LLVM

class OrthodoxyASTAction : public clang::PluginASTAction
{
public:
    bool ParseArgs(
        const clang::CompilerInstance &CI,
        const std::vector<std::string> &args) override;

    clang::PluginASTAction::ActionType getActionType() override;

    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(
        clang::CompilerInstance &CI,
        llvm::StringRef InFile) override;

private:
    bool M_WarnOnly = false;
};

bool OrthodoxyASTAction::ParseArgs(
    const clang::CompilerInstance &,
    const std::vector<std::string> &args)
{
    size_t i = 0;
    size_t n = args.size();
    while (i < n)
    {
        llvm::StringRef arg = args[i++];
        if (arg == "-warn-only")
        {
            M_WarnOnly = true;
        }
        else
        {
            Orthodoxy::error_message() << "unknown plugin argument \"" << arg << "\"\n";
            return false;
        }
    }
    return true;
}

clang::PluginASTAction::ActionType
OrthodoxyASTAction::getActionType()
{
    return clang::PluginASTAction::ActionType::AddBeforeMainAction;
}

std::unique_ptr<clang::ASTConsumer>
OrthodoxyASTAction::CreateASTConsumer(
    clang::CompilerInstance &CI, llvm::StringRef)
{
    return std::make_unique<OrthodoxyASTConsumer>(CI, M_WarnOnly);
}

static clang::FrontendPluginRegistry::Add<OrthodoxyASTAction> X(
    "orthodoxy", "enforcer of C++ programming guidelines");
