#include "ui/ui_main.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <mujoco/mjmacro.h>
#include <mujoco/mujoco.h>
#include "engine/engine_array_safety.h"


//------------------------------------ Default themes ----------------------------------------------

// theme spacing 0 : tight
static const mjuiThemeSpacing themeSpacing0 = {
  270,   // total
  15,    // scroll
  120,   // label
  8,     // section
  6,     // cornersect
  6,     // cornersep
  4,     // itemside
  4,     // itemmid
  4,     // itemver
  8,     // texthor
  4,     // textver
  30,    // linescroll
  4      // samples
};


// theme spacing 1 : wide
static const mjuiThemeSpacing themeSpacing1 = {
  310,   // total
  15,    // scroll
  120,   // label
  10,    // section
  10,    // cornersect
  10,    // cornersep
  7,     // itemside
  7,     // itemmid
  7,     // itemver
  10,    // texthor
  5,     // textver
  30,    // linescroll
  4      // samples
};


// theme color 0 : default
static const mjuiThemeColor themeColor0 = {
  {0.25, 0.25, 0.25},   // master
  {0.12, 0.12, 0.12},   // thumb
  {0.6,  0.2,  0.2},    // secttitle
  {0.1,  0.1,  0.1},    // secttitle2
  {0.45, 0.17, 0.17},   // secttitleuncheck
  {0.45, 0.17, 0.17},   // secttitleuncheck2
  {0.45, 0.17, 0.17},   // secttitlecheck
  {0.45, 0.17, 0.17},   // secttitlecheck2
  {1.0,  1.0,  1.0},    // sectfont
  {0.7,  0.7,  0.7},    // sectsymbol
  {0.1,  0.1,  0.1},    // sectpane
  {0.25, 0.25, 0.25},   // separator
  {0.1,  0.1,  0.1},    // separator2
  {0.0,  0.0,  1.0},    // shortcut
  {1.0,  1.0,  1.0},    // fontactive
  {0.5,  0.5,  0.5},    // fontinactive
  {0.3,  0.3,  0.3},    // decorinactive
  {0.4,  0.4,  0.4},    // decorinactive2
  {0.6,  0.4,  0.4},    // button
  {0.4,  0.4,  0.7},    // check
  {0.4,  0.6,  0.4},    // radio
  {0.4,  0.6,  0.6},    // select
  {0.2,  0.3,  0.3},    // select2
  {0.3,  0.2,  0.3},    // slider
  {0.6,  0.4,  0.6},    // slider2
  {0.6,  0.6,  0.4},    // edit
  {0.7,  0.0,  0.0},    // edit2
  {0.9,  0.9,  0.9}     // cursor
};


// theme color 1 : orange
static const mjuiThemeColor themeColor1 = {
  {0.2,  0.2,  0.2},    // master
  {0.12, 0.12, 0.12},   // thumb
  {0.3,  0.3,  0.3},    // secttitle
  {0.15, 0.15, 0.15},   // secttitle2
  {0.25, 0.25, 0.25},   // secttitleuncheck
  {0.25, 0.25, 0.25},   // secttitleuncheck2
  {0.25, 0.25, 0.25},   // secttitlecheck
  {0.25, 0.25, 0.25},   // secttitlecheck2
  {0.8,  0.8,  0.8},    // sectfont
  {0.7,  0.7,  0.7},    // sectsymbol
  {0.15, 0.15, 0.15},   // sectpane
  {0.2,  0.2,  0.2},    // separator
  {0.15, 0.15, 0.15},   // separator2
  {0.0,  0.0,  1.0},    // shortcut
  {0.9,  0.9,  0.9},    // fontactive
  {0.5,  0.5,  0.5},    // fontinactive
  {0.2,  0.2,  0.2},    // decorinactive
  {0.25, 0.25, 0.25},   // decorinactive2
  {0.6,  0.4,  0.2},    // button
  {0.6,  0.4,  0.2},    // check
  {0.6,  0.4,  0.2},    // radio
  {0.6,  0.4,  0.2},    // select
  {0.3,  0.2,  0.1},    // select2
  {0.2,  0.2,  0.2},    // slider
  {0.6,  0.4,  0.2},    // slider2
  {0.6,  0.4,  0.2},    // edit
  {0.7,  0.0,  0.0},    // edit2
  {0.9,  0.9,  0.9}     // cursor
};


