// File: src/parameters/parameters_test.c

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "parameters.h"

#define ASSERT_TRUE(x)  assert((x) && "ASSERT_TRUE failed: " #x)
#define ASSERT_FALSE(x) assert(!(x) && "ASSERT_FALSE failed: " #x)
#define ASSERT_EQ_INT(a,b) assert(((a)==(b)) && "ASSERT_EQ_INT failed")
#define ASSERT_EQ_BOOL(a,b) assert(((a)==(b)) && "ASSERT_EQ_BOOL failed")
#define ASSERT_STREQ(a,b) assert((strcmp((a),(b))==0) && "ASSERT_STREQ failed")

static parameters_t params_null(void) {
    return getNullParameters();
}

static parameters_t params_only_rows(int rows) {
    parameters_t p = getNullParameters();
    p.rows = rows;
    p.rowsDefined = true;
    return p;
}

static parameters_t params_only_cols(int cols) {
    parameters_t p = getNullParameters();
    p.cols = cols;
    p.colsDefined = true;
    return p;
}

static parameters_t params_only_blockSize(int bs) {
    parameters_t p = getNullParameters();
    p.blockSize = bs;
    p.blockSizeDefined = true;
    return p;
}

static parameters_t params_only_fps(int fps) {
    parameters_t p = getNullParameters();
    p.fps = fps;
    p.fpsDefined = true;
    return p;
}

static parameters_t params_only_delay(int delay) {
    parameters_t p = getNullParameters();
    p.delay = delay;
    p.delayDefined = true;
    return p;
}

static parameters_t params_only_fullscreen(bool fs) {
    parameters_t p = getNullParameters();
    p.fullscreen = fs;
    p.fullscreenDefined = true;
    return p;
}

// ---- New helper: "base defaults are always fully-defined" contract ----
static void assert_base_defaults_fully_defined(parameters_t p) {
    // El contrato fuerte: los "base defaults" tienen TODO definido.
    ASSERT_TRUE(p.rowsDefined);
    ASSERT_TRUE(p.colsDefined);
    ASSERT_TRUE(p.blockSizeDefined);
    ASSERT_TRUE(p.fpsDefined);
    ASSERT_TRUE(p.delayDefined);
    ASSERT_TRUE(p.fullscreenDefined);

    // Y además: areAllParametersSet debe devolver true (y no tocar buffer si existe)
    char buf[64];
    memset(buf, 0xAB, sizeof(buf));
    ASSERT_TRUE(areAllParametersSet(p, buf));
    // Si devolvió true, no debería haber escrito un nombre de parámetro faltante.
    // (No es un contrato explícito de la función, pero es un buen "canary".)
    ASSERT_TRUE(buf[0] == (char)0xAB);
}

static void test_getNullParameters_all_undefined() {
    parameters_t p = getNullParameters();

    ASSERT_FALSE(p.rowsDefined);
    ASSERT_FALSE(p.colsDefined);
    ASSERT_FALSE(p.blockSizeDefined);
    ASSERT_FALSE(p.fpsDefined);
    ASSERT_FALSE(p.delayDefined);
    ASSERT_FALSE(p.fullscreenDefined);
}

// ---- Base defaults: full contract + values ----
static void test_getBaseDefaultParameters_all_defined_and_valid() {
    parameters_t p = getBaseDefaultParameters();

    // Contrato: TODO definido siempre.
    assert_base_defaults_fully_defined(p);

    // Valores hardcodeados en el .c
    ASSERT_EQ_INT(p.rows, 70);
    ASSERT_EQ_INT(p.cols, 100);
    ASSERT_EQ_INT(p.blockSize, 17);
    ASSERT_EQ_INT(p.fps, 30);
    ASSERT_EQ_INT(p.delay, 100);
    ASSERT_EQ_BOOL(p.fullscreen, false);

    // Invariantes mínimas razonables
    ASSERT_TRUE(p.rows > 0);
    ASSERT_TRUE(p.cols > 0);
    ASSERT_TRUE(p.blockSize > 0);
    ASSERT_TRUE(p.fps > 0);
    ASSERT_TRUE(p.delay >= 0);
}

