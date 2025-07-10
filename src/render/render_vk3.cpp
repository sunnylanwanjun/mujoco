#include "render/render_vk3.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mujoco/mjmacro.h>
#include <mujoco/mjvisualize.h>
#include <mujoco/mujoco.h>
#include "engine/engine_sort.h"
#include "engine/engine_vis_init.h"
#include "render/render_context.h"
#include "render_engine/engine_entry.h"

#include <GLFW/glfw3.h>

void* mjr_init(mjrRect viewport, mjvScene* scn, const mjrContext* con) {
    void* window = EngineInit(viewport.width, viewport.height, nullptr);
    return window;
}

void mjr_render(mjrRect viewport, mjvScene* scn, const mjrContext* con) {
    EngineUpdate();
}

void mjr_finish(void) {
    EngineUnInit();
}

int mjr_getError(void) {
    return 0;
} 