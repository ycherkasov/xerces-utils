

macro(OpenCVDependency)

#   cmake_policy( PUSH )  
#   cmake_policy( SET CMP0015 OLD )
   
   find_package(OpenCV REQUIRED)
   
#   cmake_policy( POP )

   include_directories(${OpenCV_INCLUDE_DIRS})

endmacro(OpenCVDependency)


function(OpenCVDependencyLink TARGET)
   target_link_libraries(${TARGET} ${OpenCV_LIBS})
endfunction(OpenCVDependencyLink)