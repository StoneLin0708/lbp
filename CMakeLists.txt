cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project(lbp_svm VERSION 0.1.0)

if( NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
message("   Build Type : " ${CMAKE_BUILD_TYPE})

# include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DARMA_NO_DEBUG")
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(third_party/googletest)
include_directories(${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)

add_subdirectory(third_party/fmt EXCLUDE_FROM_ALL)

find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
  message("-> OpenCV : " ${OpenCV_VERSION})
endif()

find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(src)
add_subdirectory(src)
