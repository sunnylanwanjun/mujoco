# Copyright 2021 DeepMind Technologies Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.16)

# Make CMAKE_C_VISIBILITY_PRESET work properly.
set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)
# INTERPROCEDURAL_OPTIMIZATION is enforced when enabled.
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
# Default to GLVND if available.
set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)
# Avoid BUILD_SHARED_LIBS getting overridden by an option() in ccd.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# This line has to appear before 'PROJECT' in order to be able to disable incremental linking
set(MSVC_INCREMENTAL_DEFAULT ON)

project(
  mujoco
  VERSION 3.3.4
  DESCRIPTION "MuJoCo Physics Simulator"
  HOMEPAGE_URL "https://mujoco.org"
)

enable_language(C)
enable_language(CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(MUJOCO_BUILD_EXAMPLES "Build samples for MuJoCo" ON)
option(MUJOCO_BUILD_SIMULATE "Build simulate library for MuJoCo" ON)
option(MUJOCO_BUILD_TESTS "Build tests for MuJoCo" ON)
option(MUJOCO_TEST_PYTHON_UTIL "Build and test utility libraries for Python bindings" ON)
option(MUJOCO_WITH_USD "Build with OpenUSD" OFF)
option(MUJOCO_WITH_VK "Build vk for MuJoCo" ON)

# Option to provide a path to an existing USD build directory or to Houdini HFS directory.
set(USD_DIR "" CACHE PATH "Path to an existing USD build directory.")
set(HOUDINI_HFS_DIR "" CACHE PATH "Path to Houdini HFS directory to build USD plugins against.")
if(USD_DIR)
  if(EXISTS "${USD_DIR}")
    # If the path is provided, set MUJOCO_WITH_USD to ON and add the path to CMAKE_PREFIX_PATH.
    set(MUJOCO_WITH_USD ON CACHE BOOL "Build with OpenUSD" FORCE)
    list(PREPEND CMAKE_PREFIX_PATH "${USD_DIR}")
    message(STATUS "Using custom USD build directory: ${USD_DIR}")
  else()
    message(WARNING "Invalid path provided for USD_DIR: ${USD_DIR}")
  endif()
elseif(HOUDINI_HFS_DIR)
  if(EXISTS "${HOUDINI_HFS_DIR}")
    set(MUJOCO_WITH_USD ON CACHE BOOL "Build with OpenUSD" FORCE)
    message(STATUS "Building Mujoco USD against Houdini HFS: ${HOUDINI_HFS_DIR}")
  endif()
endif()

if(APPLE AND (MUJOCO_BUILD_EXAMPLES OR MUJOCO_BUILD_SIMULATE))
  enable_language(OBJC)
  enable_language(OBJCXX)
endif()

include(MujocoOptions)
include(MujocoMacOS)
include(MujocoDependencies)

set(MUJOCO_HEADERS
    include/mujoco/mjdata.h
    include/mujoco/mjexport.h
    include/mujoco/mjmacro.h
    include/mujoco/mjmodel.h
    include/mujoco/mjplugin.h
    include/mujoco/mjrender.h
    include/mujoco/mjsan.h
    include/mujoco/mjspec.h
    include/mujoco/mjthread.h
    include/mujoco/mjtnum.h
    include/mujoco/mjui.h
    include/mujoco/mjvisualize.h
    include/mujoco/mjxmacro.h
    include/mujoco/mujoco.h
)

# Add metadata to mujoco.dll when building on Windows.
if(WIN32)
  set(MUJOCO_RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/dist/mujoco.rc)
else()
  set(MUJOCO_RESOURCE_FILES "")
endif()

add_library(mujoco SHARED ${MUJOCO_RESOURCE_FILES})
target_include_directories(
  mujoco
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/>
  PRIVATE src
)

add_subdirectory(plugin/elasticity)
add_subdirectory(plugin/actuator)
add_subdirectory(plugin/sensor)
add_subdirectory(plugin/sdf)
add_subdirectory(src/engine)
add_subdirectory(src/user)
add_subdirectory(src/xml)
add_subdirectory(src/render)
add_subdirectory(src/thread)
add_subdirectory(src/ui)

target_compile_definitions(mujoco PRIVATE _GNU_SOURCE CCD_STATIC_DEFINE MUJOCO_DLL_EXPORTS -DMC_IMPLEM_ENABLE)
if(MUJOCO_ENABLE_AVX_INTRINSICS)
  target_compile_definitions(mujoco PUBLIC mjUSEPLATFORMSIMD)
endif()

target_compile_options(
  mujoco
  PRIVATE ${AVX_COMPILE_OPTIONS}
          ${MUJOCO_MACOS_COMPILE_OPTIONS}
          ${EXTRA_COMPILE_OPTIONS}
          ${MUJOCO_CXX_FLAGS}
)
target_link_options(
  mujoco
  PRIVATE
  ${MUJOCO_MACOS_LINK_OPTIONS}
  ${EXTRA_LINK_OPTIONS}
)

if(MUJOCO_WITH_VK)
  add_definitions(-DMUJOCO_WITH_VK)
  find_package (Vulkan REQUIRED)
  set(render_engine_lib Vulkan::Vulkan glfw render_engine)
else()
  set(render_engine_lib)
endif()

target_link_libraries(
  mujoco
  PRIVATE ccd
          lodepng
          qhullstatic_r
          tinyobjloader
          tinyxml2
          ${render_engine_lib}
)

set_target_properties(
  mujoco PROPERTIES VERSION "${mujoco_VERSION}" PUBLIC_HEADER "${MUJOCO_HEADERS}"
)

# CMake's built-in FRAMEWORK option doesn't give us control over the dylib name inside the
# Framework. We instead make our own Framework here.
if(APPLE AND MUJOCO_BUILD_MACOS_FRAMEWORKS)
  set(TAPI
      "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/tapi"
  )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dist/Info.plist.framework.in
    ${CMAKE_CURRENT_SOURCE_DIR}/dist/Info.framework.plist
  )
  set_target_properties(
    mujoco
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY
               "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/mujoco.framework/Versions/A"
               BUILD_WITH_INSTALL_NAME_DIR TRUE
               INSTALL_NAME_DIR "@rpath/mujoco.framework/Versions/A"
  )
  add_custom_command(
    TARGET mujoco
    POST_BUILD
    COMMAND mkdir -p $<TARGET_FILE_DIR:mujoco>/Headers
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && cp ${MUJOCO_HEADERS} $<TARGET_FILE_DIR:mujoco>/Headers
    COMMAND mkdir -p $<TARGET_FILE_DIR:mujoco>/Modules
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/dist/module.modulemap $<TARGET_FILE_DIR:mujoco>/Modules
    COMMAND mkdir -p $<TARGET_FILE_DIR:mujoco>/Resources
    COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/dist/Info.framework.plist
            $<TARGET_FILE_DIR:mujoco>/Resources/Info.plist
    COMMAND ${TAPI} stubify $<TARGET_FILE:mujoco> -o $<TARGET_FILE_DIR:mujoco>/mujoco.tbd
    COMMAND ln -fhs A $<TARGET_FILE_DIR:mujoco>/../Current
    COMMAND ln -fhs Versions/Current/mujoco.tbd $<TARGET_FILE_DIR:mujoco>/../../mujoco.tbd
    COMMAND ln -fhs Versions/Current/Headers $<TARGET_FILE_DIR:mujoco>/../../Headers
    COMMAND ln -fhs Versions/Current/Modules $<TARGET_FILE_DIR:mujoco>/../../Modules
    COMMAND ln -fhs Versions/Current/Resources $<TARGET_FILE_DIR:mujoco>/../../Resources
    COMMAND_EXPAND_LISTS
  )
