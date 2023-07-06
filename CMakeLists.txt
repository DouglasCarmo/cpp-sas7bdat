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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

#include(CheckIPOSupported)
#check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

#if( ipo_supported )
#    message(STATUS "IPO / LTO enabled")
     #set_property(TARGET project_options PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
#else()
#    message(STATUS "IPO / LTO not supported: <${ipo_error}>")
#endif()

# variable controlling whether the build is a shared or static library
if (WIN32)
  set(LIBRARY_TYPE STATIC CACHE STRING "type of library to make for lib_cppsas7bdat")
  set(Boost_USE_STATIC_LIBS ON)
else()
  #set(LIBRARY_TYPE SHARED CACHE STRING "type of library to make for lib_cppsas7bdat")
  set(LIBRARY_TYPE STATIC CACHE STRING "type of library to make for lib_cppsas7bdat")
  set(Boost_USE_STATIC_LIBS ON)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(BOOST_DATE_TIME_NO_LIB)
if (WIN32)
  add_compile_options(/EHsc)
else()
  add_compile_options(-Wall -Wextra -pedantic -O3)
endif()

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_11)

# Add linker configuration
include(cmake/Linker.cmake)
configure_linker(project_options)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)

option(ENABLE_CONAN "Use Conan for dependency management" OFF)
if(ENABLE_CONAN)
  include(cmake/Conan.cmake)
  run_conan()
endif()

find_package(Boost REQUIRED COMPONENTS date_time)
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "BOOST LIBRARIES LOCATION: " ${Boost_LIBRARIES})
include_directories( ${Boost_INCLUDE_DIR} )

add_subdirectory(src)

add_subdirectory(apps)

option(ENABLE_PYTHON "Enable building python module" OFF)
if(ENABLE_PYTHON)
  message("Building python module")
  add_subdirectory(python)
endif()


