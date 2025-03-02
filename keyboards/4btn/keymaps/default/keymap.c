/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

// Macro States
enum {
    MACRO_OFF,
    MACRO_1,
    MACRO_2,
    MACRO_3
};

static uint8_t current_macro = MACRO_OFF;

void keyboard_post_init_user(void) {
    srand(timer_read());  // Seed the random generator
}

// Function to randomly press a key with a delay
void random_keypress(uint16_t *keys, size_t key_count) {
    if (key_count == 0) return;
    uint16_t random_delay = rand() % 5000;  // Random delay up to 5s

    //test
    random_delay=1000;

    wait_ms(random_delay);
    wait_ms(1000);

    uint16_t key = keys[rand() % key_count];  // Use QMK's rand()
    tap_code(key);
}

// The macro loop
void macro_loop(void) {
    static uint16_t last_run = 0;
    if (current_macro == MACRO_OFF || timer_elapsed(last_run) < 1000) return;
    
    last_run = timer_read();

    switch (current_macro) {
        case MACRO_1: {
            // uint16_t keys[] = { KC_PGUP, KC_PGDN, KC_UP, KC_DOWN, LALT(KC_TAB) };
            tap_code(KC_UP);
            //random_keypress(keys, sizeof(keys) / sizeof(keys[0]));
            break;
        }
        case MACRO_2: {
            tap_code16(LALT(KC_TAB));
            //random_keypress(keys, sizeof(keys) / sizeof(keys[0]));
            break;
        }
        case MACRO_3: {
            uint16_t keys[] = { KC_PGUP, KC_PGDN };
            random_keypress(keys, sizeof(keys) / sizeof(keys[0]));
            break;
        }
    }
}

// Runs every cycle
void matrix_scan_user(void) {
    macro_loop();
}

// Keymap definition
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        // KC_BTN1, KC_BTN2, KC_BTN3, KC_BTN4  // Button mappings
        KC_BTN1, KC_BTN2, KC_BTN3, KC_D  // Button mappings
    )
};

// Process button presses
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_BTN1:
                current_macro = MACRO_OFF;
                break;
            case KC_BTN2:
                current_macro = MACRO_1;
                break;
            case KC_BTN3:
                current_macro = MACRO_2;
                break;
            case KC_BTN4:
                current_macro = MACRO_3;
                break;
        }
    }
    return true;
}