# Copyright: D.M., 2025-present
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.31)
include(CheckLinkerFlag)

project(orthodoxy VERSION "1.0.0" LANGUAGES C CXX)

set(ORTHODOXY_LLVM_CONFIG "" CACHE FILEPATH "Optional path to llvm-config")
set(ORTHODOXY_CLANG_PLUGIN_INSTALL_DIR "" CACHE FILEPATH "Optional path where to install the plugin")

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

add_library(orthodoxy MODULE)
target_sources(orthodoxy PRIVATE
  "src/astconsumer.cpp"
  "src/config.cpp"
  "src/configmanager.cpp"
  "src/diag.cpp"
  "src/diagdesc.cpp"
  "src/utility.cpp"
  "src/main.cpp")
target_precompile_headers(orthodoxy PRIVATE
  "src/stdafx.h")
set_target_properties(orthodoxy PROPERTIES
  PREFIX "")

if(NOT ORTHODOXY_LLVM_CONFIG)
  find_package(Clang CONFIG REQUIRED)
  find_package(LLVM CONFIG REQUIRED)
  target_include_directories(orthodoxy PRIVATE
    ${CLANG_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS})
  target_link_libraries(orthodoxy PRIVATE
    clang-cpp LLVM)
else()
  message(STATUS "LLVM config: ${ORTHODOXY_LLVM_CONFIG}")
  #
  execute_process(COMMAND "${ORTHODOXY_LLVM_CONFIG}" "--includedir"
    OUTPUT_VARIABLE LLVM_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY)
  execute_process(COMMAND "${ORTHODOXY_LLVM_CONFIG}" "--libdir"
    OUTPUT_VARIABLE LLVM_LIBRARY_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY)
  message(STATUS "LLVM headers: ${LLVM_INCLUDE_DIR}")
  message(STATUS "LLVM libraries: ${LLVM_LIBRARY_DIR}")
  target_include_directories(orthodoxy PRIVATE "${LLVM_INCLUDE_DIR}")
  target_link_libraries(orthodoxy PRIVATE "clang-cpp" "LLVM")
  target_link_directories(orthodoxy PRIVATE "${LLVM_LIBRARY_DIR}")
endif()

check_linker_flag(C "-Wl,--no-undefined" HAVE_LINKER_FLAG_NO_UNDEFINED)
if(HAVE_LINKER_FLAG_NO_UNDEFINED)
  target_link_options(orthodoxy PRIVATE "-Wl,--no-undefined")
endif()

# Scripts generation
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

# In-tree compiler scripts
if(NOT CMAKE_CROSSCOMPILING AND NOT WIN32)
  foreach(prog IN ITEMS "clang" "clang++")
    orthodoxy_genscript(
      "orthodox-${prog}" "${prog}"
      "${CMAKE_CURRENT_BINARY_DIR}")
  endforeach()
endif()

# Install plugin
include(GNUInstallDirs)
if(NOT ORTHODOXY_CLANG_PLUGIN_INSTALL_DIR)
  install(TARGETS orthodoxy
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/clang/plugins")
else()
  install(TARGETS orthodoxy
    LIBRARY DESTINATION "${ORTHODOXY_CLANG_PLUGIN_INSTALL_DIR}")
endif()
foreach(prog IN ITEMS "clang" "clang++")
  if(NOT ORTHODOXY_CLANG_PLUGIN_INSTALL_DIR)
    orthodoxy_genscript(
      "install-bin/orthodox-${prog}" "${prog}"
      "${CMAKE_INSTALL_FULL_LIBDIR}/clang/plugins")
  else()
    orthodoxy_genscript(
      "install-bin/orthodox-${prog}" "${prog}"
      "${ORTHODOXY_CLANG_PLUGIN_INSTALL_DIR}")
  endif()
  install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/install-bin/orthodox-${prog}"
    DESTINATION "${CMAKE_INSTALL_BINDIR}")
endforeach()

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
