# Copyright: D.M., 2025-present
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.31)
project(orthodoxy VERSION "1.0.0" LANGUAGES C CXX)

include(CheckLinkerFlag)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#---------------------------------------------------------------------
# Options

set(ORTHODOXY_LLVM_CONFIG "" CACHE FILEPATH "Optional path to llvm-config")

#---------------------------------------------------------------------
# Compile flags

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CROSSCOMPILING)
  include(CTest)
else()
  message(STATUS "Compiler is not native Clang. Tests are not available.")
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
endif()

set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

if(NOT MSVC)
  add_compile_options("-Wall" "-Wextra")
endif()

check_linker_flag(C "-Wl,--no-undefined" HAVE_LINKER_FLAG_NO_UNDEFINED)

#---------------------------------------------------------------------
# Dependencies

add_library(orthodoxy-llvm-clang INTERFACE)

set(ACTUAL_LLVM_TOOL_SUFFIX "") # tool suffix, eg. "-21" if "llvm-config-21"

if(ORTHODOXY_LLVM_CONFIG)
  set(ACTUAL_LLVM_CONFIG "${ORTHODOXY_LLVM_CONFIG}")
elseif(NOT CMAKE_CROSSCOMPILING)
  find_program(ACTUAL_LLVM_CONFIG "llvm-config" REQUIRED NO_CACHE)
else()
  set(ACTUAL_LLVM_CONFIG "")
endif()

function(orthodoxy_get_suffix_from_llvmconfig llvmconfig outvar)
  get_filename_component(name "${llvmconfig}" NAME)
  string(FIND "${name}" "llvm-config" pos REVERSE)
  if(pos EQUAL -1)
    set(suffix "")
  else()
    math(EXPR pos "${pos}+11")
    string(SUBSTRING "${name}" "${pos}" -1 suffix)
  endif()
  set("${outvar}" "${suffix}" PARENT_SCOPE)
endfunction()

function(orthodoxy_get_suffix_without_llvmconfig resultvar)
  get_property(configurations
    TARGET llvm-config
    PROPERTY IMPORTED_CONFIGURATIONS)
  list(GET configurations 0 configuration)
  get_property(llvmconfig
    TARGET llvm-config
    PROPERTY "IMPORTED_LOCATION_${configuration}")
  orthodoxy_get_suffix_from_llvmconfig("${llvmconfig}" result)
  set("${resultvar}" "${result}" PARENT_SCOPE)
endfunction()

function(orthodoxy_llvmconfig_get flag resultvar)
  execute_process(COMMAND "${ACTUAL_LLVM_CONFIG}" "${flag}"
    OUTPUT_VARIABLE result
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY)
  set("${resultvar}" "${result}" PARENT_SCOPE)
endfunction()

function(orthodoxy_get_major_version version resultvar)
  string(FIND "${version}" "." pos)
  string(SUBSTRING "${version}" 0 "${pos}" result)
  set("${resultvar}" "${result}" PARENT_SCOPE)
endfunction()

# Find clang with llvm-config (the preferred method)
if(ACTUAL_LLVM_CONFIG)
  message(STATUS "LLVM config: ${ACTUAL_LLVM_CONFIG}")
  orthodoxy_llvmconfig_get("--includedir" LLVM_INCLUDE_DIR)
  orthodoxy_llvmconfig_get("--libdir" LLVM_LIBRARY_DIR)
  orthodoxy_llvmconfig_get("--version" LLVM_VERSION)
  message(STATUS "LLVM headers: ${LLVM_INCLUDE_DIR}")
  message(STATUS "LLVM libraries: ${LLVM_LIBRARY_DIR}")
  message(STATUS "LLVM version: ${LLVM_VERSION}")
  target_include_directories(orthodoxy-llvm-clang INTERFACE "${LLVM_INCLUDE_DIR}")
  target_link_libraries(orthodoxy-llvm-clang INTERFACE "clang-cpp" "LLVM")
  target_link_directories(orthodoxy-llvm-clang INTERFACE "${LLVM_LIBRARY_DIR}")
endif()

