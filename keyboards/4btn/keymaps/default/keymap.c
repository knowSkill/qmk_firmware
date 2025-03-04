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
#include "pico/bootrom.h"  // ✅ This provides reset_usb_boot()

// Macro States
enum {
    MACRO_OFF,
    MACRO_1,
    MACRO_2,
    MACRO_3
};

// Macro Sequence Selection
enum {
    ALT_TAB_UP_DOWN_PGUP_PGDOWN,
    UP_DOWN_PGUP_PGDOWN,
    UP_DOWN
};

// Keyboard sequence type
enum {
    UP_OR_DOWN,
    PGUP_OR_PGDN,
    ALT_TAB,
    SEQ_TYPE_MAX
};

static uint8_t current_macro = MACRO_OFF;

void keyboard_post_init_user(void) {
    srand(timer_read());  // Seed the random generator
}

void macro_sequence_random(uint8_t selection) {

    uint8_t action = rand() % SEQ_TYPE_MAX;
    uint8_t rand_loops;
    uint16_t key;
    static uint8_t alt_tab_count = 0;

    switch (action) {
        case ALT_TAB:
            if (alt_tab_count++ >= 10)
            {
                tap_code16(LALT(KC_TAB));
                alt_tab_count = 0;
            }
            break;
        case PGUP_OR_PGDN:
            rand_loops = rand()%3;
            key = rand()%2 ? KC_PGUP : KC_PGDN;

            for(uint8_t i=0; i<rand_loops; i++)
            {
                tap_code_delay(key, rand()%20);
                wait_ms(rand()%300+100);
            }
            break;
        case UP_OR_DOWN:
            rand_loops = rand()%5;
            key = rand()%2 ? KC_UP : KC_DOWN;

            for(uint8_t i=0; i<rand_loops; i++)
            {
                tap_code_delay(key, rand()%20);
                wait_ms(rand()%300+100);
            }
            break;
    }
}

// The macro loop
void macro_loop(void) {
    static uint16_t last_run = 0;
    static uint16_t random_delay = 0;

    if (current_macro == MACRO_OFF || timer_elapsed(last_run) < random_delay)
        return;

    random_delay = (rand() % 1500)+500;  // Random delay up to 2s
    last_run = timer_read();

    switch (current_macro) {
        case MACRO_1: {
            macro_sequence_random(ALT_TAB_UP_DOWN_PGUP_PGDOWN);
            break;
        }
        case MACRO_2: {
            macro_sequence_random(ALT_TAB_UP_DOWN_PGUP_PGDOWN);
            break;
        }
        case MACRO_3: {
            uint8_t rand_loops = rand()%5;
            uint16_t key = rand()%2 ? KC_UP : KC_DOWN;

            for(uint8_t i=0; i<rand_loops; i++)
            {
                tap_code_delay(key, rand()%20);
                wait_ms(rand()%500+200);
            }
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
        KC_BTN1, KC_BTN2, KC_BTN3, KC_BTN4  // Button mappings
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
                // current_macro = MACRO_3;
                reset_usb_boot(0, 0);  // Forces BOOTSEL mode
                break;
        }
    }
    return true;
}