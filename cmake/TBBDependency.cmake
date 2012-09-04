
macro(TBBDependency)
  find_package(TBB REQUIRED)
  include_directories(${TBB_INCLUDE_DIRS})
endmacro(TBBDependency)

function(TBBDependencyLink TARGET)
  target_link_libraries(${TARGET} ${TBB_LIBRARIES})
endfunction(TBBDependencyLink)