# Find clang without llvm-config
if(NOT ACTUAL_LLVM_CONFIG)
  find_package(Clang CONFIG REQUIRED)
  find_package(LLVM CONFIG REQUIRED)
  target_include_directories(orthodoxy-llvm-clang INTERFACE
    ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
  target_link_libraries(orthodoxy-llvm-clang INTERFACE
    clang-cpp LLVM)
endif()

# Find the major version
orthodoxy_get_major_version("${LLVM_VERSION}" LLVM_MAJOR_VERSION)
message(STATUS "LLVM major version: ${LLVM_MAJOR_VERSION}")

# Installation-specific names and paths with llvm-config
if(ACTUAL_LLVM_CONFIG)
  orthodoxy_get_suffix_from_llvmconfig("${ACTUAL_LLVM_CONFIG}" ACTUAL_LLVM_TOOL_SUFFIX)
endif()

# Installation-specific names and paths without llvm-config
if(NOT ACTUAL_LLVM_CONFIG)
  # HACK: get the name of llvm-config from cmake and deduce from that
  orthodoxy_get_suffix_without_llvmconfig(ACTUAL_LLVM_TOOL_SUFFIX)
endif()

# XXX Clang does not currently have a standard install location for plugins
# see llvm/llvm-project#156821
set(ACTUAL_CLANG_PLUGIN_BASE_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/orthodoxy")
set(ACTUAL_CLANG_PLUGIN_DIR "${ACTUAL_CLANG_PLUGIN_BASE_DIR}/${LLVM_MAJOR_VERSION}")

message(STATUS "LLVM tool suffix: ${ACTUAL_LLVM_TOOL_SUFFIX}")
message(STATUS "Clang plugin directory: ${ACTUAL_CLANG_PLUGIN_DIR}")

#---------------------------------------------------------------------
# Target

add_library(orthodoxy MODULE)
target_sources(orthodoxy PRIVATE
  "src/astconsumer.cpp"
  "src/config.cpp"
  "src/configmanager.cpp"
  "src/diag.cpp"
  "src/diagdesc.cpp"
  "src/utility.cpp"
  "src/main.cpp")
target_precompile_headers(orthodoxy PRIVATE "src/stdafx.h")
set_target_properties(orthodoxy PROPERTIES PREFIX "")
target_link_libraries(orthodoxy PRIVATE orthodoxy-llvm-clang)

#---------------------------------------------------------------------
# Script generator

function(orthodoxy_genscript outfile clang plugindir)
  set(pluginext "${CMAKE_SHARED_MODULE_SUFFIX}")
  configure_file(
    "${CMAKE_SOURCE_DIR}/scripts/orthodox-compiler.sh.in"
    "${outfile}"
    FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
    @ONLY)
endfunction()

#---------------------------------------------------------------------
# In-tree compiler scripts

if(NOT CMAKE_CROSSCOMPILING AND NOT WIN32)
  foreach(prog IN ITEMS "clang" "clang++")
    orthodoxy_genscript("orthodox-${prog}"
      "${prog}${ACTUAL_LLVM_TOOL_SUFFIX}"
      "${CMAKE_CURRENT_BINARY_DIR}")
  endforeach()
endif()

#---------------------------------------------------------------------
# Installation

message(STATUS "Plugin will be installed in ${ACTUAL_CLANG_PLUGIN_DIR}")
install(TARGETS orthodoxy LIBRARY DESTINATION "${ACTUAL_CLANG_PLUGIN_DIR}")
foreach(prog IN ITEMS "clang" "clang++")
  orthodoxy_genscript(
    "install-bin/orthodox-${prog}${ACTUAL_LLVM_TOOL_SUFFIX}"
    "${prog}${ACTUAL_LLVM_TOOL_SUFFIX}"
    "${ACTUAL_CLANG_PLUGIN_DIR}")
  install(PROGRAMS
    "${CMAKE_CURRENT_BINARY_DIR}/install-bin/orthodox-${prog}${ACTUAL_LLVM_TOOL_SUFFIX}"
    DESTINATION "${CMAKE_INSTALL_BINDIR}")
endforeach()

configure_package_config_file(
  "cmake/orthodoxyConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/orthodoxyConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/orthodoxy"
  PATH_VARS ACTUAL_CLANG_PLUGIN_BASE_DIR)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/orthodoxyConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/orthodoxy")

configure_file(
  "pkgconfig/orthodoxy-for-clang.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/orthodoxy-for-clang${LLVM_MAJOR_VERSION}.pc"
  @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/orthodoxy-for-clang${LLVM_MAJOR_VERSION}.pc"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/pkgconfig")

#---------------------------------------------------------------------
# Tests

if(BUILD_TESTING AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CROSSCOMPILING)
  find_package(Python REQUIRED COMPONENTS Interpreter)

  file(GLOB_RECURSE ORTHODOXY_TEST_SOURCES CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/test"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*/test.cpp")

  foreach(src IN LISTS ORTHODOXY_TEST_SOURCES)
    add_test(
      NAME "${src}"
      COMMAND
          "${Python_EXECUTABLE}"
          "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test-cpp.py"
          "--cxx" "${CMAKE_CXX_COMPILER}"
          "--plugin" "$<TARGET_FILE:orthodoxy>"
          "${CMAKE_CURRENT_SOURCE_DIR}/test/${src}")
  endforeach()
endif()
