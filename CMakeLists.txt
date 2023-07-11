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
add_library(project_warnings INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
# standard compiler warnings
configure_linker(project_options)
set_project_warnings(project_warnings)
enable_sanitizers(project_options)

option(ENABLE_CONAN "Use Conan for dependency management" OFF)
if(ENABLE_CONAN)
  include(cmake/Conan.cmake)
  run_conan()
endif()

find_package(Boost REQUIRED COMPONENTS date_time)
include_directories( ${Boost_INCLUDE_DIR} )
add_subdirectory(src)
add_subdirectory(apps)

