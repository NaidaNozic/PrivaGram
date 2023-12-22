cmake_minimum_required(VERSION 3.11.1)
project(PIR CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build." FORCE)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# find boost library
find_package(Boost 1.70
COMPONENTS
system
program_options
thread
chrono
REQUIRED
)

# provides keccak library
add_subdirectory(keccak)

# PIR
set(PIR_CLASS_SRC
  SEAL_Server.cpp
  SEAL_Client.cpp
  Share.cpp
  AddShare.cpp
  ShamirShare.cpp
  NetworkAdapter.cpp
  PIR_Client.cpp
  PIR_Server.cpp
  utils.cpp
)

# Import Microsoft SEAL
find_package(SEAL 3.7 REQUIRED
        # Optionaly providing a path so this can be built without installing SEAL
        PATHS ${PIR_SOURCE_DIR}/SEAL/build/cmake
)

add_library(pir_class ${PIR_CLASS_SRC})
target_link_libraries(pir_class keccak Threads::Threads SEAL::seal)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_executable(client client_runner.cpp)
add_executable(server server_runner.cpp)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(client pir_class ${Boost_LIBRARIES})
target_link_libraries(server pir_class ${Boost_LIBRARIES})

set(BUILD_TESTS ON CACHE BOOL "Build unit tests.")

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    macro(FetchContent_MakeAvailable NAME)
        FetchContent_GetProperties(${NAME})
        if(NOT ${NAME}_POPULATED)
            FetchContent_Populate(${NAME})
            add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR})
        endif()
    endmacro()
endif()

# test related stuff, only build if desired
if(BUILD_TESTS)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Catch2_SOURCE_DIR}/contrib/")

add_executable(keccak_test tests/keccak_test.cpp)
target_link_libraries(keccak_test Catch2::Catch2 keccak)

add_executable(addshare_test tests/addshare_test.cpp)
target_link_libraries(addshare_test Catch2::Catch2 pir_class)

add_executable(shamirshare_test tests/shamirshare_test.cpp)
target_link_libraries(shamirshare_test Catch2::Catch2 pir_class)

add_executable(he_test tests/he_test.cpp)
target_link_libraries(he_test Catch2::Catch2 pir_class)

include(CTest)
include(Catch)
catch_discover_tests(keccak_test)
catch_discover_tests(addshare_test)
catch_discover_tests(shamirshare_test)
catch_discover_tests(he_test)
endif()
