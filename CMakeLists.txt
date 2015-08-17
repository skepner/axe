cmake_minimum_required(VERSION 3.3)
PROJECT(axe)

include_directories(include)
add_executable(telephone tests/telephone.cc)

# ----------------------------------------------------------------------

set(CXX_VERSION "-std=c++14")

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CXX_WARNING_FLAGS "-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic") #  -Wno-padded
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CXX_WARNING_FLAGS "-Wall -Wextra")
else()
  message(FATAL_ERROR "Unsupported C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# ----------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_VERSION} ${CXX_WARNING_FLAGS}")

# ----------------------------------------------------------------------

enable_testing()
add_test(t_telephone telephone)
# add_dependencies(t_telephone telephone)
