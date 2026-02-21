#include "render_state.h"

#include "../grid/grid.h"

render_state_t* createRenderState(grid_t* grid, Sint32 mouseX, Sint32 mouseY) {
    render_state_t* rs = malloc(sizeof(render_state_t));

    rs->mouseX = mouseX;
    rs->mouseY = mouseY;
    rs->grid = makeCopyGrid(grid);

    return rs;
}
void destroyRenderState(render_state_t** renderState) {
    render_state_t* rs = *renderState;

    freeGrid(&rs->grid);
    free(rs);
    *renderState = NULL;

    return;

}

