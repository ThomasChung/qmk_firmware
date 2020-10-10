/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum custom_keycodes {
  SELECT_EMOJI = SAFE_RANGE, 
  SOUND_PREFS,
};

enum encoder_names {
  _LEFT,
  _RIGHT,
};


// Shortcut to make keymap more readable
#define SLEEP   KC_SYSTEM_SLEEP	// sleep
#define EXPOSE   KC_F3			// expose
#define G_CAM   LGUI(KC_E)	// turn on or off camera
#define G_MIC   LGUI(KC_D)	// mute or unmute microphone
#define SCREENSHOT LGUI(LSFT(KC_4))  // screenshot

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        SELECT_EMOJI, SLEEP, KC_MUTE,
        SOUND_PREFS, SCREENSHOT, XXXXXXX,
        EXPOSE, G_CAM, G_MIC 
    ),
};



int16_t emoji_index = 0;      

uint16_t backspaces = 0;

const char emojis[10][20] = {
    " :smile: ",
    " :spaghetti: ",
    " :sweat: ",
    " :heart: ",
    " :sunflower: ",
    " :tada: ",
    " :doughnut: ",
    " :sleepy: ",
    " :poop: ",
};



void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            emoji_index++;
            if (emoji_index > 9) {
            	emoji_index = 0;
            }

            int i;
            
            // delete the old emoji	
            for (i = 0; i < backspaces; ++i) {
                register_code(KC_BSPACE);
                unregister_code(KC_BSPACE);
            }

	    // calc the length of the current emoji
	    backspaces = 0;
  	    for (i = 0; i < 40; i++) {
	        if (emojis[emoji_index][i] == '\0') {
	            break;
	        }
	        backspaces++;
	    }

            send_string(emojis[emoji_index]);	    

        } else {
            emoji_index--;
            if (emoji_index < 0) {
            	emoji_index = 9;
            }
            
            int i;
            
            // delete the old emoji	
            for (i = 0; i < backspaces; ++i) {
                register_code(KC_BSPACE);
                unregister_code(KC_BSPACE);
            }

	    // calc the lenght of the current emoji
	    backspaces = 0;
  	    for (i = 0; i < 40; i++) {
	        if (emojis[emoji_index][i] == '\0') {
	            break;
	        }
	        backspaces++;
	    }
	    
            send_string(emojis[emoji_index]);
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
}




bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  
  switch (keycode) {
    case SELECT_EMOJI:
      if (record->event.pressed && backspaces > 0) {
      	    backspaces = 0;
            register_code(KC_ENTER);
            unregister_code(KC_ENTER);
      } else {
        // Do something else when release
      }
      return false; // Skip all further processing of this key
    case SOUND_PREFS:
      if (record->event.pressed) {
         SEND_STRING(SS_LALT(SS_TAP(X_F2)));
         _delay_ms(1000);
         SEND_STRING(SS_LGUI("l"));
         _delay_ms(1000);
         SEND_STRING("sound");
         _delay_ms(200);
         SEND_STRING(SS_TAP(X_ENTER));
      } else {
        // Do something else when release
      }
      return false; // Skip all further processing of this key
           
    default:
      return true; // Process all other keycodes normally
  }
}
