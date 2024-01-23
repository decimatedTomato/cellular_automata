#pragma once

#include "define.h"

/* Picks a value from the color palette of a given ruleset */
color_t color_palette_pick(state_t *state, position_t pos);
void render_square_grid(state_t *state, color_t *texture);
void print_square_grid(state_t *state);

// Game of Life
cell_value_t GoL_get_next_value(state_t *state, position_t pos);
cell_value_t GoL_get_random_value(state_t *state, position_t pos, int rand_int);
enum GoL_states {
    GoL_DEAD = 0,
    GoL_ALIVE = 1,
    GoL_STATES_COUNT
};
#define GoL_palette (color_t[]){ [GoL_DEAD]=BLACK, [GoL_ALIVE]=WHITE }
#define GoL_palette_count ARRAY_LEN(GoL_palette)

// Bob's Brain
cell_value_t BB_get_next_value(state_t *state, position_t pos);
cell_value_t BB_get_random_value(state_t *state, position_t pos, int rand_int);
enum BB_states {
    BB_DEAD = 0,
    BB_DYING = 1,
    BB_ALIVE = 2,
    BB_STATES_COUNT
};
#define BB_palette (color_t[]){ [BB_DEAD]=BLACK, [BB_DYING]=BLUE, [BB_ALIVE]=AZURE }
#define BB_palette_count ARRAY_LEN(BB_palette)

// Wireworld
cell_value_t WW_get_next_value(state_t *state, position_t pos);
cell_value_t WW_get_random_value(state_t *state, position_t pos, int rand_int);
enum WW_states {
    WW_EMPTY = 0,
    WW_ELECTRON_HEAD = 1,
    WW_ELECTRON_TAIL = 2,
    WW_CONDUCTOR = 3,
    WW_STATES_COUNT
};
#define WW_palette (color_t[]){ [WW_EMPTY]=BLACK, [WW_ELECTRON_HEAD]=BLUE, [WW_ELECTRON_TAIL]=RED, [WW_CONDUCTOR]=YELLOW }
#define WW_palette_count ARRAY_LEN(WW_palette)
