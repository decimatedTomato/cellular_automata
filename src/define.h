#pragma once

/* Project wide definitions */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define DELAY
#define FRAME_DELAY 100

#ifdef DELAY
    #ifdef _WIN32
        #include <Windows.h>
        #define WAIT(X) Sleep(X)
    #else
        #include <unistd.h>
        #define WAIT(X) usleep((X)*1000)
    #endif
#else
    #define WAIT(X)
#endif // DELAY

typedef uint32_t u32;

#define GRID_WIDTH 6
#define GRID_HEIGHT 3
// #define GRID_WIDTH 192
// #define GRID_HEIGHT 108
#define CELL_COUNT GRID_WIDTH*GRID_HEIGHT

#define ARRAY_LEN(X) (sizeof(X)/sizeof(X[0]))

//            0xAABBGGRR
#define BLACK 0xFF000000
#define WHITE 0xFFFFFFFF
#define BLUE  0xFFFF0000
#define AZURE 0xFFFF7F0F

typedef u32 position_t;
typedef u32 cell_value_t;
typedef u32 color_t;

typedef enum {
    GAME_OF_LIFE = 0,
    BRIANS_BRAIN,
    RULESET_COUNT
} ruleset_name;

typedef struct ruleset_t ruleset_t;
typedef struct state_t {
    ruleset_name ruleset;
    ruleset_t *rules;
    cell_value_t *cells;
} state_t;
struct ruleset_t {
    color_t(*get_color)(state_t *state, position_t pos);
    cell_value_t(*get_next_value)(state_t *state, position_t pos);
    cell_value_t(*get_random_value)(state_t *state, position_t pos, int rand_int);
    color_t *palette;
    size_t palette_count;
    void (*render)(state_t *state, color_t *texture);
};