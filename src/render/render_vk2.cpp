#include "render/render_vk2.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#include <mujoco/mjmacro.h>
#include <mujoco/mujoco.h>

void mjr_restoreBuffer(const mjrContext* con) {
    mju_warning("vk mjr_restoreBuffer");
}

void mjr_textActual(int font, const char* txt, const mjrContext* con,
                    float x, float y, float z, float r, float g, float b) {
    mju_warning("vk mjr_textActual");
}

void mjr_setBuffer(int framebuffer, mjrContext* con) {
    mju_warning("vk mjr_setBuffer");
}

void mjr_readPixels(unsigned char* rgb, float* depth,
                          mjrRect viewport, const mjrContext* con) {
    mju_warning("vk mjr_readPixels");
}

void mjr_drawPixels(const unsigned char* rgb, const float* depth,
                          mjrRect viewport, const mjrContext* con) {
    mju_warning("vk mjr_drawPixels");
}

void mjr_blitBuffer(mjrRect src, mjrRect dst,
                          int flg_color, int flg_depth, const mjrContext* con) {
    mju_warning("vk mjr_blitBuffer");
}

void mjr_setAux(int index, const mjrContext* con) {
    mju_warning("vk mjr_setAux");
}

void mjr_blitAux(int index, mjrRect src, int left, int bottom,
                       const mjrContext* con) {
    mju_warning("vk mjr_blitAux");
}

void mjr_text(int font, const char* txt, const mjrContext* con,
                    float x, float y, float r, float g, float b) {
    mju_warning("vk mjr_text");
}

void mjr_overlay(int font, int gridpos, mjrRect viewport,
                       const char* overlay, const char* overlay2, const mjrContext* con) {
    mju_warning("vk mjr_overlay");
}

mjrRect mjr_maxViewport(const mjrContext* con) {
    mju_warning("vk mjr_maxViewport");
    mjrRect rect = {0, 0, 0, 0};
    return rect;
}

void mjr_rectangle(mjrRect viewport, float r, float g, float b, float a) {
    mju_warning("vk mjr_rectangle");
}

void mjr_label(mjrRect viewport, int font, const char* txt,
                     float r, float g, float b, float a, float rt, float gt, float bt,
                     const mjrContext* con) {
    mju_warning("vk mjr_label");
}

void mjr_figure(mjrRect viewport, mjvFigure* fig, const mjrContext* con) {
    mju_warning("vk mjr_figure");
}

// Find first rectangle containing mouse, -1: not found.
int mjr_findRect(int x, int y, int nrect, const mjrRect* rect) {
    // scan
    for (int i = 0; i < nrect; i++) {
        if (x >= rect[i].left &&
            x < rect[i].left + rect[i].width &&
            y >= rect[i].bottom &&
            y < rect[i].bottom + rect[i].height) {
            return i;
        }
    }

    // not found
    return -1;
}