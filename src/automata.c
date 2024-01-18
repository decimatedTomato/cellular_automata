#include "define.h"
#include "automata.h"

extern ruleset_t current_ruleset;

/* Repeated functions */

void render_square_grid(state_t *state) {
    color_t(*color_of)(state_t *state, position_t pos) = state->rules->get_color;
    for (position_t c = 0; c < CELL_COUNT; c++) {
        state->texture[c] = color_of(state, state->cells[c]);
    }
}

color_t color_palette_pick(state_t *state, position_t pos) {
    return current_ruleset.palette[state->cells[pos] % current_ruleset.palette_count];
}

/* Game of life
In this automata a cell can have 2 states, ALIVE or DEAD
Each cell obeys 3 rules
- If a living cell is surrounded by less than 3 other living cells it dies, as via underpopulation
- If a living cell is surrounded by more than 4 other living cells it does, as via overpopulation
- If a dead cell is surrounded by precisely 3 living cells it comes to life, as via reproduction 
*/

struct GoL_data {
    u32 grid_width;
};

u32 get_width() {
    return (*(struct GoL_data*)current_ruleset.automata_specific_data).grid_width;
}

cell_value_t GoL_get_value(state_t *state, position_t pos) {
    u32 width = get_width();
    cell_value_t (*grid)[CELL_COUNT/width] = (cell_value_t (*)[CELL_COUNT/width])state->cells;
    return grid[pos % width][pos / width];
}
cell_value_t GoL_get_next_value(state_t *state, position_t pos) {
    // TODO regular GoL calculation
    return GoL_DEAD;
}