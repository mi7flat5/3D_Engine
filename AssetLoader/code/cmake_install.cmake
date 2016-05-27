# Install script for directory: C:/assimp-3.1.1/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/assimp-3.1.1/Build/code/Debug/assimpd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/assimp-3.1.1/Build/code/Release/assimp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/assimp-3.1.1/Build/code/MinSizeRel/assimp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/assimp-3.1.1/Build/code/RelWithDebInfo/assimp.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.1.1")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/assimp-3.1.1/Build/code/Debug/assimpd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/assimp-3.1.1/Build/code/Release/assimp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/assimp-3.1.1/Build/code/MinSizeRel/assimp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/assimp-3.1.1/Build/code/RelWithDebInfo/assimp.dll")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "C:/assimp-3.1.1/code/../include/assimp/anim.h"
    "C:/assimp-3.1.1/code/../include/assimp/ai_assert.h"
    "C:/assimp-3.1.1/code/../include/assimp/camera.h"
    "C:/assimp-3.1.1/code/../include/assimp/color4.h"
    "C:/assimp-3.1.1/code/../include/assimp/color4.inl"
    "C:/assimp-3.1.1/code/../include/assimp/config.h"
    "C:/assimp-3.1.1/code/../include/assimp/defs.h"
    "C:/assimp-3.1.1/code/../include/assimp/cfileio.h"
    "C:/assimp-3.1.1/code/../include/assimp/light.h"
    "C:/assimp-3.1.1/code/../include/assimp/material.h"
    "C:/assimp-3.1.1/code/../include/assimp/material.inl"
    "C:/assimp-3.1.1/code/../include/assimp/matrix3x3.h"
    "C:/assimp-3.1.1/code/../include/assimp/matrix3x3.inl"
    "C:/assimp-3.1.1/code/../include/assimp/matrix4x4.h"
    "C:/assimp-3.1.1/code/../include/assimp/matrix4x4.inl"
    "C:/assimp-3.1.1/code/../include/assimp/mesh.h"
    "C:/assimp-3.1.1/code/../include/assimp/postprocess.h"
    "C:/assimp-3.1.1/code/../include/assimp/quaternion.h"
    "C:/assimp-3.1.1/code/../include/assimp/quaternion.inl"
    "C:/assimp-3.1.1/code/../include/assimp/scene.h"
    "C:/assimp-3.1.1/code/../include/assimp/metadata.h"
    "C:/assimp-3.1.1/code/../include/assimp/texture.h"
    "C:/assimp-3.1.1/code/../include/assimp/types.h"
    "C:/assimp-3.1.1/code/../include/assimp/vector2.h"
    "C:/assimp-3.1.1/code/../include/assimp/vector2.inl"
    "C:/assimp-3.1.1/code/../include/assimp/vector3.h"
    "C:/assimp-3.1.1/code/../include/assimp/vector3.inl"
    "C:/assimp-3.1.1/code/../include/assimp/version.h"
    "C:/assimp-3.1.1/code/../include/assimp/cimport.h"
    "C:/assimp-3.1.1/code/../include/assimp/importerdesc.h"
    "C:/assimp-3.1.1/code/../include/assimp/Importer.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/DefaultLogger.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/ProgressHandler.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/IOStream.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/IOSystem.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/Logger.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/LogStream.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/NullLogger.hpp"
    "C:/assimp-3.1.1/code/../include/assimp/cexport.h"
    "C:/assimp-3.1.1/code/../include/assimp/Exporter.hpp"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "C:/assimp-3.1.1/code/../include/assimp/Compiler/pushpack1.h"
    "C:/assimp-3.1.1/code/../include/assimp/Compiler/poppack1.h"
    "C:/assimp-3.1.1/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

