# Minimum CMake required version for building woow projects.
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Root path of the main project.
set(TESTS_ROOT "${CMAKE_SOURCE_DIR}")

# Root path of the main project.
set(APP_ROOT "${TESTS_ROOT}/")

# Cmake file defining the internal woow variables.
include(${APP_ROOT}/scripts/cmake/variables.cmake)

project(UnitTests CXX C)
set(CMAKE_CXX_STANDARD 11)

message("Preparing to build in ${APP_COMPILE_TYPE} Mode.")

# Cmake file defining utility macros/functions.
include(${APP_ROOT}/scripts/cmake/utils.cmake)

# Including the depends.cmake file depending on the Platform (win or unix). This is the place where platform dependent variables should be defined.
include(${APP_ROOT}/scripts/cmake/depends_${PLATFORM_DIR}.cmake)


# Seting platform dependent C++ compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${APP_CXX_FLAGS}")

# Seting up platform specific exe linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${APP_EXE_LINKER_FLAGS}")

# Setting GTest/GMock path variables
set(GTEST_ROOT "Q:/Victor_space/googletest-release-1.8.1/googletest")
set(GTEST_INCLUDE_DIR "Q:/Victor_space/googletest-release-1.8.1/googletest/include")
set(GTEST_LIBRARY ${GTEST_ROOT})
set(GTEST_MAIN_LIBRARY ${GTEST_ROOT})
set(GMOCK_ROOT "Q:/Victor_space/googletest-release-1.8.1/googlemock")
set(GTEST_INCLUDE_DIRS ${GTEST_ROOT} ${GTEST_INCLUDE_DIR} ${GMOCK_ROOT} "${GMOCK_ROOT}/include")

# Initialize list with mock and fixture files.
file(GLOB unitests_HEADERS "Tests/*.h")
file(GLOB unitests_SOURCES "Tests/*.cpp")
file(GLOB mocks_FILES "mocks/*.h" "mocks/*.cpp")
file(GLOB fixtures_FILES "fixtures/*.h" "fixtures/*.cpp")

# Specify include directories
include_directories(${GTEST_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} "Tests" "mocks" "fixtures" "${APP_ROOT}"
"C:/Users/CVI/Documents/Arduino/hardware/WAV/avr/cores/lgt8f")

set(tested_HEADERS_LIST
    "${APP_ROOT}/sim808.h"
    "${APP_ROOT}/DFRobot_sim808.h"
    )

set(tested_SOURCES_LIST
    "${APP_ROOT}/sim808.cpp"
    "${APP_ROOT}/DFRobot_sim808.cpp"
    )
    
set(bin_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${bin_dir} CACHE PATH "bin" FORCE)
link_directories(${Boost_LIBRARY_DIR})

# Defining the name of the executable.
set(EXE_FILE_NAME "${PROJECT_NAME}${TARGET_POSTFIX}")

# Adding macro definitions.
add_definitions(${PLATFORM_SPECIFIC_DEFS})

# Adding the GTest package
include(GoogleTest)

add_executable(${EXE_FILE_NAME}
    "${GTEST_ROOT}/src/gtest-all.cc"
    "${GMOCK_ROOT}/src/gmock-all.cc"
    ${tested_HEADERS_LIST}
    ${tested_SOURCES_LIST}
    ${fixtures_FILES}
    ${mocks_FILES}
    ${unitests_HEADERS}
    ${unitests_SOURCES}
    )

target_link_libraries(${EXE_FILE_NAME}
    ${PLATFORM_SPECIFIC_LIBRARIES}
    )
    
set_target_properties("${EXE_FILE_NAME}" PROPERTIES LINKER_LANGUAGE CXX)

# Adding tests
gtest_add_tests(${CMAKE_CURRENT_SOURCE_DIR}/bin/${EXE_FILE_NAME} "" ${unitests_SOURCES})

include(CTest)
enable_testing()