endif()

# Add a namespace alias to mujoco to be used by the examples.
# This simulates the install interface when building with sources.
add_library(mujoco::mujoco ALIAS mujoco)

add_subdirectory(model)

# `simulate` defines a macro, embed_in_bundle, that's used by `sample`, so that
# subdirectory needs to be added first.
# TODO: Remove this order dependency.
if(MUJOCO_BUILD_SIMULATE)
  add_subdirectory(simulate)
  #add_subdirectory(python/dist/mujoco-3.3.4/mujoco/simulate)
endif()

#add_subdirectory(python/dist/mujoco-3.3.4/mujoco)

if(MUJOCO_BUILD_EXAMPLES)
  add_subdirectory(sample)
endif()

if(MUJOCO_WITH_USD)
  add_subdirectory(src/experimental/usd)
endif()

if(BUILD_TESTING AND MUJOCO_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
  if(MUJOCO_TEST_PYTHON_UTIL)
    add_subdirectory(python/mujoco/util)
    #add_subdirectory(python/dist/mujoco-3.3.4/mujoco/util)
  endif()
endif()

if(NOT (APPLE AND MUJOCO_BUILD_MACOS_FRAMEWORKS))
  # Install the libraries.
  install(
    TARGETS mujoco
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT runtime
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT runtime
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mujoco" COMPONENT dev
  )

  set(CONFIG_PACKAGE_LOCATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

  # Generate and install the mujocoTargets.cmake file. This defines the targets as
  # IMPORTED libraries for downstream users.
  install(
    EXPORT ${PROJECT_NAME}
    DESTINATION ${CONFIG_PACKAGE_LOCATION}
    NAMESPACE mujoco::
    FILE "${PROJECT_NAME}Targets.cmake"
  )

  include(CMakePackageConfigHelpers)

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${mujoco_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

  configure_package_config_file(
    cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CONFIG_PACKAGE_LOCATION}
  )

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION ${CONFIG_PACKAGE_LOCATION}
  )

  # Install also models into share folder.
  install(
    DIRECTORY model
    DESTINATION "${CMAKE_INSTALL_DATADIR}/mujoco"
    PATTERN "CMakeLists.txt" EXCLUDE
  )
endif()
