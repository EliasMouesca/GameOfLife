#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "grid.h"
#include "game.h"

bool runTest(grid_t input, grid_t expected) {
    if (input.rows != expected.rows || input.cols != expected.cols) return false;

    bool* input_copy = malloc(sizeof(bool) * input.rows * input.cols);
    memcpy(input_copy, input.cells, sizeof(bool) * input.rows * input.cols);

    grid_t test_grid = { input_copy, input.rows, input.cols };
    update(test_grid);

    bool result = true;
    for (int r = 0; r < input.rows; r++)
        for (int c = 0; c < input.cols; c++)
            if (input_copy[r * input.cols + c] != expected.cells[r * input.cols + c])
                result = false;

    free(input_copy);
    return result;
}

void printResult(const char* name, bool result) {
    printf("%s: %s\n", name, result ? "OK" : "FAILED");
}

int main() {
    bool ok;

    // ---- Test 1: Blinker (oscillator)
    {
        bool input_arr[5][5] = {
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,0,0,0}
        };
        bool expected_arr[5][5] = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,1,1,1,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
        };

        grid_t input = { (bool*)input_arr, 5, 5 };
        grid_t expected = { (bool*)expected_arr, 5, 5 };

        ok = runTest(input, expected);
        printResult("Blinker → horizontal", ok);
    }

    // ---- Test 2: Block (still life)
    {
        bool input_arr[4][4] = {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        };
        bool expected_arr[4][4] = {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        };

        grid_t input = { (bool*)input_arr, 4, 4 };
        grid_t expected = { (bool*)expected_arr, 4, 4 };

        ok = runTest(input, expected);
        printResult("Block → unchanged", ok);
    }

    // ---- Test 3: Boat (still life)
    {
        bool input_arr[5][5] = {
            {0,0,0,0,0},
            {0,1,1,0,0},
            {0,1,0,1,0},
            {0,0,1,0,0},
            {0,0,0,0,0}
        };
        bool expected_arr[5][5] = {
            {0,0,0,0,0},
            {0,1,1,0,0},
            {0,1,0,1,0},
            {0,0,1,0,0},
            {0,0,0,0,0}
        };

        grid_t input = { (bool*)input_arr, 5, 5 };
        grid_t expected = { (bool*)expected_arr, 5, 5 };

        ok = runTest(input, expected);
        printResult("Boat → unchanged", ok);
    }

    // ---- Test 4: Toad (oscillator)
    {
        bool input_arr[6][6] = {
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {0,0,1,1,1,0},
            {0,1,1,1,0,0},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0}
        };
        bool expected_arr[6][6] = {
            {0,0,0,0,0,0},
            {0,0,0,1,0,0},
            {0,1,0,0,1,0},
            {0,1,0,0,1,0},
            {0,0,1,0,0,0},
            {0,0,0,0,0,0}
        };

        grid_t input = { (bool*)input_arr, 6, 6 };
        grid_t expected = { (bool*)expected_arr, 6, 6 };

        ok = runTest(input, expected);
        printResult("Toad → phase 2", ok);
    }

    // ---- Test 5: Empty grid
    {
        bool input_arr[3][3] = {
            {0,0,0},
            {0,0,0},
            {0,0,0}
        };
        bool expected_arr[3][3] = {
            {0,0,0},
            {0,0,0},
            {0,0,0}
        };

        grid_t input = { (bool*)input_arr, 3, 3 };
        grid_t expected = { (bool*)expected_arr, 3, 3 };

        ok = runTest(input, expected);
        printResult("Empty grid → unchanged", ok);
    }

    // ---- Test 6: Glider (spaceship)
    {
        bool input_arr[5][5] = {
            {0,0,1,0,0},
            {1,0,1,0,0},
            {0,1,1,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
        };
        bool expected_arr[5][5] = {
            {0,1,0,0,0},
            {0,0,1,1,0},
            {0,1,1,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
        };

        grid_t input = { (bool*)input_arr, 5, 5 };
        grid_t expected = { (bool*)expected_arr, 5, 5 };

        ok = runTest(input, expected);
        printResult("Glider → 1 step", ok);
    }

    // ---- Test 7: Blinker at edge
    {
        bool input_arr[5][5] = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {1,1,1,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
        };
        bool expected_arr[5][5] = {
            {0,0,0,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0},
            {0,1,0,0,0},
            {0,0,0,0,0}
        };

        grid_t input = { (bool*)input_arr, 5, 5 };
        grid_t expected = { (bool*)expected_arr, 5, 5 };

        ok = runTest(input, expected);
        printResult("Blinker → vertical at edge", ok);
    }

    return 0;
}

