// File: src/parameters/parameters_test.c

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "parameters.h"

#define ASSERT_TRUE(x)        assert((x) && "ASSERT_TRUE failed: " #x)
#define ASSERT_FALSE(x)       assert(!(x) && "ASSERT_FALSE failed: " #x)
#define ASSERT_EQ_INT(a,b)    assert(((a)==(b)) && "ASSERT_EQ_INT failed")
#define ASSERT_EQ_BOOL(a,b)   assert(((a)==(b)) && "ASSERT_EQ_BOOL failed")
#define ASSERT_STREQ(a,b)     assert((strcmp((a),(b))==0) && "ASSERT_STREQ failed")

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

static parameters_t params_only_party(bool party) {
    parameters_t p = getNullParameters();
    p.party = party;
    p.partyDefined = true;
    return p;
}

static void assert_base_defaults_fully_defined(parameters_t p) {
    ASSERT_TRUE(p.rowsDefined);
    ASSERT_TRUE(p.colsDefined);
    ASSERT_TRUE(p.blockSizeDefined);
    ASSERT_TRUE(p.fpsDefined);
    ASSERT_TRUE(p.delayDefined);
    ASSERT_TRUE(p.fullscreenDefined);
    ASSERT_TRUE(p.partyDefined);

    char buf[64];
    memset(buf, 0xAB, sizeof(buf));
    ASSERT_TRUE(areAllParametersSet(p, buf));
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
    ASSERT_FALSE(p.partyDefined);
}

static void test_getBaseDefaultParameters_all_defined_and_valid() {
    parameters_t p = getBaseDefaultParameters();

    assert_base_defaults_fully_defined(p);

    ASSERT_EQ_INT(p.rows, 70);
    ASSERT_EQ_INT(p.cols, 100);
    ASSERT_EQ_INT(p.blockSize, 17);
    ASSERT_EQ_INT(p.fps, 30);
    ASSERT_EQ_INT(p.delay, 100);
    ASSERT_EQ_BOOL(p.fullscreen, false);
    ASSERT_EQ_BOOL(p.party, false);

    ASSERT_TRUE(p.rows > 0);
    ASSERT_TRUE(p.cols > 0);
    ASSERT_TRUE(p.blockSize > 0);
    ASSERT_TRUE(p.fps > 0);
    ASSERT_TRUE(p.delay >= 0);
}

static void test_getBaseDefaultParameters_is_stable_across_calls() {
    parameters_t a = getBaseDefaultParameters();
    parameters_t b = getBaseDefaultParameters();

    ASSERT_EQ_INT(a.rows, b.rows);
    ASSERT_EQ_INT(a.cols, b.cols);
    ASSERT_EQ_INT(a.blockSize, b.blockSize);
    ASSERT_EQ_INT(a.fps, b.fps);
    ASSERT_EQ_INT(a.delay, b.delay);
    ASSERT_EQ_BOOL(a.fullscreen, b.fullscreen);
    ASSERT_EQ_BOOL(a.party, b.party);

    ASSERT_EQ_BOOL(a.rowsDefined, b.rowsDefined);
    ASSERT_EQ_BOOL(a.colsDefined, b.colsDefined);
    ASSERT_EQ_BOOL(a.blockSizeDefined, b.blockSizeDefined);
    ASSERT_EQ_BOOL(a.fpsDefined, b.fpsDefined);
    ASSERT_EQ_BOOL(a.delayDefined, b.delayDefined);
    ASSERT_EQ_BOOL(a.fullscreenDefined, b.fullscreenDefined);
    ASSERT_EQ_BOOL(a.partyDefined, b.partyDefined);
}

