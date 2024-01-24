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
    color_t(*color_of)(state_t *state, u32 pos) = state->rules->get_color;
    for (u32 c = 0; c < CELL_COUNT; c++) {
        texture[c] = color_of(state, c);
    }
}

color_t color_palette_pick(state_t *state, u32 pos) {
    return state->rules->palette[state->cells[pos] % state->rules->palette_count];
}

/* Game of life
In this cellular automata a cell can have 2 states, ALIVE or DEAD
Each generation:
- If a living cell is surrounded by less than 2 other living cells it dies, as via underpopulation
- If a living cell is surrounded by more than 3 other living cells it does, as via overpopulation
- If a dead cell is surrounded by precisely 3 living cells it comes to life, as via reproduction 
*/

cell_value_t GoL_get_next_value(cell_value_t *cells, u32 pos) {
    int is_alive = cells[pos];
    int neighbors = -is_alive;
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            const u32 x = (pos % GRID_WIDTH + i + GRID_WIDTH) % GRID_WIDTH;
            const u32 y = (pos / GRID_WIDTH + j + GRID_HEIGHT) % GRID_HEIGHT;
            neighbors += cells[y * GRID_WIDTH + x];
        }
    }

    if (is_alive && (neighbors < 2 || neighbors > 3)) return GoL_DEAD;
    if (!is_alive && neighbors == 3) return GoL_ALIVE;
    return is_alive;
}

cell_value_t GoL_get_random_value(int rand_int) {
    return rand_int % GoL_STATES_COUNT;
}

/* Brian's Brain
In this cellular automata a cell can have 3 states, ALIVE, DYING or DEAD
Each generation:
- If a dead cell has exactly 2 alive neighbors it comes to life, as via reproduction
- If a cell is alive it begins to die, as via aging
- If a cell is dying it dies
*/

cell_value_t BB_get_next_value(cell_value_t *cells, u32 pos) {
    switch (cells[pos])
    {
    case BB_ALIVE:
        return BB_DYING;
    case BB_DYING:
        return BB_DEAD;
    case BB_DEAD:
        int neighbors = 0;
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                const u32 x = (pos % GRID_WIDTH + i + GRID_WIDTH) % GRID_WIDTH;
                const u32 y = (pos / GRID_WIDTH + j + GRID_HEIGHT) % GRID_HEIGHT;
                neighbors += cells[y * GRID_WIDTH + x] == BB_ALIVE;
            }
        }
        return (neighbors == 2) ? BB_ALIVE : BB_DEAD;
    default:
        assert(0 && "UNREACHABLE");
        break;
    }
    return BB_DEAD;
}

cell_value_t BB_get_random_value(int rand_int) {
    return rand_int % BB_STATES_COUNT;
}

/* Wireworld
In this cellular automata a cell can have 3 states, ALIVE, DYING or DEAD
Each generation:
- If a cell is empty it stays empty
- If a cell contains an electron head it becomes an electron tail,
- If a cell contains an electron tail it becomes a conductor,
- If a cell contains a conductor and one or two of the neighbouring cells are electron heads it becomes an electorn head, otherwise remains conductor
*/

cell_value_t WW_get_next_value(cell_value_t *cells, u32 pos) {
    switch (cells[pos]) {
        case WW_EMPTY:
            return WW_EMPTY;
        case WW_ELECTRON_HEAD:
            return WW_ELECTRON_TAIL;
        case WW_ELECTRON_TAIL:
            return WW_CONDUCTOR;
        case WW_CONDUCTOR:
            int neighbor_electron = 0;
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    const u32 x = (pos % GRID_WIDTH + i + GRID_WIDTH) % GRID_WIDTH;
                    const u32 y = (pos / GRID_WIDTH + j + GRID_HEIGHT) % GRID_HEIGHT;
                    neighbor_electron += cells[y * GRID_WIDTH + x] == WW_ELECTRON_HEAD;
                }
            }
            return (neighbor_electron == 2 || neighbor_electron == 1) ? WW_ELECTRON_HEAD : WW_CONDUCTOR;
        default:
            assert(0 && "UNREACHABLE");
    }
    return WW_EMPTY;
}
cell_value_t WW_get_random_value(int rand_int) {
    if (rand_int % 5 == 0) return WW_ELECTRON_HEAD;
    if (rand_int % 5 == 1) return WW_ELECTRON_TAIL;
    if (rand_int % 5 == 2) return WW_CONDUCTOR;
    return WW_EMPTY;
    // return rand_int % WW_palette_count;
}

