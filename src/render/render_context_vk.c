#include "render/render_context.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mujoco/mjmacro.h>
#include <mujoco/mjvisualize.h>
#include <mujoco/mujoco.h>

// bitmap font definitions
#include "render/font/normal50.inc"
#include "render/font/normal100.inc"
#include "render/font/normal150.inc"
#include "render/font/normal200.inc"
#include "render/font/normal250.inc"
#include "render/font/normal300.inc"
#include "render/font/back50.inc"
#include "render/font/back100.inc"
#include "render/font/back150.inc"
#include "render/font/back200.inc"
#include "render/font/back250.inc"
#include "render/font/back300.inc"
#include "render/font/big50.inc"
#include "render/font/big100.inc"
#include "render/font/big150.inc"
#include "render/font/big200.inc"
#include "render/font/big250.inc"
#include "render/font/big300.inc"

void mjr_defaultContext(mjrContext* con) {

}

void mjr_makeContext(const mjModel* m, mjrContext* con, int fontscale) {

}

void mjr_changeFont(int fontscale, mjrContext* con) {

}

void mjr_addAux(int index, int width, int height, int samples, mjrContext* con) {

}

void mjr_freeContext(mjrContext* con) {

}

void mjr_resizeOffscreen(int offwidth, int offheight, mjrContext* con) {

}

void mjr_uploadTexture(const mjModel* m, const mjrContext* con, int texid) {

}

void mjr_uploadMesh(const mjModel* m, const mjrContext* con, int meshid) {

}

void mjr_uploadHField(const mjModel* m, const mjrContext* con, int hfieldid) {
    
}