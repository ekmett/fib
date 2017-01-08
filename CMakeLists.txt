cmake_minimum_required(VERSION 3.2.2)
project(FIB VERSION 0.1 LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(Event_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
add_compile_options(-std=c++11)

option(ENABLE_DOCS "Build Docs" OFF)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.47.0 COMPONENTS context)

find_package(Event)

if(ENABLE_DOCS)
  find_package(Doxygen)
endif()

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS}) 
endif()

add_library(fib worker.cpp) 

if(Boost_FOUND)
  target_link_libraries(fib ${Boost_LIBRARIES})
endif()
