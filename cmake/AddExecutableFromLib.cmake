function(AddExecutableFromLibProduceMainCpp)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
"
/*
 * main.cpp
 *
 * Generated file
 *
 */
int appmain(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  return appmain(argc, argv);
}
"
  )
endfunction()


function(AddExecutableFromLib Target)
  AddExecutableFromLibProduceMainCpp()
  add_executable(${TARGET} ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
  target_link_libraries(${TARGET} ${TARGET}Lib)
endfunction()

function(AddWin32ExecutableFromLib Target)
  AddExecutableFromLibProduceMainCpp()
#  add_executable(${TARGET} WIN32 ${CMAKE_CURRENT_BINARY_DIR}/main.cpp ${QRC_SOURCES})
  add_executable(${TARGET} WIN32 ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
  target_link_libraries(${TARGET} ${TARGET}Lib)
endfunction()





