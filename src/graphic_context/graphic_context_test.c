// File: src/graphic_context/graphic_context_test.c

#include <assert.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "graphic_context.h"
#include "../parameters/parameters.h"

#define ASSERT_TRUE(x)        assert((x) && "ASSERT_TRUE failed: " #x)
#define ASSERT_FALSE(x)       assert(!(x) && "ASSERT_FALSE failed: " #x)
#define ASSERT_EQ_INT(a,b)    assert(((a)==(b)) && "ASSERT_EQ_INT failed")

static parameters_t params_gc(int rows, int cols, int blockSize, int fps) {
    parameters_t p = getNullParameters();

    p.rows = rows; p.rowsDefined = true;
    p.cols = cols; p.colsDefined = true;
    p.blockSize = blockSize; p.blockSizeDefined = true;
    p.fps = fps; p.fpsDefined = true;

    // setGraphicContextParameters() llama areAllParametersSet() y exige todo
    p.delay = 0; p.delayDefined = true;
    p.fullscreen = false; p.fullscreenDefined = true;
    p.party = false; p.partyDefined = true;

    return p;
}

static void test_createGraphicContext_initializes_sdl_and_objects() {
    graphic_context_t* gc = createGraphicContext();

    ASSERT_TRUE(gc != NULL);
    ASSERT_TRUE(gc->window != NULL);
    ASSERT_TRUE(gc->renderer != NULL);

    ASSERT_EQ_INT(gc->blockSize, 0);
    ASSERT_EQ_INT(gc->fps, 0);

    // SDL video debería estar inicializado
    ASSERT_TRUE((SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) != 0);

    destroyGraphicContext(&gc);
    ASSERT_TRUE(gc == NULL);
}

static void test_setGraphicContextParameters_sets_fields_and_window_size_and_shows() {
    graphic_context_t* gc = createGraphicContext();

    parameters_t p = params_gc(/*rows*/ 10, /*cols*/ 20, /*blockSize*/ 7, /*fps*/ 60);
    setGraphicContextParameters(gc, p);

    ASSERT_EQ_INT(gc->blockSize, 7);
    ASSERT_EQ_INT(gc->fps, 60);

    int w = 0, h = 0;
    SDL_GetWindowSize(gc->window, &w, &h);
    ASSERT_EQ_INT(w, 20 * 7);
    ASSERT_EQ_INT(h, 10 * 7);

    // debería estar "shown" luego de SDL_ShowWindow()
    ASSERT_FALSE((SDL_GetWindowFlags(gc->window) & SDL_WINDOW_HIDDEN) != 0);

    destroyGraphicContext(&gc);
    ASSERT_TRUE(gc == NULL);
}

static void test_destroyGraphicContext_null_safe_and_quits_sdl() {
    graphic_context_t* gc = createGraphicContext();
    destroyGraphicContext(&gc);
    ASSERT_TRUE(gc == NULL);

    // SDL_Quit() debería apagar video
    ASSERT_FALSE((SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) != 0);

    // doble destroy no debería explotar
    destroyGraphicContext(&gc);
    ASSERT_TRUE(gc == NULL);
}

int main(void) {
    test_createGraphicContext_initializes_sdl_and_objects();
    test_setGraphicContextParameters_sets_fields_and_window_size_and_shows();
    test_destroyGraphicContext_null_safe_and_quits_sdl();
    return 0;
}