static void test_getBaseDefaultParameters_is_stable_across_calls() {
    parameters_t a = getBaseDefaultParameters();
    parameters_t b = getBaseDefaultParameters();

    // Si esto falla, alguien está metiendo estado global raro.
    ASSERT_EQ_INT(a.rows, b.rows);
    ASSERT_EQ_INT(a.cols, b.cols);
    ASSERT_EQ_INT(a.blockSize, b.blockSize);
    ASSERT_EQ_INT(a.fps, b.fps);
    ASSERT_EQ_INT(a.delay, b.delay);
    ASSERT_EQ_BOOL(a.fullscreen, b.fullscreen);

    ASSERT_EQ_BOOL(a.rowsDefined, b.rowsDefined);
    ASSERT_EQ_BOOL(a.colsDefined, b.colsDefined);
    ASSERT_EQ_BOOL(a.blockSizeDefined, b.blockSizeDefined);
    ASSERT_EQ_BOOL(a.fpsDefined, b.fpsDefined);
    ASSERT_EQ_BOOL(a.delayDefined, b.delayDefined);
    ASSERT_EQ_BOOL(a.fullscreenDefined, b.fullscreenDefined);
}

static void test_getSensibleDefaultParameters_sets_defined_and_calculates() {
    int screenW = 1920;
    int screenH = 1080;

    // blockSize = 1080 / 85 = 12 (int)
    // rows = (1080/12) * 8/10 = 90*8/10 = 72
    // cols = (1920/12) * 8/10 = 160*8/10 = 128
    parameters_t p = getSensibleDefaultParameters(screenW, screenH);

    ASSERT_TRUE(p.rowsDefined);
    ASSERT_TRUE(p.colsDefined);
    ASSERT_TRUE(p.blockSizeDefined);
    ASSERT_TRUE(p.fpsDefined);
    ASSERT_TRUE(p.delayDefined);
    ASSERT_TRUE(p.fullscreenDefined);

    ASSERT_EQ_INT(p.blockSize, 12);
    ASSERT_EQ_INT(p.rows, 72);
    ASSERT_EQ_INT(p.cols, 128);

    // Los defaults restantes
    ASSERT_EQ_INT(p.fps, 30);
    ASSERT_EQ_INT(p.delay, 100);
    ASSERT_EQ_BOOL(p.fullscreen, false);

    // Invariantes mínimas
    ASSERT_TRUE(p.rows > 0);
    ASSERT_TRUE(p.cols > 0);
    ASSERT_TRUE(p.blockSize > 0);
}

static void test_areAllParametersSet_false_reports_first_missing_in_order() {
    char buf[64];

    parameters_t p = params_null();
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "cols"); // el primero que chequea

    p = params_only_cols(10);
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "rows");

    p = params_only_cols(10);
    p.rows = 20; p.rowsDefined = true;
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "blockSize");

    p.blockSize = 3; p.blockSizeDefined = true;
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "fps");

    p.fps = 60; p.fpsDefined = true;
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "delay");

    p.delay = 5; p.delayDefined = true;
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "fullscreen");

    p.fullscreen = true; p.fullscreenDefined = true;
    ASSERT_TRUE(areAllParametersSet(p, buf));
}

static void test_areAllParametersSet_true_accepts_null_buffer() {
    parameters_t p = getBaseDefaultParameters();
    ASSERT_TRUE(areAllParametersSet(p, NULL));
}

