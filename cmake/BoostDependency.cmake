

macro(BoostDependency REQUIRED_COMPONENTS)
  set(Boost_USE_STATIC_LIBS   ON)
  set(Boost_USE_MULTITHREADED ON)
  
  add_definitions(-DBOOST_ALL_NO_LIB)

  find_package(Boost 1.41 REQUIRED COMPONENTS ${REQUIRED_COMPONENTS})

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
  endif()
endmacro(BoostDependency)


macro(BoostHeaders)
  set(Boost_USE_STATIC_LIBS   ON)
  set(Boost_USE_MULTITHREADED ON)

  add_definitions(-DBOOST_ALL_NO_LIB)

  find_package(Boost 1.41 REQUIRED )

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
  endif()
endmacro(BoostHeaders)

function(BoostDependencyLink TARGET)
#  message("Boost_LIBRARIES=${Boost_LIBRARIES}")
  target_link_libraries(${TARGET} ${Boost_LIBRARIES})
endfunction(BoostDependencyLink)