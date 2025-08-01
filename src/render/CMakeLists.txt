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

if(MUJOCO_WITH_VK)

  OPTION(WIN32 "Win32 Platform" ON)
  add_definitions(-DWIN32)
  add_definitions(-D_WIN32)

  set(MUJOCO_RENDER_SRCS
      render_context_vk.cpp
      render_context.h
      render_vk2.cpp
      render_vk2.h
      render_vk3.cpp
      render_vk3.h
  )

  target_sources(mujoco PRIVATE ${MUJOCO_RENDER_SRCS})
  add_definitions(-DRENDER_ENGINE_EMBED)

  add_subdirectory(render_engine)

else()

  set(MUJOCO_RENDER_SRCS
      glad/glad.c
      glad/glad.h
      glad/loader.cc
      render_context.c
      render_context.h
      render_gl2.c
      render_gl2.h
      render_gl3.c
      render_gl3.h
      render_util.c
      render_util.h
  )

  target_sources(mujoco PRIVATE ${MUJOCO_RENDER_SRCS})

  # GLAD needs dlopen/dlsym
  if(CMAKE_DL_LIBS)
    target_link_libraries(mujoco PRIVATE ${CMAKE_DL_LIBS})
  endif()

endif()