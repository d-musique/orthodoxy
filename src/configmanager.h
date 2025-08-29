// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
BEFORE_LLVM
#include <clang/Basic/SourceLocation.h>
AFTER_LLVM
#include <memory>

namespace clang
{
    class CompilerInstance;
    class Decl;
    class Expr;
    class Stmt;
}

struct OrthodoxyConfig;

class OrthodoxyConfigManager
{
public:
    explicit OrthodoxyConfigManager(clang::CompilerInstance &CI);
    ~OrthodoxyConfigManager();

    static OrthodoxyConfig &GetDefaultConfig();
    OrthodoxyConfig &GetConfigForDecl(const clang::Decl *D);
    OrthodoxyConfig &GetConfigForExpr(const clang::Expr *E);
    OrthodoxyConfig &GetConfigForStmt(const clang::Stmt *S);
    OrthodoxyConfig &GetConfigForSourceLocation(clang::SourceLocation location);
    OrthodoxyConfig &GetConfig(clang::FileID fileID, llvm::StringRef fileName);

private:
    struct Private;
    std::unique_ptr<Private> M_priv;
};
