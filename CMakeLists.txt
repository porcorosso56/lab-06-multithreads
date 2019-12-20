cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.218.tar.gz"
  SHA1 "9a3594a61227967fb92159339ba326701b287b19"
)

project(multithread)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

#Boost::log + Boost::thread + Boost::filesystem
hunter_add_package(Boost COMPONENTS log thread filesystem system)
find_package(Boost CONFIG REQUIRED log thread filesystem log_setup system)

add_library(picosha2 INTERFACE)

add_executable(multithread
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

#SHA2
target_include_directories(picosha2
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third-party/PicoSHA2
)
target_link_libraries(multithread 
	Boost::log
	Boost::thread	
	Boost::filesystem
	Boost::log_setup
	Boost::system
)

