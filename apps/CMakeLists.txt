
find_package(fmt)
find_package(spdlog)

add_executable(cppsas7bdat-ci cppsas7bdat-ci.cpp)

target_link_libraries(cppsas7bdat-ci
  PRIVATE fmt::fmt
  PRIVATE project_options
  PUBLIC cppsas7bdat
  )

if( ipo_supported )
    set_property(TARGET cppsas7bdat-ci PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
