// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config.h"
#include "utility.h"
BEFORE_LLVM
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/YAMLTraits.h>
#include <llvm/Support/raw_ostream.h>
AFTER_LLVM

namespace yaml = llvm::yaml;

template <>
struct yaml::MappingTraits<OrthodoxyConfig>
{
    static void mapping(yaml::IO &io, OrthodoxyConfig &info);
};

void yaml::MappingTraits<OrthodoxyConfig>::mapping(yaml::IO &io, OrthodoxyConfig &info)
{
    io.mapOptional("InheritParent", info.InheritParent);
    io.mapOptional("IncludeForbid", info.IncludeForbid);
    io.mapOptional("IncludeAllow", info.IncludeAllow);
    io.mapOptional("Class", info.Class);
    io.mapOptional("NonStandardLayout", info.NonStandardLayout);
    io.mapOptional("NonTrivial", info.NonTrivial);
    io.mapOptional("NonPOD", info.NonPOD);
    io.mapOptional("EnumClass", info.EnumClass);
    io.mapOptional("Constructor", info.Constructor);
    io.mapOptional("Destructor", info.Destructor);
    io.mapOptional("Inheritance", info.Inheritance);
    io.mapOptional("VirtualInheritance", info.VirtualInheritance);
    io.mapOptional("MemberFunction", info.MemberFunction);
    io.mapOptional("VirtualMemberFunction", info.VirtualMemberFunction);
    io.mapOptional("Exception", info.Exception);
    io.mapOptional("Throw", info.Throw);
    io.mapOptional("TryCatch", info.TryCatch);
    io.mapOptional("Template", info.Template);
    io.mapOptional("Concept", info.Concept);
    io.mapOptional("Module", info.Module);
    io.mapOptional("ModuleImport", info.ModuleImport);
    io.mapOptional("ModuleExport", info.ModuleExport);
    io.mapOptional("RangeBasedLoop", info.RangeBasedLoop);
    io.mapOptional("Access", info.Access);
    io.mapOptional("MemberAccess", info.MemberAccess);
    io.mapOptional("InheritanceAccess", info.InheritanceAccess);
    io.mapOptional("ImplicitThis", info.ImplicitThis);
    io.mapOptional("CStyleCast", info.CStyleCast);
    io.mapOptional("NamedCast", info.NamedCast);
    io.mapOptional("DynamicCast", info.DynamicCast);
    io.mapOptional("StaticCast", info.StaticCast);
    io.mapOptional("ConstCast", info.ConstCast);
    io.mapOptional("ReinterpretCast", info.ReinterpretCast);
    io.mapOptional("Overload", info.Overload);
    io.mapOptional("FunctionOverload", info.FunctionOverload);
    io.mapOptional("OperatorOverload", info.OperatorOverload);
    io.mapOptional("ConversionOverload", info.ConversionOverload);
    io.mapOptional("Reference", info.Reference);
    io.mapOptional("RValueReference", info.RValueReference);
    io.mapOptional("LValueReference", info.LValueReference);
    io.mapOptional("Lambda", info.Lambda);
    io.mapOptional("LambdaCapture", info.LambdaCapture);
    io.mapOptional("New", info.New);
    io.mapOptional("Auto", info.Auto);
    io.mapOptional("AutoReturn", info.AutoReturn);
    io.mapOptional("AutoVariable", info.AutoVariable);
    io.mapOptional("UserDefinedLiteral", info.UserDefinedLiteral);
    io.mapOptional("DefaultArgument", info.DefaultArgument);
    io.mapOptional("Namespace", info.Namespace);
}

std::error_code Orthodoxy::ParseConfig(llvm::MemoryBuffer &mb, OrthodoxyConfig &config)
{
    yaml::Input yin(mb);
    yin.setAllowUnknownKeys(true);
    yin >> config;
    return yin.error();
}

struct InheritOrNotConfig
{
    bool InheritParent = false;
};

template <>
struct yaml::MappingTraits<InheritOrNotConfig>
{
    static void mapping(yaml::IO &io, InheritOrNotConfig &info);
};

void yaml::MappingTraits<InheritOrNotConfig>::mapping(yaml::IO &io, InheritOrNotConfig &info)
{
    io.mapOptional("InheritParent", info.InheritParent);
}

static void NonPrintingDiagHandler(const llvm::SMDiagnostic &, void *)
{
}

std::error_code Orthodoxy::IsConfigInheriting(llvm::MemoryBuffer &mb, bool *inh)
{
    yaml::Input yin(mb, nullptr, NonPrintingDiagHandler);
    yin.setAllowUnknownKeys(true);
    InheritOrNotConfig config;
    yin >> config;
    std::error_code err = yin.error();
    if (err) return err;
    *inh = config.InheritParent;
    return {};
}

std::error_code Orthodoxy::RecursiveParseConfig(llvm::StringRef fileName, llvm::ArrayRef<llvm::StringRef> configNames, OrthodoxyConfig &config)
{
    llvm::SmallVector<char, 256> absTmp;

    if (llvm::sys::path::is_relative(fileName))
    {
        absTmp.assign(fileName.begin(), fileName.end());
        std::error_code err = llvm::sys::fs::make_absolute(absTmp);
        if (err) return err;
        fileName = llvm::StringRef(absTmp.data(), absTmp.size());
    }

    std::unique_ptr<llvm::MemoryBuffer> mb;
    bool inherit = false;

    {
        auto mbResult = llvm::MemoryBuffer::getFile(fileName);
        if (!mbResult) return mbResult.getError();
        mb = std::move(*mbResult);
    }

    {
        std::error_code err = IsConfigInheriting(*mb, &inherit);
        if (err) return err;
    }

    if (inherit)
    {
        llvm::StringRef dir = llvm::sys::path::parent_path(fileName);
        std::string parentName = Orthodoxy::FindConfigUncached(
            dir, configNames);
        if (parentName.empty())
        {
            warning_message() << "cannot find inherited configuration for \"" << fileName <<  "\"\n";
        }
        else
        {
            std::error_code err = Orthodoxy::RecursiveParseConfig(parentName, configNames, config);
            if (err) return err;
        }
    }

    return Orthodoxy::ParseConfig(*mb, config);
}

std::string Orthodoxy::FindConfigUncached(llvm::StringRef fname, llvm::ArrayRef<llvm::StringRef> configNames)
{
    llvm::StringRef dir = llvm::sys::path::parent_path(fname);
    return Orthodoxy::FindDirConfigUncached(dir, configNames);
}

std::string Orthodoxy::FindDirConfigUncached(llvm::StringRef topdir, llvm::ArrayRef<llvm::StringRef> configNames)
{
    std::string result;
    llvm::StringRef dir = topdir;
    llvm::StringRef sep = llvm::sys::path::get_separator();
    for (;;)
    {
        for (size_t i = 0; result.empty() && i < configNames.size(); ++i)
        {
            llvm::raw_string_ostream OS(result);
            if (!dir.empty()) OS << dir << sep;
            OS << configNames[i];
            if (llvm::sys::fs::exists(result)) break;
            else result.clear();
        }
        if (!llvm::sys::path::has_parent_path(dir)) break;
        dir = llvm::sys::path::parent_path(dir);
    }
    return result;
}
