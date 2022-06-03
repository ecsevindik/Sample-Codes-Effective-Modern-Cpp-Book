cmake_minimum_required(VERSION 3.8...3.21)
project(Item22_UsingPimplIdiomDefineSpecialMemberFuncs)
set(CMAKE_CXX_STANDARD 14)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  if(NOT CMAKE_CXX_FLAGS MATCHES "std")
      if(CMAKE_BUILD_TYPE MATCHES Debug)
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O0 -Wall -Wuninitialized")
      else()
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -Wall -Wuninitialized")
      endif()
  endif()
endif()

set(MainSources "${PROJECT_SOURCE_DIR}/widget.h" "${PROJECT_SOURCE_DIR}/widget.cpp")

add_executable( ${PROJECT_NAME}
  ${MainSources}
  ${PROJECT_SOURCE_DIR}/main.cpp
  )
