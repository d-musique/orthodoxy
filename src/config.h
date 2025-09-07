// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
BEFORE_LLVM
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Error.h>
AFTER_LLVM
#include <vector>

namespace llvm { class SourceMgr; }
namespace llvm { class MemoryBuffer; }

struct OrthodoxyConfig
{
    bool InheritParent = false;
    std::vector<std::string> IncludeForbid;
    std::vector<std::string> IncludeAllow;
    bool Class = true;
    bool NonPOD = true;
    bool NonTrivial = true;
    bool NonStandardLayout = true;
    bool Constructor = true;
    bool CopyConstructor = true;
    bool MoveConstructor = true;
    bool ConversionConstructor = true;
    bool Destructor = true;
    bool Inheritance = true;
    bool VirtualInheritance = true;
    bool EnumClass = true;
    bool MemberFunction = true;
    bool VirtualMemberFunction = true;
    bool Exception = true;
    bool Throw = true;
    bool TryCatch = true;
    bool Template = true;
    bool Concept = true;
    bool Module = true;
    bool ModuleImport = true;
    bool ModuleExport = true;
    bool RangeBasedLoop = true;
    bool Access = true;
    bool MemberAccess = true;
    bool InheritanceAccess = true;
    bool ImplicitThis = true;
    bool CStyleCast = true;
    bool NamedCast = true;
    bool DynamicCast = true;
    bool StaticCast = true;
    bool ConstCast = true;
    bool ReinterpretCast = true;
    bool Overload = true;
    bool FunctionOverload = true;
    bool OperatorOverload = true;
    bool ConversionOverload = true;
    bool Reference = true;
    bool RValueReference = true;
    bool LValueReference = true;
    bool Lambda = true;
    bool LambdaCapture = true;
    bool New = true;
    bool Auto = true;
    bool AutoReturn = true;
    bool AutoVariable = true;
    bool UserDefinedLiteral = true;
    bool DefaultArgument = true;
    bool Namespace = true;
    unsigned int NamespaceDepth = 0;
    bool Mutable = true;
};

namespace Orthodoxy {
std::error_code ParseConfig(llvm::MemoryBuffer &mb, OrthodoxyConfig &config);
std::error_code IsConfigInheriting(llvm::MemoryBuffer &mb, bool *inh);
std::error_code RecursiveParseConfig(llvm::StringRef fileName, llvm::ArrayRef<llvm::StringRef> configNames, OrthodoxyConfig &config);
std::string FindConfigUncached(llvm::StringRef fname, llvm::ArrayRef<llvm::StringRef> configNames);
std::string FindDirConfigUncached(llvm::StringRef topdir, llvm::ArrayRef<llvm::StringRef> configNames);
}
