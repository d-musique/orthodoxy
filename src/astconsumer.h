// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
BEFORE_LLVM
#include <clang/AST/ASTConsumer.h>
AFTER_LLVM
#include <memory>

namespace clang
{
    class CompilerInstance;
}

class OrthodoxyASTConsumer : public clang::ASTConsumer
{
public:
    explicit OrthodoxyASTConsumer(clang::CompilerInstance &CI, bool warnOnly);
    ~OrthodoxyASTConsumer();
    void HandleTranslationUnit(clang::ASTContext &AST) override;

private:
    struct Private;
    std::unique_ptr<Private> M_priv;
};