static void test_getSensibleDefaultParameters_sets_defined_and_calculates() {
    int screenW = 1920;
    int screenH = 1080;

    parameters_t p = getSensibleDefaultParameters(screenW, screenH);

    ASSERT_TRUE(p.rowsDefined);
    ASSERT_TRUE(p.colsDefined);
    ASSERT_TRUE(p.blockSizeDefined);
    ASSERT_TRUE(p.fpsDefined);
    ASSERT_TRUE(p.delayDefined);
    ASSERT_TRUE(p.fullscreenDefined);
    ASSERT_TRUE(p.partyDefined);

    // screenH / 85 = 1080 / 85 = 12 (int)
    ASSERT_EQ_INT(p.blockSize, 12);

    // rows = (screenH / blockSize) * 8 / 10 = (1080/12)*0.8 = 90*0.8 = 72
    ASSERT_EQ_INT(p.rows, 72);

    // cols = (screenW / blockSize) * 8 / 10 = (1920/12)*0.8 = 160*0.8 = 128
    ASSERT_EQ_INT(p.cols, 128);

    ASSERT_EQ_INT(p.fps, 30);
    ASSERT_EQ_INT(p.delay, 100);
    ASSERT_EQ_BOOL(p.fullscreen, false);
    ASSERT_EQ_BOOL(p.party, false);

    ASSERT_TRUE(p.rows > 0);
    ASSERT_TRUE(p.cols > 0);
    ASSERT_TRUE(p.blockSize > 0);
}

static void test_areAllParametersSet_false_reports_first_missing_in_order() {
    char buf[64];

    parameters_t p = params_null();
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "cols");

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
    ASSERT_FALSE(areAllParametersSet(p, buf));
    ASSERT_STREQ(buf, "party");

    p.party = true; p.partyDefined = true;
    ASSERT_TRUE(areAllParametersSet(p, buf));
}

static void test_areAllParametersSet_true_accepts_null_buffer() {
    parameters_t p = getBaseDefaultParameters();
    ASSERT_TRUE(areAllParametersSet(p, NULL));
}

static void test_solveParameters_precedence_options_over_config_over_default_ints() {
    int w = 1920, h = 1080;

    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def);

    parameters_t cfg = params_null();
    parameters_t opt = params_null();

    parameters_t r = solveParameters(def, cfg, opt, w, h);
    ASSERT_TRUE(r.rowsDefined);
    ASSERT_EQ_INT(r.rows, def.rows);

    cfg = params_only_rows(111);
    r = solveParameters(def, cfg, opt, w, h);
    ASSERT_EQ_INT(r.rows, 111);

    opt = params_only_rows(222);
    r = solveParameters(def, cfg, opt, w, h);
    ASSERT_EQ_INT(r.rows, 222);

    cfg = params_only_cols(333);
    opt = params_null();
    r = solveParameters(def, cfg, opt, w, h);
    ASSERT_EQ_INT(r.cols, 333);

    opt = params_only_cols(444);
    r = solveParameters(def, cfg, opt, w, h);
    ASSERT_EQ_INT(r.cols, 444);
}

static void test_solveParameters_precedence_bool() {
    int w = 1920, h = 1080;

    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def);

    parameters_t cfg = params_only_fullscreen(true);
    parameters_t opt = params_null();

    parameters_t r = solveParameters(def, cfg, opt, w, h);
    ASSERT_TRUE(r.fullscreenDefined);
    ASSERT_EQ_BOOL(r.fullscreen, true);

    opt = params_only_fullscreen(false);
    r = solveParameters(def, cfg, opt, w, h);
    ASSERT_TRUE(r.fullscreenDefined);
    ASSERT_EQ_BOOL(r.fullscreen, false);
}

static void test_solveParameters_fieldwise_independence_non_fullscreen() {
    int w = 1920, h = 1080;

    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def);

    parameters_t cfg = params_only_fps(144);
    parameters_t opt = params_only_delay(7);

    parameters_t r = solveParameters(def, cfg, opt, w, h);

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

