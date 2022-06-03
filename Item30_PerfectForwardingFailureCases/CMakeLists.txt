cmake_minimum_required(VERSION 3.8...3.21)
project(Item30_PerfectForwardingFailureCases)
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

add_executable( ${PROJECT_NAME} 
  ${PROJECT_SOURCE_DIR}/main.cpp
  )
