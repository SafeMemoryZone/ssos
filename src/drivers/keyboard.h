#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	// Special keys
	KEY_NONE = 0,
	KEY_UNKNOWN,

	// Function keys
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	// Modifier keys
	KEY_LEFT_SHIFT,
	KEY_RIGHT_SHIFT,
	KEY_LEFT_CTRL,
	KEY_RIGHT_CTRL,
	KEY_LEFT_ALT,
	KEY_RIGHT_ALT,
	KEY_LEFT_GUI,
	KEY_RIGHT_GUI,

	// Alphanumeric keys
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	// Number row
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_ZERO,

	// Symbols
	KEY_MINUS,
	KEY_EQUAL,
	KEY_LEFT_BRACKET,
	KEY_RIGHT_BRACKET,
	KEY_BACKSLASH,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_GRAVE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,

	// Lock keys
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_SCROLLLOCK,

	// Whitespace & control keys
	KEY_TAB,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_ESCAPE,

	// Navigation keys
	KEY_CURSOR_LEFT,
	KEY_CURSOR_RIGHT,
	KEY_CURSOR_UP,
	KEY_CURSOR_DOWN,
	KEY_HOME,
	KEY_END,
	KEY_INSERT,
	KEY_DELETE,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_PRINT_SCREEN,

	// Keypad keys
	KEY_KP_0,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
	KEY_KP_PERIOD,
	KEY_KP_PLUS,
	KEY_KP_MINUS,
	KEY_KP_ASTERISK,
	KEY_KP_SLASH,
	KEY_KP_ENTER,

	// Media & system keys
	KEY_PLAY_PAUSE,
	KEY_STOP,
	KEY_PREVIOUS_TRACK,
	KEY_NEXT_TRACK,
	KEY_MUTE,
	KEY_VOLUME_DOWN,
	KEY_VOLUME_UP,
	KEY_CALCULATOR,
	KEY_MEDIA_SELECT,
	KEY_SLEEP,
	KEY_WAKE,
	KEY_POWER,
	KEY_MY_COMPUTER,
	KEY_EMAIL,
	KEY_WWW_HOME,
	KEY_WWW_BACK,
	KEY_WWW_FORWARD,
	KEY_WWW_REFRESH,
	KEY_WWW_SEARCH,
	KEY_WWW_STOP,
	KEY_WWW_FAVORITES,

	// Application keys
	KEY_APPS
} key_t;

typedef struct {
	key_t keycode;
	bool is_released;
} keyboard_event_t;

int init_keyboard(void);
keyboard_event_t consume_event(void);
void install_keyboard_event_callback(void (*callback)(keyboard_event_t));
#endif  // KEYBOARD_H
