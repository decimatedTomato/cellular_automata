#include <time.h>

// Portability issues
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#include "automata.h"
#include "handle_opengl.c"

// USER DEFINITIONS
#define DEFAULT_WINDOW_WIDTH SCREEN_WIDTH/3
#define DEFAULT_WINDOW_HEIGHT SCREEN_HEIGHT/3
// #define DEFAULT_WINDOW_WIDTH 640
// #define DEFAULT_WINDOW_HEIGHT 480

#define GRID_WIDTH 50
#define GRID_HEIGHT 50

#define VERTEX_SHADER_FILE_PATH "res/Shaders/vertex_old.glsl"
#define SHADER_FILE_PATH "res/Shaders/frag_test.glsl"
#define START_PAUSED true

ruleset_t rulesets[] = {
    (ruleset_t) {
        &color_palette_pick,
        &GoL_get_value,
        &GoL_get_next_value,
        GoL_palette,
        GoL_palette_count,
        render_square_grid,
        NULL,
    },
};
#define A ARRAY_LEN(GoL_palette);
// assert(("The number of defined rulesets is incorrect", ARRAY_LEN(rulesets) == RULESET_COUNT));

state_t state = {0};
color_t texture[CELL_COUNT] = {0};
cell_value_t grid[CELL_COUNT] = {0};
cell_value_t next[CELL_COUNT] = {0};

// DISPLAY
typedef enum DisplayMode {
    PLAYING, PAUSED
} DisplayMode;

DisplayMode mode = PLAYING;
int window_width = DEFAULT_WINDOW_WIDTH;
int window_height = DEFAULT_WINDOW_HEIGHT;
bool is_fullscreen = false;

void init_Texture() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GRID_WIDTH, GRID_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
}

void init_state(state_t *state) {
    state->ruleset = 0;
    state->rules = rulesets;
    state->cells = grid;
    state->texture = texture;
}

void swap_buffers(cell_value_t *buff1, cell_value_t *buff2) {
    cell_value_t *tmp = buff1;
    buff1 = buff2;
    buff2 = tmp;
}

/* Update everything in simulation */
void update(state_t *state) {
    // update cells
    for (size_t c = 0; c < CELL_COUNT; c++) {
        next[c] = state->rules->get_next_value(state, c);
    }
    swap_buffers(state->cells, next);
}

void render(state_t *state) {
    state->rules->render(state);
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
    update(&state);
    render_frame();
}

void save() {
    // save
}

int main() {
    long long unsigned int t = time(NULL);
    printf("Random seed %llu\n", t);
    srand(t);

    if(init_GLFW(window_width, window_height, "Simulation") == -1) exit(1);
    init_Debug_Callback();
    init_Quad();

    init_Shader(VERTEX_SHADER_FILE_PATH, SHADER_FILE_PATH);
    init_Uniforms();
    init_Texture();

    // Initialize automata
    init_state(&state);

    /* Loop until the user closes the window */
    bool keep_running = true;
    while (keep_running) {
        take_user_input();
        keep_running = render_frame();
        if (mode != PAUSED) update(&state);
    }

    clean_up();
    return 0;
}

