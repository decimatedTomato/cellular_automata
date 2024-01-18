#pragma once

/* Project wide definitions */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
typedef uint32_t u32;

#define CELL_COUNT 80*45

#define ARRAY_LEN(X) (sizeof(X)/sizeof(X[0]))

#define BLACK 0xFF000000
#define WHITE 0xFFFFFFFF

typedef u32 position_t;
typedef u32 cell_value_t;
typedef u32 color_t;

typedef enum {
    GAME_OF_LIFE = 0,
    RULESET_COUNT
} ruleset_name;

typedef struct ruleset_t ruleset_t;
typedef struct state_t {
    ruleset_name ruleset;
    ruleset_t *rules;
    cell_value_t *cells;
    color_t *texture;
} state_t;
struct ruleset_t {
    color_t(*get_color)(state_t *state, position_t pos);
    cell_value_t(*get_value)(state_t *state, position_t pos);
    cell_value_t(*get_next_value)(state_t *state, position_t pos);
    color_t *palette;
    size_t palette_count;
    void (*render)(state_t *state);
    void *automata_specific_data;
};