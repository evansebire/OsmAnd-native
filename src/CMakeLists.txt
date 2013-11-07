project(OsmAnd-native)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

if(DEFINED CMAKE_COMPILER_ASM_FLAGS)
  set(CMAKE_ASM_FLAGS ${CMAKE_COMPILER_ASM_FLAGS})
endif()
if(DEFINED CMAKE_COMPILER_C_FLAGS)
  set(CMAKE_C_FLAGS ${CMAKE_COMPILER_C_FLAGS})
endif()
if(DEFINED CMAKE_COMPILER_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS ${CMAKE_COMPILER_CXX_FLAGS})
endif()

if (${CMAKE_TARGET_OS} STREQUAL "qnx")
  if (${CMAKE_TARGET_CPU_ARCH} STREQUAL "arm")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QNX_TARGET}/armle-v7/usr)
  elseif (${CMAKE_TARGET_CPU_ARCH} STREQUAL "x86")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QNX_TARGET}/x86/usr)
  endif()
endif()

add_definitions(-DOSMAND_TARGET_OS_${CMAKE_TARGET_OS})

find_path(OSMAND_ROOT_DIRECTORY core/core.cmake)

set(OSMAND_LIBRARIES_PATH_RELEASE
  ${OSMAND_ROOT_DIRECTORY}/binaries/${CMAKE_TARGET_OS}/${CMAKE_TARGET_CPU_ARCH}/Release
)

set(OSMAND_LIBRARIES_PATH_DEBUG
  ${OSMAND_ROOT_DIRECTORY}/binaries/${CMAKE_TARGET_OS}/${CMAKE_TARGET_CPU_ARCH}/Debug
)

find_library(OSMAND_CORE_SHARED_DEBUG OsmAndCore_shared PATHS ${OSMAND_LIBRARIES_PATH_DEBUG})
find_library(OSMAND_CORE_STATIC_DEBUG OsmAndCore_static PATHS ${OSMAND_LIBRARIES_PATH_DEBUG})
find_library(SKIA_STATIC_DEBUG skia_static PATHS ${OSMAND_LIBRARIES_PATH_DEBUG})
find_library(GDAL_STATIC_DEBUG gdal_static PATHS ${OSMAND_LIBRARIES_PATH_DEBUG})
find_library(PNG_STATIC_DEBUG png_static PATHS ${OSMAND_LIBRARIES_PATH_DEBUG})
find_library(JPEG_STATIC_DEBUG jpeg_static PATHS ${OSMAND_LIBRARIES_PATH_DEBUG})
find_library(Z_STATIC_DEBUG z_static PATHS ${OSMAND_LIBRARIES_PATH_DEBUG})

find_library(OSMAND_CORE_SHARED_RELEASE OsmAndCore_shared PATHS ${OSMAND_LIBRARIES_PATH_RELEASE})
find_library(OSMAND_CORE_STATIC_RELEASE OsmAndCore_static PATHS ${OSMAND_LIBRARIES_PATH_RELEASE})
find_library(SKIA_STATIC_RELEASE skia_static PATHS ${OSMAND_LIBRARIES_PATH_RELEASE})
find_library(GDAL_STATIC_RELEASE gdal_static PATHS ${OSMAND_LIBRARIES_PATH_RELEASE})
find_library(PNG_STATIC_RELEASE png_static PATHS ${OSMAND_LIBRARIES_PATH_RELEASE})
find_library(JPEG_STATIC_RELEASE jpeg_static PATHS ${OSMAND_LIBRARIES_PATH_RELEASE})
find_library(Z_STATIC_RELEASE z_static PATHS ${OSMAND_LIBRARIES_PATH_RELEASE})

find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Quick REQUIRED)

# Common configuration
include_directories(
  ${OSMAND_ROOT_DIRECTORY}/core/include
  ${CMAKE_SOURCE_DIR}/include
)

if(CMAKE_TARGET_OS STREQUAL "linux" OR CMAKE_TARGET_OS STREQUAL "darwin" OR CMAKE_TARGET_OS STREQUAL "windows")
  add_definitions(-DOSMAND_OPENGL_RENDERER_SUPPORTED)
endif()
if(CMAKE_TARGET_OS STREQUAL "ios" OR CMAKE_TARGET_OS STREQUAL "android" OR CMAKE_TARGET_OS STREQUAL "qnx")
  add_definitions(-DOSMAND_OPENGLES2_RENDERER_SUPPORTED)
endif()

add_subdirectory(clientcore)

if(CMAKE_TARGET_OS STREQUAL "linux" OR CMAKE_TARGET_OS STREQUAL "darwin" OR CMAKE_TARGET_OS STREQUAL "windows")
  qt5_add_resources(RsrcSources desktop/Resource.qrc)

  add_executable(OsmAnd_native
    desktop/main.cpp
    ${RsrcSources}
  )

  target_link_libraries(OsmAnd_native
    OsmAndClientCore
    debug ${OSMAND_CORE_SHARED_DEBUG}
    optimized ${OSMAND_CORE_SHARED_RELEASE}
  )

  qt5_use_modules(OsmAnd_native
    Core
    Widgets
    Sql
    Quick
  )
elseif(CMAKE_TARGET_OS STREQUAL "qnx")
  add_executable(OsmAnd_native
    blackberry10/main.cpp
    ${MocSources}
  )

  target_link_libraries(OsmAnd_native
    OsmAndClientCore
    gif
    fontconfig
    freetype
    socket
    GLESv2
    EGL
    debug
      ${OSMAND_CORE_STATIC_DEBUG}
      ${SKIA_STATIC_DEBUG}
      ${GDAL_STATIC_DEBUG}
      ${PNG_STATIC_DEBUG}
      ${JPEG_STATIC_DEBUG}
      ${Z_STATIC_DEBUG}
    optimized
      ${OSMAND_CORE_STATIC_RELEASE}
      ${SKIA_STATIC_RELEASE}
      ${GDAL_STATIC_RELEASE}
      ${PNG_STATIC_RELEASE}
      ${JPEG_STATIC_RELEASE}
      ${Z_STATIC_RELEASE}
  )

  qt5_use_modules(OsmAnd_native
    Core
    Sql
    Quick
  )
endif ()