static void test_solveParameters_fullscreen_true_derives_rows_cols_from_window_and_blocksize() {
    int w = 1920, h = 1080;

    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def);

    parameters_t cfg = params_null();
    parameters_t opt = params_null();

    cfg = params_only_fullscreen(true);
    cfg.blockSize = 10; cfg.blockSizeDefined = true;

    parameters_t r = solveParameters(def, cfg, opt, w, h);

    ASSERT_TRUE(r.fullscreenDefined);
    ASSERT_TRUE(r.fullscreen);

    ASSERT_TRUE(r.blockSizeDefined);
    ASSERT_EQ_INT(r.blockSize, 10);

    ASSERT_TRUE(r.colsDefined);
    ASSERT_TRUE(r.rowsDefined);
    ASSERT_EQ_INT(r.cols, w / 10);
    ASSERT_EQ_INT(r.rows, h / 10);
}

static void test_solveParameters_fullscreen_true_ignores_rows_cols_sources() {
    int w = 1920, h = 1080;

    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def);

    parameters_t cfg = params_null();
    parameters_t opt = params_null();

    cfg = params_only_fullscreen(true);
    cfg.blockSize = 8; cfg.blockSizeDefined = true;

    cfg.rows = 1; cfg.rowsDefined = true;
    cfg.cols = 1; cfg.colsDefined = true;

    opt.rows = 2; opt.rowsDefined = true;
    opt.cols = 2; opt.colsDefined = true;

    parameters_t r = solveParameters(def, cfg, opt, w, h);

    ASSERT_TRUE(r.fullscreenDefined);
    ASSERT_TRUE(r.fullscreen);

    ASSERT_EQ_INT(r.rows, h / 8);
    ASSERT_EQ_INT(r.cols, w / 8);
}

static void test_solveParameters_fullscreen_false_uses_choose_for_rows_cols() {
    int w = 1920, h = 1080;

    parameters_t def = getBaseDefaultParameters();
    assert_base_defaults_fully_defined(def);

    parameters_t cfg = params_null();
    parameters_t opt = params_null();

    cfg = params_only_fullscreen(false);

    cfg.rows = 111; cfg.rowsDefined = true;
    cfg.cols = 222; cfg.colsDefined = true;

    opt.rows = 333; opt.rowsDefined = true;
    opt.cols = 444; opt.colsDefined = true;

    parameters_t r = solveParameters(def, cfg, opt, w, h);

    ASSERT_TRUE(r.fullscreenDefined);
    ASSERT_FALSE(r.fullscreen);

    ASSERT_TRUE(r.rowsDefined);
    ASSERT_TRUE(r.colsDefined);
    ASSERT_EQ_INT(r.rows, 333);
    ASSERT_EQ_INT(r.cols, 444);
}

static void test_solveParameters_all_sources_null_stays_undefined() {
    int w = 1920, h = 1080;

    parameters_t z = params_null();
    parameters_t r = solveParameters(z, z, z, w, h);

    ASSERT_FALSE(r.rowsDefined);       ASSERT_EQ_INT(r.rows, 0);
    ASSERT_FALSE(r.colsDefined);       ASSERT_EQ_INT(r.cols, 0);
    ASSERT_FALSE(r.blockSizeDefined);  ASSERT_EQ_INT(r.blockSize, 0);
    ASSERT_FALSE(r.fpsDefined);        ASSERT_EQ_INT(r.fps, 0);
    ASSERT_FALSE(r.delayDefined);      ASSERT_EQ_INT(r.delay, 0);
    ASSERT_FALSE(r.fullscreenDefined); ASSERT_EQ_BOOL(r.fullscreen, false);
    ASSERT_FALSE(r.partyDefined);      ASSERT_EQ_BOOL(r.party, false);
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
    test_solveParameters_fieldwise_independence_non_fullscreen();

    test_solveParameters_fullscreen_true_derives_rows_cols_from_window_and_blocksize();
    test_solveParameters_fullscreen_true_ignores_rows_cols_sources();
    test_solveParameters_fullscreen_false_uses_choose_for_rows_cols();

    test_solveParameters_all_sources_null_stays_undefined();

    return 0;
}
