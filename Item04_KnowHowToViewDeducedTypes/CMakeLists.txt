cmake_minimum_required(VERSION 3.8...3.21)
project(Item04_KnowHowToViewDeducedTypes)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED)

add_executable( ${PROJECT_NAME} 
  ${PROJECT_SOURCE_DIR}/main.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ${Utils_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  )


