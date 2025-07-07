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

void mjr_render(mjrRect viewport, mjvScene* scn, const mjrContext* con) {
    // mju_warning("vk mjr_render");
}

void mjr_finish(void) {
    mju_warning("vk mjr_finish");
}

int mjr_getError(void) {
    mju_warning("vk mjr_getError");
    return 0;
} 