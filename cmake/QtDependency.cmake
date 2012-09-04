

# TODO
#   add macro parm - REQUIRED_PLUGINS
#

macro(QtDependency HeadersToMoc Resources)
  set(QT_QMAKE_EXECUTABLE "${QT_ROOT}/bin/qmake.exe" CACHE FILEPATH "")
  
  find_package(Qt4 REQUIRED)
  
#  find_library(QJPEG_LIB qjpeg PATHS ${QT_PLUGINS_DIR}/imageformats)

  include(${QT_USE_FILE})

  if(NOT HeadersToMoc STREQUAL "")
    qt4_wrap_cpp(MOC_SOURCES ${Files_src_include_SimpleStitch_h})
    source_group("moc" FILES ${MOC_SOURCES})
    if(MSVC_IDE)
      source_group("moc" FILES ${MOC_SOURCES})
    endif()
  endif()
  
  if(NOT Resources STREQUAL "")
    qt4_add_resources(QRC_SOURCES ${Resources})
    if(MSVC_IDE)
      source_group("resources" FILES ${QRC_SOURCES})
    endif()
  endif()
  
#  set(SOURCES ${SOURCES} ${MOC_SOURCES})
  set(SOURCES ${SOURCES} ${MOC_SOURCES} ${QRC_SOURCES})
 
endmacro(QtDependency)


function(QtDependencyLink TARGET)
  target_link_libraries(${TARGET} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
#  target_link_libraries(${TARGET} ${QJPEG_LIB})
endfunction(QtDependencyLink)