set(LOGIC_MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/logic)
set(ENGINE_MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CORE_MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/core)

set(LOGIC_MODULE
#${LOGIC_MODULE_ROOT}/model_loading.cpp
${LOGIC_MODULE_ROOT}/mujoco_rendering.cpp
${LOGIC_MODULE_ROOT}/application.cpp
${CORE_MODULE_ROOT}/file_utils.cpp
${CORE_MODULE_ROOT}/file_utils.h
${CORE_MODULE_ROOT}/geo_utils.cpp
${CORE_MODULE_ROOT}/geo_utils.h
${CORE_MODULE_ROOT}/gpu_resource.cpp
${CORE_MODULE_ROOT}/gpu_resource.h
${CORE_MODULE_ROOT}/gpu_buffer.cpp
${CORE_MODULE_ROOT}/gpu_buffer.h
${CORE_MODULE_ROOT}/gpu_image.cpp
${CORE_MODULE_ROOT}/gpu_image.h
${CORE_MODULE_ROOT}/swap_chain.cpp
${CORE_MODULE_ROOT}/swap_chain.h
${CORE_MODULE_ROOT}/device.cpp
${CORE_MODULE_ROOT}/device.h
${CORE_MODULE_ROOT}/frame_buffer.cpp
${CORE_MODULE_ROOT}/frame_buffer.h
${CORE_MODULE_ROOT}/shader.cpp
${CORE_MODULE_ROOT}/shader.h
${CORE_MODULE_ROOT}/core_common.h
)

source_group(TREE ${ENGINE_MODULE_ROOT} FILES
${LOGIC_MODULE}
)

find_package (Vulkan REQUIRED)
add_library(render_engine STATIC 
${LOGIC_MODULE}
${ENGINE_MODULE_ROOT}/engine_entry.cpp
${ENGINE_MODULE_ROOT}/engine_entry.h
)

set_target_properties(render_engine PROPERTIES
                      FOLDER "render_engine")

target_include_directories(
  render_engine
  PUBLIC $<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>
         $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
         $<TARGET_PROPERTY:tinyobjloader,INTERFACE_INCLUDE_DIRECTORIES>
         ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
         ${CMAKE_CURRENT_SOURCE_DIR}
         ${Vulkan_INCLUDE_DIRS}
)

set_property(TARGET render_engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_definitions(-DWORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources/")