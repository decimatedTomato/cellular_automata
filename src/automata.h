#include "define.h"

/* Picks a value from the color palette of a given ruleset */
color_t color_palette_pick(state_t *state, position_t pos);
void render_square_grid(state_t *state);

// Game of Life
enum GoL_states {
    GoL_DEAD = 0,
    GoL_ALIVE = 1
};
cell_value_t GoL_get_value(state_t *state, position_t pos);
cell_value_t GoL_get_next_value(state_t *state, position_t pos);
color_t GoL_palette[] = { [GoL_DEAD]=BLACK, [GoL_ALIVE]=WHITE };
#define GoL_palette_count ARRAY_LEN(GoL_palette)
