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

static void assert_cells_equal_bool(const grid_t* g, const bool* expected) {
    ASSERT_EQ_INT((int)g->elem_size, (int)sizeof(bool));
    int n = g->rows * g->cols;
    const bool* cells = (const bool*)g->cells;
    for (int i = 0; i < n; i++) {
        ASSERT_EQ_BOOL(cells[i], expected[i]);
    }
}

static void test_initializeGrid_sets_zero_and_null() {
    grid_t g;

    g.rows = 123;
    g.cols = 456;
    g.elem_size = 99;
    g.cells = (void*)0x1;

    initializeGrid(&g);

    ASSERT_EQ_INT(g.rows, 0);
    ASSERT_EQ_INT(g.cols, 0);
    ASSERT_EQ_INT((int)g.elem_size, 0);
    ASSERT_TRUE(g.cells == NULL);
}

static void test_makeGrid_copies_buffer_not_alias_bool() {
    bool init[] = {
        1, 0, 1,
        0, 1, 0
    };

    grid_t g = makeGrid(2, 3, sizeof(bool), init);

    ASSERT_EQ_INT(g.rows, 2);
    ASSERT_EQ_INT(g.cols, 3);
    ASSERT_EQ_INT((int)g.elem_size, (int)sizeof(bool));
    ASSERT_TRUE(g.cells != NULL);

    assert_cells_equal_bool(&g, init);

    init[0] = 0;
    ASSERT_TRUE(((bool*)g.cells)[0] == 1);

    freeGrid(g);
}

static void test_makeGrid_null_init_zeroes_memory_bool() {
    grid_t g = makeGrid(2, 2, sizeof(bool), NULL);

    ASSERT_EQ_INT(g.rows, 2);
    ASSERT_EQ_INT(g.cols, 2);
    ASSERT_EQ_INT((int)g.elem_size, (int)sizeof(bool));
    ASSERT_TRUE(g.cells != NULL);

    bool expected[] = { 0, 0, 0, 0 };
    assert_cells_equal_bool(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_left_dx1_bool() {
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, sizeof(bool), init);

    shiftGrid(g, DIRECTION_LEFT);

    bool expected[] = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0
    };
    assert_cells_equal_bool(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_right_dx_minus1_bool() {
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, sizeof(bool), init);

    shiftGrid(g, DIRECTION_RIGHT);

    bool expected[] = {
        0, 1, 0,
        0, 0, 1,
        0, 0, 0
    };
    assert_cells_equal_bool(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_up_dy1_bool() {
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, sizeof(bool), init);

    shiftGrid(g, DIRECTION_UP);

    bool expected[] = {
        0, 1, 0,
        0, 0, 1,
        0, 0, 0
    };
    assert_cells_equal_bool(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_down_dy_minus1_bool() {
    bool init[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    grid_t g = makeGrid(3, 3, sizeof(bool), init);

    shiftGrid(g, DIRECTION_DOWN);

    bool expected[] = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0
    };
    assert_cells_equal_bool(&g, expected);

    freeGrid(g);
}

static void test_shiftGrid_none_is_noop_bool() {
    bool init[] = { 1, 0, 1, 0 };
    grid_t g = makeGrid(1, 4, sizeof(bool), init);

    shiftGrid(g, DIRECTION_NONE);

    assert_cells_equal_bool(&g, init);

    freeGrid(g);
}

static void test_shiftGrid_1x1_edges_bool() {
    bool init[] = { 1 };
    grid_t g;

    g = makeGrid(1, 1, sizeof(bool), init);
    shiftGrid(g, DIRECTION_UP);
    ASSERT_FALSE(((bool*)g.cells)[0]);
    freeGrid(g);

    g = makeGrid(1, 1, sizeof(bool), init);
    shiftGrid(g, DIRECTION_DOWN);
    ASSERT_FALSE(((bool*)g.cells)[0]);
    freeGrid(g);

    g = makeGrid(1, 1, sizeof(bool), init);
    shiftGrid(g, DIRECTION_LEFT);
    ASSERT_FALSE(((bool*)g.cells)[0]);
    freeGrid(g);

    g = makeGrid(1, 1, sizeof(bool), init);
    shiftGrid(g, DIRECTION_RIGHT);
    ASSERT_FALSE(((bool*)g.cells)[0]);
    freeGrid(g);

    g = makeGrid(1, 1, sizeof(bool), init);
    shiftGrid(g, DIRECTION_NONE);
    ASSERT_TRUE(((bool*)g.cells)[0]);
    freeGrid(g);
}

int main(void) {
    test_initializeGrid_sets_zero_and_null();

    test_makeGrid_copies_buffer_not_alias_bool();
    test_makeGrid_null_init_zeroes_memory_bool();

    test_shiftGrid_left_dx1_bool();
    test_shiftGrid_right_dx_minus1_bool();
    test_shiftGrid_up_dy1_bool();
    test_shiftGrid_down_dy_minus1_bool();

    test_shiftGrid_none_is_noop_bool();
    test_shiftGrid_1x1_edges_bool();

    return 0;
}

