#pragma once

/* Project wide definitions */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// #define _DEBUG
#ifdef _DEBUG
    #define DEBUG(X) X 
#else
    #define DEBUG(X)
#endif // _DEBUG

#define DELAY
#define FRAME_DELAY 10

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
typedef uint8_t u8;

#define GRID_WIDTH 640
#define GRID_HEIGHT 360
// #define GRID_WIDTH 1920
// #define GRID_HEIGHT 1080
#define CELL_COUNT GRID_WIDTH*GRID_HEIGHT

#define ARRAY_LEN(X) (sizeof(X)/sizeof(X[0]))

//              0xAABBGGRR
#define BLACK   0xFF000000
#define WHITE   0xFFFFFFFF
#define BLUE    0xFFFF0000
#define RED     0xFF0000FF
#define YELLOW  0xFF00FFFF
#define AZURE   0xFFFF7F0F
typedef u32 cell_value_t;
#define ALPHA_PORTION(COL) \
    ((COL & 0xFF000000) >> 24)
#define RED_PORTION(COL) \
    ((COL & 0x00FF0000) >> 16)
#define GREEN_PORTION(COL) \
    ((COL & 0x0000FF00) >>  8)
#define BLUE_PORTION(COL) \
    ((COL & 0x000000FF) >>  0)

typedef enum {
    GAME_OF_LIFE = 0,
    BRIANS_BRAIN,
    WIRE_WORLD,
    BELOUSOV_ZHABATINSKY,
    // LANGTONS_ANT,
    RULESET_COUNT
} ruleset_name;

typedef u32 color_t;
typedef struct ruleset_t ruleset_t;
typedef struct state_t {
    ruleset_name ruleset;
    ruleset_t *rules;
    cell_value_t *cells;
} state_t;
struct ruleset_t {
    color_t(*get_color)(state_t *state, u32 pos);
    cell_value_t(*get_next_value)(cell_value_t *cells, u32 pos);
    cell_value_t(*get_random_value)(int rand_int);
    color_t *palette;
    size_t palette_count;
    void (*render)(state_t *state, color_t *texture);
};