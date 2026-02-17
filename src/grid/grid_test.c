// File: src/grid/grid_test.c

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"

#define ASSERT_TRUE(x)  assert((x) && "ASSERT_TRUE failed: " #x)
#define ASSERT_FALSE(x) assert(!(x) && "ASSERT_FALSE failed: " #x)
#define ASSERT_EQ_INT(a,b) assert(((a)==(b)) && "ASSERT_EQ_INT failed")
#define ASSERT_EQ_BOOL(a,b) assert(((a)==(b)) && "ASSERT_EQ_BOOL failed")

static void assert_cells_equal(const grid_t* g, const bool* expected) {
    int n = g->rows * g->cols;
    for (int i = 0; i < n; i++) {
        ASSERT_EQ_BOOL(g->cells[i], expected[i]);
    }
}

static void test_initializeGrid_sets_zero_and_null() {
    grid_t g;

    // Ensuciamos para que el test tenga sentido
    g.rows = 123;
    g.cols = 456;
    g.cells = (bool*)0x1;

    initializeGrid(&g);

    ASSERT_EQ_INT(g.rows, 0);
    ASSERT_EQ_INT(g.cols, 0);
    ASSERT_TRUE(g.cells == NULL);
}

static void test_makeGrid_copies_buffer_not_alias() {
    bool init[] = {
        1, 0, 1,
        0, 1, 0
    };

    grid_t g = makeGrid(2, 3, init);

    ASSERT_EQ_INT(g.rows, 2);
    ASSERT_EQ_INT(g.cols, 3);
    ASSERT_TRUE(g.cells != NULL);

    // Contenido igual
    assert_cells_equal(&g, init);

    // Pero NO alias: si cambio init, g no cambia
    init[0] = 0;
    ASSERT_TRUE(g.cells[0] == 1);

    freeGrid(g);
}

static void test_shiftGrid_left_moves_content_right() {
    // DIRECTION_LEFT => dx=1, dy=0
    // buffer[y,x] = old[y, x+1]  -> se desplaza "hacia la izquierda" visualmente (pierde la col 0 y mete false al final)
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, init);

    shiftGrid(g, DIRECTION_LEFT);

    bool expected[] = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0
    };
    assert_cells_equal(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_right_moves_content_left() {
    // DIRECTION_RIGHT => dx=-1
    // buffer[y,x] = old[y, x-1] -> mete false en col 0 y corre todo a la derecha (contenido va a la derecha visual)
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, init);

    shiftGrid(g, DIRECTION_RIGHT);

    bool expected[] = {
        0, 1, 0,
        0, 0, 1,
        0, 0, 0
    };
    assert_cells_equal(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_up_moves_content_down() {
    // DIRECTION_UP => dy=1
    // buffer[y,x] = old[y+1, x] -> "sube" el origen, visualmente baja el contenido (pierde fila 0 y mete false abajo)
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, init);

    shiftGrid(g, DIRECTION_UP);

    bool expected[] = {
        0, 1, 0,
        0, 0, 1,
        0, 0, 0
    };
    assert_cells_equal(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_down_moves_content_up() {
    // DIRECTION_DOWN => dy=-1
    // buffer[y,x] = old[y-1, x] -> mete false arriba, mueve contenido hacia arriba visualmente
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, init);

    shiftGrid(g, DIRECTION_DOWN);

    bool expected[] = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0
    };
    assert_cells_equal(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_none_is_noop() {
    bool init[] = {
        1, 0, 1, 0
    };
    grid_t g = makeGrid(1, 4, init);

    shiftGrid(g, DIRECTION_NONE);

    assert_cells_equal(&g, init);

    freeGrid(g);
}

static void test_shiftGrid_1x1_all_directions_result_false_or_same() {
    bool init[] = { 1 };
    grid_t g;

    // UP -> newY = y+1 out of bounds => false
    g = makeGrid(1, 1, init);
    shiftGrid(g, DIRECTION_UP);
    ASSERT_FALSE(g.cells[0]);
    freeGrid(g);

    // DOWN -> newY = y-1 out of bounds => false
    g = makeGrid(1, 1, init);
    shiftGrid(g, DIRECTION_DOWN);
    ASSERT_FALSE(g.cells[0]);
    freeGrid(g);

    // LEFT -> newX = x+1 out of bounds => false
    g = makeGrid(1, 1, init);
    shiftGrid(g, DIRECTION_LEFT);
    ASSERT_FALSE(g.cells[0]);
    freeGrid(g);

    // RIGHT -> newX = x-1 out of bounds => false
    g = makeGrid(1, 1, init);
    shiftGrid(g, DIRECTION_RIGHT);
    ASSERT_FALSE(g.cells[0]);
    freeGrid(g);

    // NONE -> debería quedar igual
    g = makeGrid(1, 1, init);
    shiftGrid(g, DIRECTION_NONE);
    ASSERT_TRUE(g.cells[0]);
    freeGrid(g);
}

int main(void) {
    test_initializeGrid_sets_zero_and_null();
    test_makeGrid_copies_buffer_not_alias();

    test_shiftGrid_left_moves_content_right();
    test_shiftGrid_right_moves_content_left();
    test_shiftGrid_up_moves_content_down();
    test_shiftGrid_down_moves_content_up();

    test_shiftGrid_none_is_noop();
    test_shiftGrid_1x1_all_directions_result_false_or_same();

    return 0;
}

