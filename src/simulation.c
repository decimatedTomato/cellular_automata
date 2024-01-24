#include <time.h>

#include "define.h"
#include "automata.h"
#include "handle_opengl.h"

// USER DEFINITIONS
#define CA BELOUSOV_ZHABATINSKY

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define DEFAULT_WINDOW_WIDTH SCREEN_WIDTH/3
#define DEFAULT_WINDOW_HEIGHT SCREEN_HEIGHT/3

#define VERTEX_SHADER_FILE_PATH "res/Shaders/vertex_old.glsl"
#define FRAGMENT_SHADER_FILE_PATH "res/Shaders/fragment_old.glsl"

ruleset_t rulesets[] = {
    (ruleset_t) {
        &color_palette_pick,
        &GoL_get_next_value,
        &GoL_get_random_value,
        GoL_palette,
        GoL_palette_count,
        render_square_grid,
    },
    (ruleset_t) {
        &color_palette_pick,
        &BB_get_next_value,
        &BB_get_random_value,
        BB_palette,
        BB_palette_count,
        render_square_grid,
    },
    (ruleset_t) {
        &color_palette_pick,
        &WW_get_next_value,
        &WW_get_random_value,
        WW_palette,
        WW_palette_count,
        render_square_grid,
    },
    (ruleset_t) {
        &BZ_color_palette_sample,
        &BZ_get_next_value,
        &BZ_get_random_value,
        BZ_palette,
        BZ_palette_count,
        render_square_grid,
    },
};

state_t state = {0};
color_t texture[CELL_COUNT] = {0};
cell_value_t cell_buff1[CELL_COUNT] = {0};
cell_value_t cell_buff2[CELL_COUNT] = {0};
cell_value_t *grid = cell_buff1;
cell_value_t *next = cell_buff2;

// DISPLAY
typedef enum DisplayMode {
    PLAYING, PAUSED
} DisplayMode;

DisplayMode mode = PAUSED;

void init_state(state_t *state) {
    state->ruleset = CA;
    state->rules = &rulesets[CA];
    state->cells = grid;
}

void init_grid(state_t *state) {
    for (cell_value_t c = 0; c < CELL_COUNT; c++) {
        state->cells[c] = state->rules->get_random_value(rand());
    }
}

/* Update everything in simulation */
void update(state_t *state) {
    for (size_t c = 0; c < CELL_COUNT; c++) {
        next[c] = state->rules->get_next_value(state->cells, c);
    }
    cell_value_t *tmp = next;
    next = state->cells;
    state->cells = tmp;
}

bool render(state_t *state) {
    state->rules->render(state, texture);
    update_Texture(texture, GRID_WIDTH, GRID_HEIGHT);
    return render_frame();
}

void restart() {
    init_grid(&state);
}

void load() {
    // load from save
    render_frame();
}

void pause() {
    if (mode == PAUSED) mode = PLAYING;
    else if (mode == PLAYING) mode = PAUSED;
}

void step() {
    mode = PAUSED;
    render_frame();
    update(&state);
}

void save() {
    // save
}

int main() {
    assert((ARRAY_LEN(rulesets) == RULESET_COUNT) && "Number of rulesets does not match expectation");
    long long unsigned int t = time(NULL);
    printf("Random seed %llu\n", t);
    srand(t);

    if(init_GLFW(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Simulation") == -1) exit(1);
    init_Debug_Callback();
    init_Quad();

    init_Shader(VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);
    init_Uniforms();

    // Initialize automata
    init_state(&state);
    init_grid(&state);
    state.rules->render(&state, texture);
    init_Texture(texture, GRID_WIDTH, GRID_HEIGHT);

    /* Loop until the user closes the window */
    bool keep_running = true;
    while (keep_running) {
        take_user_input();
        keep_running = render(&state);
        if (mode != PAUSED) update(&state);
        DEBUG(print_square_grid(&state));
        WAIT(FRAME_DELAY);
    }
    clean_up();
    return 0;
}

