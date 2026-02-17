// File: src/example/example_test.c

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "example.h"

#define ASSERT_TRUE(x)  assert((x) && "ASSERT_TRUE failed: " #x)
#define ASSERT_FALSE(x) assert(!(x) && "ASSERT_FALSE failed: " #x)
#define ASSERT_EQ_INT(a,b) assert(((a)==(b)) && "ASSERT_EQ_INT failed")
#define ASSERT_EQ_BOOL(a,b) assert(((a)==(b)) && "ASSERT_EQ_BOOL failed")

static int count_alive(const example_t* e) {
    int n = 0;
    for (int i = 0; i < e->rows * e->cols; i++) n += e->cells[i] ? 1 : 0;
    return n;
}

static void assert_dims_and_pos(example_t e, int rows, int cols, horizontalPosition_t h, verticalPosition_t v) {
    ASSERT_EQ_INT(e.rows, rows);
    ASSERT_EQ_INT(e.cols, cols);
    ASSERT_EQ_INT((int)e.hpos, (int)h);
    ASSERT_EQ_INT((int)e.vpos, (int)v);
}

static void assert_cells_equal_flat(example_t e, const bool* expected_flat) {
    for (int i = 0; i < e.rows * e.cols; i++) {
        ASSERT_EQ_BOOL(e.cells[i], expected_flat[i]);
    }
}

static void test_destroyExample_nulls_cells() {
    example_t e = chaos();
    ASSERT_TRUE(e.cells != NULL);

    destroyExample(&e);
    ASSERT_TRUE(e.cells == NULL);
}

static void test_nothing_example_is_well_formed() {
    example_t e = nothing();
    assert_dims_and_pos(e, 0, 0, LEFT, TOP);
    ASSERT_TRUE(e.cells == NULL);

    destroyExample(&e); // free(NULL) ok
    ASSERT_TRUE(e.cells == NULL);
}

static void test_chaos_example_shape_and_data() {
    example_t e = chaos();
    assert_dims_and_pos(e, 3, 3, CENTER, MIDDLE);
    ASSERT_TRUE(e.cells != NULL);

    const bool expected[] = {
        1, 1, 0,
        0, 1, 1,
        0, 1, 0
    };
    assert_cells_equal_flat(e, expected);
    ASSERT_EQ_INT(count_alive(&e), 5);

    destroyExample(&e);
    ASSERT_TRUE(e.cells == NULL);
}

static void test_diehard_example_shape_and_data() {
    example_t e = diehard();
    assert_dims_and_pos(e, 3, 8, CENTER, MIDDLE);
    ASSERT_TRUE(e.cells != NULL);

    const bool expected[] = {
        0, 0, 0, 0, 0, 0, 1, 0,
        1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 1, 1, 1
    };
    assert_cells_equal_flat(e, expected);
    ASSERT_EQ_INT(count_alive(&e), 7);

    destroyExample(&e);
    ASSERT_TRUE(e.cells == NULL);
}

static void test_glider_example_shape_and_data() {
    example_t e = glider();
    assert_dims_and_pos(e, 3, 3, LEFT, TOP);
    ASSERT_TRUE(e.cells != NULL);

    const bool expected[] = {
        0, 1, 0,
        0, 0, 1,
        1, 1, 1
    };
    assert_cells_equal_flat(e, expected);
    ASSERT_EQ_INT(count_alive(&e), 5);

    destroyExample(&e);
    ASSERT_TRUE(e.cells == NULL);
}

static void test_acorn_example_shape_and_data() {
    example_t e = acorn();
    assert_dims_and_pos(e, 3, 7, CENTER, MIDDLE);
    ASSERT_TRUE(e.cells != NULL);

    const bool expected[] = {
        0, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0,
        1, 1, 0, 0, 1, 1, 1
    };
    assert_cells_equal_flat(e, expected);
    ASSERT_EQ_INT(count_alive(&e), 7);

    destroyExample(&e);
    ASSERT_TRUE(e.cells == NULL);
}

static void test_galaxy_example_shape_and_data() {
    example_t e = galaxy();
    assert_dims_and_pos(e, 9, 9, CENTER, MIDDLE);
    ASSERT_TRUE(e.cells != NULL);

    const bool expected[] = {
        1,1,1,1,1,1,0,1,1,
        1,1,1,1,1,1,0,1,1,
        0,0,0,0,0,0,0,1,1,
        1,1,0,0,0,0,0,1,1,
        1,1,0,0,0,0,0,1,1,
        1,1,0,0,0,0,0,1,1,
        1,1,0,0,0,0,0,0,0,
        1,1,0,1,1,1,1,1,1,
        1,1,0,1,1,1,1,1,1
    };
    assert_cells_equal_flat(e, expected);
    ASSERT_EQ_INT(count_alive(&e), 48);

    destroyExample(&e);
    ASSERT_TRUE(e.cells == NULL);
}

int main(void) {
    test_destroyExample_nulls_cells();
    test_nothing_example_is_well_formed();
    test_chaos_example_shape_and_data();
    test_diehard_example_shape_and_data();
    test_glider_example_shape_and_data();
    test_acorn_example_shape_and_data();
    test_galaxy_example_shape_and_data();
    return 0;
}