static void test_solveParameters_precedence_options_over_config_over_default_ints() {
    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def); // <- reforzamos el contrato acá también

    parameters_t cfg = params_null();
    parameters_t opt = params_null();

    // rows: solo default
    parameters_t r = solveParameters(def, cfg, opt);
    ASSERT_EQ_INT(r.rows, def.rows);
    ASSERT_TRUE(r.rowsDefined);

    // rows: config pisa default
    cfg = params_only_rows(111);
    r = solveParameters(def, cfg, opt);
    ASSERT_EQ_INT(r.rows, 111);

    // rows: options pisa config
    opt = params_only_rows(222);
    r = solveParameters(def, cfg, opt);
    ASSERT_EQ_INT(r.rows, 222);

    // cols: probamos lo mismo
    cfg = params_only_cols(333);
    opt = params_null();
    r = solveParameters(def, cfg, opt);
    ASSERT_EQ_INT(r.cols, 333);

    opt = params_only_cols(444);
    r = solveParameters(def, cfg, opt);
    ASSERT_EQ_INT(r.cols, 444);
}

static void test_solveParameters_precedence_bool() {
    parameters_t def = getBaseDefaultParameters(); // fullscreen default false
    assert_base_defaults_fully_defined(def);

    parameters_t cfg = params_null();
    parameters_t opt = params_null();

    // config true -> true
    cfg = params_only_fullscreen(true);
    parameters_t r = solveParameters(def, cfg, opt);
    ASSERT_TRUE(r.fullscreenDefined);
    ASSERT_EQ_BOOL(r.fullscreen, true);

    // options false -> false (pisa config true)
    opt = params_only_fullscreen(false);
    r = solveParameters(def, cfg, opt);
    ASSERT_TRUE(r.fullscreenDefined);
    ASSERT_EQ_BOOL(r.fullscreen, false);
}

static void test_solveParameters_fieldwise_independence() {
    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def);

    // cfg solo toca fps, opt solo toca delay. Lo demás debe venir de default.
    parameters_t cfg = params_only_fps(144);
    parameters_t opt = params_only_delay(7);

    parameters_t r = solveParameters(def, cfg, opt);

    ASSERT_EQ_INT(r.fps, 144);
    ASSERT_TRUE(r.fpsDefined);

    ASSERT_EQ_INT(r.delay, 7);
    ASSERT_TRUE(r.delayDefined);

    ASSERT_EQ_INT(r.rows, def.rows);
    ASSERT_EQ_INT(r.cols, def.cols);
    ASSERT_EQ_INT(r.blockSize, def.blockSize);
    ASSERT_EQ_BOOL(r.fullscreen, def.fullscreen);

    ASSERT_TRUE(r.rowsDefined);
    ASSERT_TRUE(r.colsDefined);
    ASSERT_TRUE(r.blockSizeDefined);
    ASSERT_TRUE(r.fullscreenDefined);
}

static void test_solveParameters_can_leave_fields_undefined_if_all_sources_undefined() {
    // Si todas las fuentes son null, todo queda undefined y 0
    parameters_t z = params_null();
    parameters_t r = solveParameters(z, z, z);

    ASSERT_FALSE(r.rowsDefined);       ASSERT_EQ_INT(r.rows, 0);
    ASSERT_FALSE(r.colsDefined);       ASSERT_EQ_INT(r.cols, 0);
    ASSERT_FALSE(r.blockSizeDefined);  ASSERT_EQ_INT(r.blockSize, 0);
    ASSERT_FALSE(r.fpsDefined);        ASSERT_EQ_INT(r.fps, 0);
    ASSERT_FALSE(r.delayDefined);      ASSERT_EQ_INT(r.delay, 0);
    ASSERT_FALSE(r.fullscreenDefined); ASSERT_EQ_BOOL(r.fullscreen, false); // bool 0
}

int main(void) {
    test_getNullParameters_all_undefined();

    test_getBaseDefaultParameters_all_defined_and_valid();
    test_getBaseDefaultParameters_is_stable_across_calls();

    test_getSensibleDefaultParameters_sets_defined_and_calculates();

    test_areAllParametersSet_false_reports_first_missing_in_order();
    test_areAllParametersSet_true_accepts_null_buffer();

    test_solveParameters_precedence_options_over_config_over_default_ints();
    test_solveParameters_precedence_bool();
    test_solveParameters_fieldwise_independence();
    test_solveParameters_can_leave_fields_undefined_if_all_sources_undefined();

    return 0;
}

