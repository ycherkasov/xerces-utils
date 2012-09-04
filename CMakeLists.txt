cmake_minimum_required(VERSION 2.8)
project (xmlutils)
set(TARGET xmlutils)

option(BUILD_TESTING "Build the testing tree." ON)
option(BUILD_SYSTEM_TESTING "Build system testing tree." OFF ) 
option(DASHBOARD_READY "Prepare for submitting results to dashboard." OFF)

include("CMakeLists.Files.txt")
include("cmake/AddExecutableFromLib.cmake")
include("cmake/AddTestsToLibs.cmake")
include("cmake/BoostDependency.cmake")
include("cmake/FindBoost.cmake")

include_directories(${INCLUDE_DIRS})

########################################################
# start execution
########################################################
BoostHeaders()

add_library(${TARGET} STATIC ${SOURCES})

message("SOURCES: " ${SOURCES})

if(BUILD_TESTING)
  message("BUILD_TESTING: " ${BUILD_TESTING})
  if(DEFINED TEST_SOURCES)

    set(LibsReqired4Test xerces-c ${TARGET})

    set(TEST_LIBS ${TEST_LIBS} ${LibsReqired4Test})

    message("TEST_SOURCES: " ${TEST_SOURCES})
    message("Link libraries to test executable: " ${TEST_LIBS})

    AddTestsToLibs("${TARGET}Tests" "${TEST_LIBS}" "${TEST_SOURCES}" ".")
  endif()
endif()







