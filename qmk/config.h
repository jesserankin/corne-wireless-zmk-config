/*
 * QMK Config for Corne - matching ZMK config
 */

#pragma once

// Tapping term (matches ZMK 150ms)
#define TAPPING_TERM 150

// Tap-hold settings
#define PERMISSIVE_HOLD       // Allows hold to trigger even if another key is pressed and released
#define HOLD_ON_OTHER_KEY_PRESS  // Similar to ZMK balanced flavor

// Combo settings
#define COMBO_COUNT 1
#define COMBO_TERM 50         // Matches ZMK combo timeout-ms

// RGB Matrix settings
#ifdef RGB_MATRIX_ENABLE
    #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
    #define RGB_MATRIX_DEFAULT_HUE 0
    #define RGB_MATRIX_DEFAULT_SAT 0      // 0 = white
    #define RGB_MATRIX_DEFAULT_VAL 40     // Dim (0-255)
    #define RGB_MATRIX_SLEEP              // Turn off when host sleeps
#endif
