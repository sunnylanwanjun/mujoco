#ifndef THIRD_PARTY_MUJOCO_SRC_RENDER_RENDER_VK_
#define THIRD_PARTY_MUJOCO_SRC_RENDER_RENDER_VK_

#include <mujoco/mjexport.h>
#include <mujoco/mjrender.h>
#include <mujoco/mjvisualize.h>

#if defined(__cplusplus)
extern "C" {
#endif

// 3D rendering
MJAPI void mjr_render(mjrRect viewport, mjvScene* scn, const mjrContext* con);

// call glFinish
MJAPI void mjr_finish(void);

// call glGetError and return result
MJAPI int mjr_getError(void);

#if defined(__cplusplus)
}
#endif
#endif  // THIRD_PARTY_MUJOCO_SRC_RENDER_RENDER_VK_