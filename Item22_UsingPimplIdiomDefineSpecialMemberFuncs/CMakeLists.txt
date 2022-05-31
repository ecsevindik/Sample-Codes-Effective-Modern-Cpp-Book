cmake_minimum_required(VERSION 3.8...3.21)
project(Item22_UsingPimplIdiomDefineSpecialMemberFuncs)
set(CMAKE_CXX_STANDARD 14)

set(MainSources "${PROJECT_SOURCE_DIR}/widget.h" "${PROJECT_SOURCE_DIR}/widget.cpp")

add_executable( ${PROJECT_NAME}
  ${MainSources}
  ${PROJECT_SOURCE_DIR}/main.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ${Utils_SOURCE_DIR}/include
  )