// theme color 2 : white
static const mjuiThemeColor themeColor2 = {
  {0.9,  0.9,  0.9},    // master
  {0.7,  0.7,  0.7},    // thumb
  {0.8,  0.8,  0.8},    // secttitle
  {1.0,  1.0,  1.0},    // secttitle2
  {0.95, 0.95, 0.95},   // secttitleuncheck
  {0.95, 0.95, 0.95},   // secttitleuncheck2
  {0.95, 0.95, 0.95},   // secttitlecheck
  {0.95, 0.95, 0.95},   // secttitlecheck2
  {0.0,  0.0,  0.8},    // sectfont
  {0.0,  0.0,  0.8},    // sectsymbol
  {1.0,  1.0,  1.0},    // sectpane
  {0.9,  0.9,  0.9},    // separator
  {1.0,  1.0,  1.0},    // separator2
  {0.0,  1.0,  1.0},    // shortcut
  {0.0,  0.0,  0.0},    // fontactive
  {0.7,  0.7,  0.7},    // fontinactive
  {0.95, 0.95, 0.95},   // decorinactive
  {0.9,  0.9,  0.9},    // decorinactive2
  {0.8,  0.8,  0.8},    // button
  {0.8,  0.8,  0.8},    // check
  {0.8,  0.8,  0.8},    // radio
  {0.8,  0.8,  0.8},    // select
  {0.9,  0.9,  0.9},    // select2
  {0.95, 0.95, 0.95},   // slider
  {0.8,  0.8,  0.8},    // slider2
  {0.8,  0.8,  0.8},    // edit
  {1.0,  0.3,  0.3},    // edit2
  {0.2,  0.2,  0.2}     // cursor
};


// theme color 3 : black
static const mjuiThemeColor themeColor3 = {
  {0.15, 0.15, 0.15},   // master
  {0.3,  0.3,  0.3},    // thumb
  {0.25, 0.25, 0.25},   // secttitle
  {0.0,  0.0,  0.0},    // secttitle2
  {0.2,  0.2,  0.2},    // secttitleuncheck
  {0.2,  0.2,  0.2},    // secttitleuncheck2
  {0.2,  0.2,  0.2},    // secttitlecheck
  {0.2,  0.2,  0.2},    // secttitlecheck2
  {1.0,  0.3,  0.3},    // sectfont
  {1.0,  0.3,  0.3},    // sectsymbol
  {0.0,  0.0,  0.0},    // sectpane
  {0.15, 0.15, 0.15},   // separator
  {0.0,  0.0,  0.0},    // separator2
  {0.0,  0.0,  1.0},    // shortcut
  {1.0,  1.0,  1.0},    // fontactive
  {0.4,  0.4,  0.4},    // fontinactive
  {0.1,  0.1,  0.1},    // decorinactive
  {0.15, 0.15, 0.15},   // decorinactive2
  {0.3,  0.3,  0.3},    // button
  {0.3,  0.3,  0.3},    // check
  {0.3,  0.3,  0.3},    // radio
  {0.3,  0.3,  0.3},    // select
  {0.15, 0.15, 0.15},   // select2
  {0.15, 0.15, 0.15},   // slider
  {0.3,  0.3,  0.3},    // slider2
  {0.3,  0.3,  0.3},    // edit
  {0.8,  0.2,  0.2},    // edit2
  {0.8,  0.8,  0.8}     // cursor
};

//---------------------------------- Public API ----------------------------------------------------

// Get builtin UI theme spacing (0-1).
mjuiThemeSpacing mjui_themeSpacing(int ind) {
  if (ind == 0) {
    return themeSpacing0;
  } else {
    return themeSpacing1;
  }
}

// Get builtin UI theme color (0-3).
mjuiThemeColor mjui_themeColor(int ind) {
  if (ind == 0) {
    return themeColor0;
  } else if (ind == 1) {
    return themeColor1;
  } else if (ind == 2) {
    return themeColor2;
  } else {
    return themeColor3;
  }
}

void mjui_add(mjUI* ui, const mjuiDef* def) {
    mju_warning("vk mjui_add");
}

void mjui_addToSection(mjUI* ui, int sect, const mjuiDef* def) {
    mju_warning("vk mjui_addToSection");
}

void mjui_resize(mjUI* ui, const mjrContext* con) {
    mju_warning("vk mjui_resize");
}

void mjui_update(int section, int item, const mjUI* ui,
                       const mjuiState* state, const mjrContext* con) {
    mju_warning("vk mjui_update");
}

mjuiItem* mjui_event(mjUI* ui, mjuiState* state, const mjrContext* con) {
    mju_warning("vk mjui_event");
    return NULL;
}

void mjui_render(mjUI* ui, const mjuiState* state, const mjrContext* con) {
    // mju_warning("vk mjui_render");
}
