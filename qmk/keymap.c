/*
 * QMK Keymap for Corne - matching ZMK config for Hyprland
 */

#include QMK_KEYBOARD_H

// Layer definitions
enum layers {
    _DEFAULT,
    _RAISE,
    _LOWER,
    _WIN_LEFT,
    _WIN_RIGHT,
    _WIN_NUM
};

// Custom keycodes for win layer behavior
enum custom_keycodes {
    WIN_B = SAFE_RANGE,  // Tap: B, Hold: LGUI + layer 3
    WIN_N,               // Tap: N, Hold: LGUI + layer 4
    WIN_NUM_COMBO        // Direct LGUI + layer 5 (for combo)
};

// Tap-hold state tracking with timers
static bool win_b_pressed = false;
static bool win_b_held = false;      // Has hold activated?
static uint16_t win_b_timer = 0;

static bool win_n_pressed = false;
static bool win_n_held = false;
static uint16_t win_n_timer = 0;

// Set initial LED color on startup
void keyboard_post_init_user(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(0, 0, 40);  // Dim white
#endif
}

// Check timers in matrix scan to activate hold after tapping term
void matrix_scan_user(void) {
    // Activate B hold after tapping term
    if (win_b_pressed && !win_b_held && timer_elapsed(win_b_timer) >= TAPPING_TERM) {
        win_b_held = true;
        register_code(KC_LGUI);
        layer_on(_WIN_LEFT);
    }
    // Activate N hold after tapping term
    if (win_n_pressed && !win_n_held && timer_elapsed(win_n_timer) >= TAPPING_TERM) {
        win_n_held = true;
        register_code(KC_LGUI);
        layer_on(_WIN_RIGHT);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WIN_B:
            if (record->event.pressed) {
                win_b_pressed = true;
                win_b_held = false;
                win_b_timer = timer_read();
            } else {
                if (win_b_held) {
                    // Was held - release
                    unregister_code(KC_LGUI);
                    layer_off(_WIN_LEFT);
                } else {
                    // Released before tapping term - tap B
                    tap_code(KC_B);
                }
                win_b_pressed = false;
                win_b_held = false;
            }
            return false;

        case WIN_N:
            if (record->event.pressed) {
                win_n_pressed = true;
                win_n_held = false;
                win_n_timer = timer_read();
            } else {
                if (win_n_held) {
                    unregister_code(KC_LGUI);
                    layer_off(_WIN_RIGHT);
                } else {
                    tap_code(KC_N);
                }
                win_n_pressed = false;
                win_n_held = false;
            }
            return false;

        case WIN_NUM_COMBO:
            // Combo activates immediately (no tapping term)
            if (record->event.pressed) {
                // Cancel any pending B/N tap
                win_b_pressed = false;
                win_n_pressed = false;
                register_code(KC_LGUI);
                layer_on(_WIN_NUM);
            } else {
                unregister_code(KC_LGUI);
                layer_off(_WIN_NUM);
            }
            return false;

        default:
            return true;
    }
}

// Combos
const uint16_t PROGMEM win_num_combo[] = {WIN_B, LT(_RAISE, KC_ESC), COMBO_END};

combo_t key_combos[] = {
    COMBO(win_num_combo, WIN_NUM_COMBO),
};

