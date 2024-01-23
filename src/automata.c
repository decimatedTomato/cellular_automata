#include "define.h"
#include "automata.h"

/* Repeated functions */
void print_square_grid(state_t *state) {
    for (int j = GRID_HEIGHT-1; j >= 0; j--) {
        for (int i = 0; i < GRID_WIDTH; i++) {
            printf("%i ", state->cells[j * GRID_WIDTH + i]);
        }
        putchar('\n');
    }
    putchar('\n');
}

void render_square_grid(state_t *state, color_t *texture) {
    color_t(*color_of)(state_t *state, position_t pos) = state->rules->get_color;
    for (position_t c = 0; c < CELL_COUNT; c++) {
        texture[c] = color_of(state, c);
    }
}

color_t color_palette_pick(state_t *state, position_t pos) {
    return state->rules->palette[state->cells[pos] % state->rules->palette_count];
}

/* Game of life
In this automata a cell can have 2 states, ALIVE or DEAD
Each generation:
- If a living cell is surrounded by less than 2 other living cells it dies, as via underpopulation
- If a living cell is surrounded by more than 3 other living cells it does, as via overpopulation
- If a dead cell is surrounded by precisely 3 living cells it comes to life, as via reproduction 
*/

cell_value_t GoL_get_next_value(state_t *state, position_t pos) {
    int is_alive = state->cells[pos];
    int neighbors = -is_alive;
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            u32 x = (pos % GRID_WIDTH + i + GRID_WIDTH) % GRID_WIDTH;
            u32 y = (pos / GRID_WIDTH + j + GRID_HEIGHT) % GRID_HEIGHT;
            neighbors += state->cells[y * GRID_WIDTH + x];
        }
    }

    if (is_alive && (neighbors < 2 || neighbors > 3)) return GoL_DEAD;
    if (!is_alive && neighbors == 3) return GoL_ALIVE;
    return is_alive;
}

cell_value_t GoL_get_random_value(state_t *state, position_t pos, int rand_int) {
    (void) state, (void) pos;
    return rand_int % GoL_STATES_COUNT;
}

/* Brian's Brain
In this automata a cell can have 3 states, ALIVE, DYING or DEAD
Each generation:
- If a dead cell has exactly 2 alive neighbors it comes to life, as via reproduction
- If a cell is alive it begins to die, as via aging
- If a cell is dying it dies
*/

cell_value_t BB_get_next_value(state_t *state, position_t pos) {
    cell_value_t current = state->cells[pos];
    switch (current)
    {
    case BB_ALIVE:
        return BB_DYING;
    case BB_DYING:
        return BB_DEAD;
    case BB_DEAD:
        int neighbors = 0;
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                u32 x = (pos % GRID_WIDTH + i + GRID_WIDTH) % GRID_WIDTH;
                u32 y = (pos / GRID_WIDTH + j + GRID_HEIGHT) % GRID_HEIGHT;
                neighbors += state->cells[y * GRID_WIDTH + x] == BB_ALIVE;
            }
        }
        if (neighbors == 2) {
            return BB_ALIVE;
        }
        return BB_DEAD;
    default:
        assert(0 && "UNREACHABLE");
        break;
    }
    return BB_DEAD;
}

cell_value_t BB_get_random_value(state_t *state, position_t pos, int rand_int) {
    (void) state, (void) pos;
    return rand_int % BB_STATES_COUNT;
}