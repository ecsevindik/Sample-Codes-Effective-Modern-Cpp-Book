cmake_minimum_required(VERSION 3.8...3.21)
project(Item01_UnderstandTemplateTypeDeduction)
set(CMAKE_CXX_STANDARD 14)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  ## if dependency is missing, then find them
  set(Util_dir ../Utils)

endif()


find_package(Boost REQUIRED)

add_executable( ${PROJECT_NAME} 
  ${PROJECT_SOURCE_DIR}/main.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ${Util_dir}/include
  ${Boost_INCLUDE_DIRS}
  )