// Layer colors (HSV values)
#ifdef RGB_MATRIX_ENABLE
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _WIN_LEFT:
        case _WIN_RIGHT:
            // Blue/purple for win layers (SUPER held)
            rgb_matrix_sethsv_noeeprom(170, 255, 80);  // Blue
            break;
        case _WIN_NUM:
            // Cyan for win+numbers
            rgb_matrix_sethsv_noeeprom(128, 255, 80);  // Cyan
            break;
        case _RAISE:
            // Green for raise
            rgb_matrix_sethsv_noeeprom(85, 255, 80);   // Green
            break;
        case _LOWER:
            // Orange for lower
            rgb_matrix_sethsv_noeeprom(20, 255, 80);   // Orange
            break;
        default:
            // Dim white for default
            rgb_matrix_sethsv_noeeprom(0, 0, 40);      // White, dim
            break;
    }
    return state;
}
#endif

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Default Layer
     * ,--------------------------------------.                    ,------------------------------------.
     * |   =    |  Q  |  W  |  E  |  R  |  T  |                    |  Y  |  U  |  I  |  O  |  P  | LGUI |
     * |--------+-----+-----+-----+-----+-----|                    |-----+-----+-----+-----+-----+------|
     * |   (    |  A  |  S  |  D  |  F  |  G  |                    |  H  |  J  |  K  |  L  |  ;  |  -   |
     * |--------+-----+-----+-----+-----+-----|                    |-----+-----+-----+-----+-----+------|
     * |   )    |  Z  |  X  |ALT/C|CTL/V|WIN_B|                    |WIN_N|CTL/M|ALT/,|  .  |  /  |  '   |
     * `--------+-----+-----+-----+-----+-----+------.      ,------+-----+-----+-----+-----+-----+------'
     *                            |BSPC |LSpc |LEsc |       |LTab |RSpc | ENT |
     *                            `-----------------'       `---------- ------'
     *                            L=LOWER       L=RAISE    R=RAISE   RSHIFT
     */
    [_DEFAULT] = LAYOUT_split_3x6_3(
        KC_EQL,  KC_Q,    KC_W,    KC_E,         KC_R,         KC_T,              KC_Y,    KC_U,         KC_I,         KC_O,    KC_P,    KC_LGUI,
        KC_LPRN, KC_A,    KC_S,    KC_D,         KC_F,         KC_G,              KC_H,    KC_J,         KC_K,         KC_L,    KC_SCLN, KC_MINS,
        KC_RPRN, KC_Z,    KC_X,    LALT_T(KC_C), LCTL_T(KC_V), WIN_B,             WIN_N,   RCTL_T(KC_M), RALT_T(KC_COMM), KC_DOT, KC_SLSH, KC_QUOT,
                              KC_BSPC, LT(_LOWER, KC_SPC), LT(_RAISE, KC_ESC),    LT(_RAISE, KC_TAB), RSFT_T(KC_SPC), KC_ENT
    ),

    /*
     * Raise Layer (symbols + numpad)
     */
    [_RAISE] = LAYOUT_split_3x6_3(
        KC_GRV,  KC_GT,   KC_PIPE, KC_HASH, KC_LBRC, KC_RBRC,           KC_7,    KC_8,    KC_9,    KC_PAST, KC_PPLS, LCA(KC_DEL),
        _______, KC_TILD, KC_AMPR, KC_PERC, KC_LCBR, KC_RCBR,           KC_4,    KC_5,    KC_6,    KC_PSLS, KC_PMNS, KC_EQL,
        _______, KC_BSLS, KC_EXLM, KC_AT,   KC_CIRC, KC_DLR,            KC_1,    KC_2,    KC_3,    _______, _______, _______,
                                           _______, _______, _______,  _______, KC_0,    _______
    ),

    /*
     * Lower Layer (F-keys + navigation + Bluetooth placeholder)
     */
    [_LOWER] = LAYOUT_split_3x6_3(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,             KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        QK_BOOT, _______, _______, _______, _______, _______,           KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
        _______, _______, _______, _______, KC_TAB,  KC_GRV,            KC_HOME, KC_END,  _______, _______, _______, _______,
                                           _______, _______, _______,  _______, _______, _______
    ),

    /*
     * Left Win Layer (hold B) - LGUI held by macro
     * All letters available, numbers via layer 5
     */
    [_WIN_LEFT] = LAYOUT_split_3x6_3(
        KC_EQL,  KC_Q,    KC_W,    KC_E,         KC_R,         KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
        KC_LPRN, KC_A,    KC_S,    KC_D,         KC_F,         KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_MINS,
        KC_RPRN, KC_Z,    KC_X,    LALT_T(KC_C), LCTL_T(KC_V), XXXXXXX,           KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,
                                  KC_BSPC, LSFT_T(KC_SPC), MO(_WIN_NUM),          KC_TAB,  RSFT_T(KC_SPC), KC_ENT
    ),

    /*
     * Right Win Layer (hold N) - LGUI held by macro
     */
    [_WIN_RIGHT] = LAYOUT_split_3x6_3(
        KC_EQL,  KC_Q,    KC_W,    KC_E,         KC_R,         KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
        KC_LPRN, KC_A,    KC_S,    KC_D,         KC_F,         KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_MINS,
        KC_RPRN, KC_Z,    KC_X,    LALT_T(KC_C), LCTL_T(KC_V), KC_B,              XXXXXXX, KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,
                                  KC_BSPC, LSFT_T(KC_SPC), KC_ESC,                KC_LALT, RSFT_T(KC_SPC), KC_ENT
    ),

    /*
     * Win Numbers Layer - activated from left win layer or combo (B + inner thumb)
     * Numbers for workspace switching while LGUI held
     * =/- for SUPER+=/- resize bindings (Hyprland)
     */
    [_WIN_NUM] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,           KC_7,    KC_8,    KC_9,    _______, KC_EQL,  _______,
        _______, _______, _______, _______, _______, _______,           KC_4,    KC_5,    KC_6,    _______, KC_MINS, _______,
        _______, _______, _______, _______, _______, _______,           KC_1,    KC_2,    KC_3,    _______, _______, _______,
                                           _______, _______, _______,  _______, _______, _______
    )
};
