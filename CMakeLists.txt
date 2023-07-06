cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)
#Set the project name
project(
  cppsas7bdat
  VERSION 1.0.1
  DESCRIPTION "A C++17 SAS7BDAT reader library"
  LANGUAGES CXX
  )


file(WRITE version.txt ${PROJECT_VERSION})

include(cmake/Sanitizers.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Cache.cmake)
include(cmake/Linker.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(LIBRARY_TYPE STATIC CACHE STRING "type of library to make for lib_cppsas7bdat")
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(BOOST_DATE_TIME_NO_LIB)
add_compile_options(-Wall -Wextra -pedantic -O3)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
configure_linker(project_options)

# standard compiler warnings
set_project_warnings(project_warnings)
enable_sanitizers(project_options)


option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <map>
    <utility>)
endif()

option(ENABLE_CONAN "Use Conan for dependency management" OFF)
if(ENABLE_CONAN)
  include(cmake/Conan.cmake)
  run_conan()
endif()

find_package(Boost REQUIRED COMPONENTS date_time)
include_directories( ${Boost_INCLUDE_DIR} )

add_subdirectory(src)
add_subdirectory(apps)

option(ENABLE_PYTHON "Enable building python module" OFF)
if(ENABLE_PYTHON)
  message("Building python module")
  add_subdirectory(python)
endif()

