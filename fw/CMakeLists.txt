cmake_minimum_required(VERSION 3.2)
project(BtCoreTest)

set (CMAKE_CXX_STANDARD 11)

################################
# GTest
################################
ADD_SUBDIRECTORY ("external/googletest" )
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

################################
# Library
################################

set(BTCORE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/BtCore/src)
set(BTCORE_SOURCE_FILES 
    ${BTCORE_SOURCE_DIR}/Bt/Encoding/Z85.cpp    
    ${BTCORE_SOURCE_DIR}/Bt/SolarMonitor/MessageFilter.cpp
    ${BTCORE_SOURCE_DIR}/Bt/Core/Platform.cpp    
)

include_directories(${BTCORE_SOURCE_DIR})
add_library(BtCore STATIC ${BTCORE_SOURCE_FILES})

################################
# Unit Tests
################################
# Add test cpp file
file(GLOB_RECURSE TEST_SOURCES 
    ${BTCORE_SOURCE_DIR}/../test/*.cpp
)

message ("TEST_SOURCES is ${TEST_SOURCES}")
add_executable( BtCoreTest ${TEST_SOURCES} )
# Link test executable against gtest & gtest_main
target_link_libraries(BtCoreTest BtCore gtest gmock gtest_main)

# add_test(RunBtCoreTest BtCoreTest)
add_custom_target(check COMMAND BtCoreTest DEPENDS BtCoreTest)