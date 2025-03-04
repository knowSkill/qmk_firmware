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

typedef enum {
    STATE_IDLE = 0,
    STATE_PRESS_UP,
    STATE_PRESS_DOWN,
    STATE_PRESS_PGUP,
    STATE_PRESS_PGDN,
    STATE_PRESS_ALT_TAB,
    STATE_MAX,
} my_state_t;

void keyboard_post_init_user(void) {
    srand(timer_read());  // Seed the random generator
}

static my_state_t   current_state = STATE_IDLE;
static uint16_t     last_event_time = 0;
static uint8_t      current_macro = MACRO_OFF;

// Runs every cycle
void matrix_scan_user(void) {
    static uint16_t delay = 0;
    static uint8_t rand_loops = 0;
    uint8_t select_state = MACRO_OFF;

    if (current_macro == MACRO_OFF || timer_elapsed(last_event_time) < delay)
        return;

    switch (current_state) {
        case STATE_IDLE:
            switch(current_macro) {
                case MACRO_1:
                    select_state = rand()%(STATE_MAX);
                    break;
                case MACRO_2:
                    select_state = rand()%(STATE_MAX-1);
                    break;
                case MACRO_3:
                    select_state = rand()%(STATE_MAX-2);
                    break;
            }
                
            switch(select_state) {
                case STATE_PRESS_UP:
                    current_state = STATE_PRESS_UP;
                    rand_loops = (rand()%15)+5;
                    break;
                case STATE_PRESS_DOWN:
                    current_state = STATE_PRESS_DOWN;
                    rand_loops = (rand()%15)+5;
                    break;
                case STATE_PRESS_PGUP:
                    current_state = STATE_PRESS_PGUP;
                    rand_loops = (rand()%3)+1;
                    break;
                case STATE_PRESS_PGDN:
                    current_state = STATE_PRESS_PGDN;
                    rand_loops = (rand()%3)+1;
                    break;
                case STATE_PRESS_ALT_TAB:
                    current_state = STATE_PRESS_ALT_TAB;
                    rand_loops = 0;
                    break;
            }

            delay = (rand()%2000)+1000;
            break;

        case STATE_PRESS_UP:
            tap_code_delay(KC_UP, rand()%20);
            delay = rand()%600+200;

            rand_loops--;
            if (rand_loops == 0) {
                current_state = STATE_IDLE;
            }
            break;

        case STATE_PRESS_DOWN:
            tap_code_delay(KC_DOWN, rand()%20);
            delay = rand()%600+200;

            rand_loops--;
            if (rand_loops == 0) {
                current_state = STATE_IDLE;
            }
            break;

        case STATE_PRESS_PGUP:
            tap_code_delay(KC_PGUP, rand()%20);
            delay = rand()%600+200;

            rand_loops--;
            if (rand_loops == 0) {
                current_state = STATE_IDLE;
            }
            break;

        case STATE_PRESS_PGDN:
            tap_code_delay(KC_PGDN, rand()%20);
            delay = rand()%600+200;

            rand_loops--;
            if (rand_loops == 0) {
                current_state = STATE_IDLE;
            }
            break;

        case STATE_PRESS_ALT_TAB:
            tap_code16(LALT(KC_TAB));
            delay = 0;
            rand_loops = 0;
            current_state = STATE_IDLE;
            break;
        case STATE_MAX:
        default:
            break;
    }

    last_event_time = timer_read();
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