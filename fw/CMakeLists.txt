cmake_minimum_required(VERSION 3.24)
project(BtCoreTest)

set (CMAKE_CXX_STANDARD 17)

################################
# GTest
################################
ADD_SUBDIRECTORY ("external/googletest" )
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

################################
# Platfrom Mock
################################

set(PLATFROM_MOCK_SOURCE_DIR ${PROJECT_SOURCE_DIR}/shared/lib/BtCore/mock)

file(GLOB_RECURSE PLATFROM_MOCK_SOURCES 
    ${PLATFROM_MOCK_SOURCE_DIR}/*.cpp 
)

# message ("PLATFROM_MOCK_SOURCES is ${PLATFROM_MOCK_SOURCES}")

include_directories(${PLATFROM_MOCK_SOURCE_DIR})
add_library(PlatformMock STATIC ${PLATFROM_MOCK_SOURCES})


################################
# Library
################################

set(BTCORE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/shared/lib/BtCore/src)

file(GLOB_RECURSE LIBRARY_SOURCES 
    ${BTCORE_SOURCE_DIR}/Bt/Sensors/VeDirect/*.cpp 
    ${BTCORE_SOURCE_DIR}/Bt/Encoding/Z85.cpp    
    ${BTCORE_SOURCE_DIR}/Bt/SolarMonitor/MessageFilter.cpp
    ${BTCORE_SOURCE_DIR}/Bt/SolarMonitor/Reader.cpp
    ${BTCORE_SOURCE_DIR}/Bt/SolarMonitor/Cli/Tokenizer.cpp
    ${BTCORE_SOURCE_DIR}/Bt/Core/*.cpp
    ${BTCORE_SOURCE_DIR}/Bt/Ui/*.cpp
#    ${BTCORE_SOURCE_DIR}/Bt/Core/Platform.cpp
#    ${BTCORE_SOURCE_DIR}/Bt/Core/Timer.cpp 
)

list(REMOVE_ITEM LIBRARY_SOURCES 
    ${BTCORE_SOURCE_DIR}/Bt/Core/Cloud.cpp
    ${BTCORE_SOURCE_DIR}/Bt/Core/RuntimeLogHandler.cpp
    ${BTCORE_SOURCE_DIR}/Bt/Ui/GfxCanvas.cpp
)

# message ("LIBRARY_SOURCES is ${LIBRARY_SOURCES}")

set(BTCORE_SOURCE_FILES 
    ${LIBRARY_SOURCES}
    #${BTCORE_SOURCE_DIR}/Bt/Sensors/VeDirect/*.cpp 
    #${BTCORE_SOURCE_DIR}/Bt/SolarMonitor/MessageFilter.cpp
    #${BTCORE_SOURCE_DIR}/Bt/Encoding/Z85.cpp    
    #${BTCORE_SOURCE_DIR}/Bt/Core/Platform.cpp    
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

# message ("TEST_SOURCES is ${TEST_SOURCES}")
include_directories(${BTCORE_SOURCE_DIR}/../test)
add_executable( BtCoreTest ${TEST_SOURCES} )
# Link test executable against gtest & gtest_main
target_link_libraries(BtCoreTest BtCore PlatformMock gtest gmock gtest_main)

# add_test(RunBtCoreTest BtCoreTest)
add_custom_target(check COMMAND BtCoreTest DEPENDS BtCoreTest)