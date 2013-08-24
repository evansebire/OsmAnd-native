project(OsmAnd-native)
cmake_minimum_required(VERSION 2.8)
 
option(Qt5Enabled false)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

find_library(OSMAND_CORE_LIBRARY OsmAnd_core)
find_path(OSMAND_ROOT_DIRECTORY core.cmake)

if (Qt5Enabled)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Sql REQUIRED)
  find_package(Qt5Quick REQUIRED)
else ()
  find_package(Qt4 REQUIRED)
  set(QT_USE_QTDECLARATIVE TRUE)
  include(${QT_USE_FILE})
endif ()

# Common configuration
include_directories(${OSMAND_ROOT_DIRECTORY}/include
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/animator
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/config
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/core
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/device
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/effects
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/gpu
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/images
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/pdf
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/pipe
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/ports
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/svg
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/text
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/utils
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/views
  ${OSMAND_ROOT_DIRECTORY}/externals/skia/upstream.patched/include/xml
  ${OSMAND_ROOT_DIRECTORY}/protos
)

include_directories(${CMAKE_SOURCE_DIR}/include)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux") # OR FreeBSD OR MacOS X OR Windows
  add_definitions(-DOSMAND_OPENGL_RENDERER_SUPPORTED)
endif ()

add_subdirectory(Common)

# Desktop configuration
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux") # OR FreeBSD OR MacOS X OR Windows
  if (Qt5Enabled)
    qt5_add_resources(RsrcSources DesktopQt5/Resource.qrc)
    qt5_wrap_cpp(MocSources ${CMAKE_SOURCE_DIR}/include/OsmAndConfig.h)

    add_executable(OsmAnd_native
      DesktopQt5/main.cpp
      ${MocSources}
      ${RsrcSources}
    )

    target_link_libraries(OsmAnd_native
      ${OSMAND_CORE_LIBRARY}
      OsmAndClientCore
    )

    qt5_use_modules(OsmAnd_native
      Core
      Widgets
      Sql
      Quick
    )

    file(INSTALL ${CMAKE_SOURCE_DIR}/DesktopQt5/main.qml
         DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OsmAnd_native.qml
    )
  else ()
    add_executable(OsmAnd_native
      DesktopQt4/main.cpp
    )

    target_link_libraries(OsmAnd_native
      ${QT_LIBRARIES}
      ${OSMAND_CORE_LIBRARY}
    )

    file(INSTALL ${CMAKE_SOURCE_DIR}/DesktopQt4/main.qml
         DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OsmAnd_native.qml
    )
  endif ()

# Blackberry 10 configuration
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "qnx")

endif ()