/* Belousov-Zhabatinsky
In this cellular automata cells can have discrete values between 0 and BZ_MAX
Each generation:
- If a cell is fully saturated (at BZ_MAX) it resets to 0
- If a cell is between extreme states it becomes the average of its neighbors + BZ_g
- If a cell is at 0 it becomes the average of the number of non-zero neighbors
*/

color_t BZ_color_palette_sample(state_t *state, u32 pos) {
    const float t = (float)state->cells[pos] / BZ_MAX;
    
    const color_t col1 = BZ_palette[0];
    const color_t col2 = BZ_palette[BZ_palette_count-1];
    // const u32 idx1 = BZ_indices[0];
    // const u32 idx2 = BZ_indices[BZ_palette_count-1];
    const float _r = (RED_PORTION(col1) * (1 - t) + RED_PORTION(col2) * (t));
    const float _g = (GREEN_PORTION(col1) * (1 - t) + GREEN_PORTION(col2) * (t));
    const float _b = (BLUE_PORTION(col1) * (1 - t) + BLUE_PORTION(col2) * (t));
    const u8 r = (u8)(_r * UINT8_MAX); // TODO rounding issue?
    const u8 g = (u8)(_g * UINT8_MAX);
    const u8 b = (u8)(_b * UINT8_MAX);

    //TODO mix arbitrary color values
    // u32 range_start = 0;
    // u8 r = 0, g = 0, b = 0;
    // for (size_t color_idx = 0; color_idx < BZ_palette_count; color_idx++) {
    //     const color_t color = BZ_palette[color_idx];
    //     float portion = ((float)(BZ_indices[color_idx])-range_start) / BZ_STATES_COUNT;
    //     r += (u8)(portion * RED_PORTION(color));
    //     g += (u8)(portion * GREEN_PORTION(color));
    //     b += (u8)(portion * BLUE_PORTION(color));
    // }
    return 0xFF000000 | g << 16 | b << 8 | r;
}

cell_value_t BZ_get_next_value(cell_value_t *cells, u32 pos) {
    cell_value_t saturation = cells[pos];
    if (saturation >= BZ_MAX) {
        return BZ_EMPTY;
    } else if (saturation > BZ_EMPTY) {
        int neighboring_saturation = 0;
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                const u32 x = (pos % GRID_WIDTH + i + GRID_WIDTH) % GRID_WIDTH;
                const u32 y = (pos / GRID_WIDTH + j + GRID_HEIGHT) % GRID_HEIGHT;
                neighboring_saturation += cells[y * GRID_WIDTH + x];
            }
        }
        const int new_saturation = neighboring_saturation / 8 + BZ_g;
        return (new_saturation > BZ_MAX) ? BZ_MAX : new_saturation;
    } else {
        int neighbors = 0;
        int neighboring_saturation = 0;
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                const u32 x = (pos % GRID_WIDTH + i + GRID_WIDTH) % GRID_WIDTH;
                const u32 y = (pos / GRID_WIDTH + j + GRID_HEIGHT) % GRID_HEIGHT;
                const int neighbor_saturation = cells[y * GRID_WIDTH + x];
                if (neighbor_saturation > BZ_EMPTY) {
                    neighbors++;
                    neighboring_saturation += neighbor_saturation;
                }
            }
        }
        return (neighbors) ? neighboring_saturation / neighbors : 0;
    }
}

cell_value_t BZ_get_random_value(int rand_int) {
    return rand_int % BZ_MAX;
}