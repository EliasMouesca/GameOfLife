#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "options.h"
#include "../parameters/parameters.h"

#define ASSERT_TRUE(x)  assert((x) && "ASSERT_TRUE failed: " #x)
#define ASSERT_FALSE(x) assert(!(x) && "ASSERT_FALSE failed: " #x)
#define ASSERT_EQ_INT(a,b) assert(((a)==(b)) && "ASSERT_EQ_INT failed")
#define ASSERT_EQ_BOOL(a,b) assert(((a)==(b)) && "ASSERT_EQ_BOOL failed")

static void test_no_args_returns_all_undefined() {
    char* argv[] = { (char*)"prog" };
    parameters_t p = optionsToParameters(1, argv);

    ASSERT_FALSE(p.rowsDefined);
    ASSERT_FALSE(p.colsDefined);
    ASSERT_FALSE(p.blockSizeDefined);
    ASSERT_FALSE(p.fpsDefined);
    ASSERT_FALSE(p.delayDefined);
    ASSERT_FALSE(p.fullscreenDefined);
}

static void test_short_int_options_set_fields() {
    char* argv[] = {
        (char*)"prog",
        (char*)"-r", (char*)"80",
        (char*)"-c", (char*)"120",
        (char*)"-b", (char*)"9",
        (char*)"-d", (char*)"50",
    };
    parameters_t p = optionsToParameters((int)(sizeof(argv)/sizeof(argv[0])), argv);

    ASSERT_TRUE(p.rowsDefined);      ASSERT_EQ_INT(p.rows, 80);
    ASSERT_TRUE(p.colsDefined);      ASSERT_EQ_INT(p.cols, 120);
    ASSERT_TRUE(p.blockSizeDefined); ASSERT_EQ_INT(p.blockSize, 9);
    ASSERT_TRUE(p.delayDefined);     ASSERT_EQ_INT(p.delay, 50);

    ASSERT_FALSE(p.fpsDefined);
    ASSERT_FALSE(p.fullscreenDefined);
}

static void test_long_fps_option_sets_fps() {
    char* argv[] = {
        (char*)"prog",
        (char*)"--fps", (char*)"144",
    };
    parameters_t p = optionsToParameters((int)(sizeof(argv)/sizeof(argv[0])), argv);

    ASSERT_TRUE(p.fpsDefined);
    ASSERT_EQ_INT(p.fps, 144);

    ASSERT_FALSE(p.rowsDefined);
    ASSERT_FALSE(p.colsDefined);
    ASSERT_FALSE(p.blockSizeDefined);
    ASSERT_FALSE(p.delayDefined);
    ASSERT_FALSE(p.fullscreenDefined);
}

static void test_fullscreen_flag_sets_fullscreen_true() {
    // Esto va a FALLAR con tu implementación actual (porque no setea params.fullscreen)
    // Arreglalo como te puse arriba y pasa.
    char* argv[] = { (char*)"prog", (char*)"-f" };
    parameters_t p = optionsToParameters((int)(sizeof(argv)/sizeof(argv[0])), argv);

    ASSERT_TRUE(p.fullscreenDefined);
    ASSERT_EQ_BOOL(p.fullscreen, true);
}

static void test_multiple_options_can_mix_short_and_long() {
    char* argv[] = {
        (char*)"prog",
        (char*)"--fps", (char*)"60",
        (char*)"-r", (char*)"70",
        (char*)"-c", (char*)"100",
        (char*)"-b", (char*)"17",
        (char*)"-d", (char*)"100",
        (char*)"-f"
    };
    parameters_t p = optionsToParameters((int)(sizeof(argv)/sizeof(argv[0])), argv);

    ASSERT_TRUE(p.fpsDefined);       ASSERT_EQ_INT(p.fps, 60);
    ASSERT_TRUE(p.rowsDefined);      ASSERT_EQ_INT(p.rows, 70);
    ASSERT_TRUE(p.colsDefined);      ASSERT_EQ_INT(p.cols, 100);
    ASSERT_TRUE(p.blockSizeDefined); ASSERT_EQ_INT(p.blockSize, 17);
    ASSERT_TRUE(p.delayDefined);     ASSERT_EQ_INT(p.delay, 100);
    ASSERT_TRUE(p.fullscreenDefined);ASSERT_EQ_BOOL(p.fullscreen, true);
}

static void test_optind_reset_makes_function_reentrant() {
    // Llamada 1
    char* argv1[] = { (char*)"prog", (char*)"-r", (char*)"10" };
    parameters_t p1 = optionsToParameters((int)(sizeof(argv1)/sizeof(argv1[0])), argv1);
    ASSERT_TRUE(p1.rowsDefined);
    ASSERT_EQ_INT(p1.rows, 10);

    // Llamada 2 (si optind no se resetea bien, esto se rompe)
    char* argv2[] = { (char*)"prog", (char*)"-c", (char*)"20" };
    parameters_t p2 = optionsToParameters((int)(sizeof(argv2)/sizeof(argv2[0])), argv2);
    ASSERT_TRUE(p2.colsDefined);
    ASSERT_EQ_INT(p2.cols, 20);
}

int main(void) {
    test_no_args_returns_all_undefined();
    test_short_int_options_set_fields();
    test_long_fps_option_sets_fps();
    test_fullscreen_flag_sets_fullscreen_true();
    test_multiple_options_can_mix_short_and_long();
    test_optind_reset_makes_function_reentrant();
    return 0;
}

