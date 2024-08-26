include(cmake/SystemLink.cmake)
#include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(cojoNES_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(cojoNES_setup_options)
  option(cojoNES_ENABLE_HARDENING "Enable hardening" ON)
  option(cojoNES_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    cojoNES_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    cojoNES_ENABLE_HARDENING
    OFF)

  cojoNES_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR cojoNES_PACKAGING_MAINTAINER_MODE)
    option(cojoNES_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(cojoNES_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(cojoNES_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(cojoNES_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(cojoNES_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(cojoNES_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(cojoNES_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(cojoNES_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(cojoNES_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(cojoNES_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(cojoNES_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(cojoNES_ENABLE_PCH "Enable precompiled headers" OFF)
    option(cojoNES_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(cojoNES_ENABLE_IPO "Enable IPO/LTO" ON)
    option(cojoNES_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(cojoNES_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(cojoNES_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(cojoNES_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(cojoNES_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(cojoNES_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(cojoNES_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(cojoNES_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(cojoNES_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(cojoNES_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(cojoNES_ENABLE_PCH "Enable precompiled headers" OFF)
    option(cojoNES_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      cojoNES_ENABLE_IPO
      cojoNES_WARNINGS_AS_ERRORS
      cojoNES_ENABLE_USER_LINKER
      cojoNES_ENABLE_SANITIZER_ADDRESS
      cojoNES_ENABLE_SANITIZER_LEAK
      cojoNES_ENABLE_SANITIZER_UNDEFINED
      cojoNES_ENABLE_SANITIZER_THREAD
      cojoNES_ENABLE_SANITIZER_MEMORY
      cojoNES_ENABLE_UNITY_BUILD
      cojoNES_ENABLE_CLANG_TIDY
      cojoNES_ENABLE_CPPCHECK
      cojoNES_ENABLE_COVERAGE
      cojoNES_ENABLE_PCH
      cojoNES_ENABLE_CACHE)
  endif()

  cojoNES_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (cojoNES_ENABLE_SANITIZER_ADDRESS OR cojoNES_ENABLE_SANITIZER_THREAD OR cojoNES_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(cojoNES_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(cojoNES_global_options)
  if(cojoNES_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    cojoNES_enable_ipo()
  endif()

  cojoNES_supports_sanitizers()

  if(cojoNES_ENABLE_HARDENING AND cojoNES_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR cojoNES_ENABLE_SANITIZER_UNDEFINED
       OR cojoNES_ENABLE_SANITIZER_ADDRESS
       OR cojoNES_ENABLE_SANITIZER_THREAD
       OR cojoNES_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${cojoNES_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${cojoNES_ENABLE_SANITIZER_UNDEFINED}")
    cojoNES_enable_hardening(cojoNES_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(cojoNES_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(cojoNES_warnings INTERFACE)
  add_library(cojoNES_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  cojoNES_set_project_warnings(
    cojoNES_warnings
    ${cojoNES_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(cojoNES_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    cojoNES_configure_linker(cojoNES_options)
  endif()

  include(cmake/Sanitizers.cmake)
  cojoNES_enable_sanitizers(
    cojoNES_options
    ${cojoNES_ENABLE_SANITIZER_ADDRESS}
    ${cojoNES_ENABLE_SANITIZER_LEAK}
    ${cojoNES_ENABLE_SANITIZER_UNDEFINED}
    ${cojoNES_ENABLE_SANITIZER_THREAD}
    ${cojoNES_ENABLE_SANITIZER_MEMORY})

  set_target_properties(cojoNES_options PROPERTIES UNITY_BUILD ${cojoNES_ENABLE_UNITY_BUILD})

  if(cojoNES_ENABLE_PCH)
    target_precompile_headers(
      cojoNES_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(cojoNES_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    cojoNES_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(cojoNES_ENABLE_CLANG_TIDY)
    cojoNES_enable_clang_tidy(cojoNES_options ${cojoNES_WARNINGS_AS_ERRORS})
  endif()

  if(cojoNES_ENABLE_CPPCHECK)
    cojoNES_enable_cppcheck(${cojoNES_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(cojoNES_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    cojoNES_enable_coverage(cojoNES_options)
  endif()

  if(cojoNES_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(cojoNES_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(cojoNES_ENABLE_HARDENING AND NOT cojoNES_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR cojoNES_ENABLE_SANITIZER_UNDEFINED
       OR cojoNES_ENABLE_SANITIZER_ADDRESS
       OR cojoNES_ENABLE_SANITIZER_THREAD
       OR cojoNES_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    cojoNES_enable_hardening(cojoNES_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()