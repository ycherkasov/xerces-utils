

function(AddTestsToLibs TargetName Libs TestsSources LinkDirs)
    find_package(GTest REQUIRED)
  
    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(${TargetName} ${TestsSources})

    foreach(Lib ${Libs})
      message("Link test library: " ${Lib})
      target_link_libraries(${TargetName} ${Lib} ${GTEST_BOTH_LIBRARIES})
    endforeach(Lib)

    add_test(${TargetName} ${TargetName})
endfunction(AddTestsToLibs)

function(AddSystemTestsToLibs TargetName Libs TestsSources)
    find_package(GTest REQUIRED)
  
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(${TargetName} ${TestsSources})
    target_link_libraries(${TargetName} ${Libs} ${GTEST_BOTH_LIBRARIES})

if(BUILD_SYSTEM_TESTING)
    add_test(${TargetName} ${TargetName})
endif()

endfunction(AddSystemTestsToLibs)
