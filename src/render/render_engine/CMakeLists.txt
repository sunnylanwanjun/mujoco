set(LOGIC_MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/logic)
set(ENGINE_MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(LOGIC_MODULE
${LOGIC_MODULE_ROOT}/model_loading.cpp
${LOGIC_MODULE_ROOT}/application.cpp
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
         ${Vulkan_INCLUDE_DIRS}
)

set_property(TARGET render_engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_definitions(-DWORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources/")