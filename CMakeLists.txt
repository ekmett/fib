cmake_minimum_required(VERSION 3.2.2)
project(FIB VERSION 0.1 LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
set(FIB_PROJECT_BRIEF "light-weight work-sharing fibers for c++11")

# we need c++11
add_compile_options(-std=c++11)

# optional doxygen support
option(ENABLE_DOCS "Build Docs" OFF)
if(ENABLE_DOCS)
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
  endif(DOXYGEN_FOUND)
endif()

# boost::context support required
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.47.0 REQUIRED COMPONENTS context)

# libevent support
find_package(Event REQUIRED)

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${Event_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_DISABLE_ASSERTS")
add_library(fib fib.cpp fib/worker.cpp)
target_link_libraries(fib ${Boost_LIBRARIES} ${Event_LIBRARIES})
