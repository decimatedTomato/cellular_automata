#pragma once

#include "define.h"

/* Picks a value from the color palette of a given ruleset */
color_t color_palette_pick(state_t *state, u32 pos);
void render_square_grid(state_t *state, color_t *texture);
void print_square_grid(state_t *state);

// Game of Life
cell_value_t GoL_get_next_value(cell_value_t *cells, u32 pos);
cell_value_t GoL_get_random_value(int rand_int);
enum GoL_states {
    GoL_DEAD = 0,
    GoL_ALIVE = 1,
    GoL_STATES_COUNT
};
#define GoL_palette (color_t[]){ [GoL_DEAD]=BLACK, [GoL_ALIVE]=WHITE }
#define GoL_palette_count ARRAY_LEN(GoL_palette)

// Bob's Brain
cell_value_t BB_get_next_value(cell_value_t *cells, u32 pos);
cell_value_t BB_get_random_value(int rand_int);
enum BB_states {
    BB_DEAD = 0,
    BB_DYING = 1,
    BB_ALIVE = 2,
    BB_STATES_COUNT
};
#define BB_palette (color_t[]){ [BB_DEAD]=BLACK, [BB_DYING]=BLUE, [BB_ALIVE]=AZURE }
#define BB_palette_count ARRAY_LEN(BB_palette)

// Wireworld
cell_value_t WW_get_next_value(cell_value_t *cells, u32 pos);
cell_value_t WW_get_random_value(int rand_int);
enum WW_states {
    WW_EMPTY = 0,
    WW_ELECTRON_HEAD = 1,
    WW_ELECTRON_TAIL = 2,
    WW_CONDUCTOR = 3,
    WW_STATES_COUNT
};
#define WW_palette (color_t[]){ [WW_EMPTY]=BLACK, [WW_ELECTRON_HEAD]=BLUE, [WW_ELECTRON_TAIL]=RED, [WW_CONDUCTOR]=YELLOW }
#define WW_palette_count ARRAY_LEN(WW_palette)

// Belousov-Zhabatinsky
#define BZ_MAX (1<<8)-1
// #define BZ_MAX (1<<4)-1
#define BZ_g 1
color_t BZ_color_palette_sample(state_t *state, u32 pos);
cell_value_t BZ_get_next_value(cell_value_t *cells, u32 pos);
cell_value_t BZ_get_random_value(int rand_int);
enum BZ_states {
    BZ_EMPTY = 0,
    BZ_PARTIAL = BZ_MAX/2,
    BZ_FULL = BZ_MAX,
    BZ_STATES_COUNT
};
#define BZ_palette (color_t[]){ [BZ_EMPTY]=BLACK, [BZ_FULL]=WHITE }
// #define BZ_palette (color_t[]){ [BZ_EMPTY]=BLACK, [BZ_PARTIAL]=YELLOW, [BZ_FULL]=BLUE }
#define BZ_palette_count ARRAY_LEN(BZ_palette)

// int BZ_indices[] = {
//     BZ_EMPTY,
//     BZ_PARTIAL,
//     BZ_FULL,
// };

// int automata_state_counts[] = {
//     [GAME_OF_LIFE] = GoL_STATES_COUNT,
//     [BRIANS_BRAIN] = BB_STATES_COUNT,
//     [WIRE_WORLD] = WW_STATES_COUNT,
//     [BELOUSOV_ZHABATINSKY] = BZ_STATES_COUNT,
// };
// #define STATES_COUNT_COUNT ARRAY_LEN(automata_state_counts)