#include "keyboard.h"

#include <stdint.h>

#include "interrupts/idt.h"
#include "ports.h"

#define MAX_RETRY_COUNT 3
#define MAX_KEYBOARD_BUFF_SIZE 24
#define DEFAULT_SCANCODE_SET 2

#define KBD_DATA 0x60
#define KBD_STATUS 0x64
#define KBD_COMMAND 0x64

#define KBD_COMMAND_ENABLE_SCANNING 0xF4
#define KBD_COMMAND_DISABLE_SCANNING 0xF5
#define KBD_COMMAND_SET_SCANCODE_SET 0xF0
#define KBD_COMMAND_WRITE_CCB 0x60

#define PS2_CONFIG_BYTE 0b00100101

#define KBD_RESP_ACK 0xFA
#define KBD_RESP_RESEND 0xFE

static keyboard_event_t keyboard_event_buff[MAX_KEYBOARD_BUFF_SIZE] = {0};
static int event_buff_idx = 0;
static int event_count = 0;
static void (*keyboard_callback)(keyboard_event_t);

static int keyboard_enable_scanning(void) {
	uint8_t resp;
	int retry_count = 0;

	do {
		if (retry_count++ == MAX_RETRY_COUNT) {
			return -1;
		}
		outb(KBD_DATA, KBD_COMMAND_ENABLE_SCANNING);
		io_wait();
		resp = inb(KBD_DATA);
	} while (resp == KBD_RESP_RESEND);

	return 0;
}

static int keyboard_disable_scanning(void) {
	uint8_t resp;
	int retry_count = 0;

	do {
		if (retry_count++ == MAX_RETRY_COUNT) {
			return -1;
		}
		outb(KBD_DATA, KBD_COMMAND_DISABLE_SCANNING);
		io_wait();
		resp = inb(KBD_DATA);
	} while (resp == KBD_RESP_RESEND);

	return 0;
}

static int keyboard_set_default_scancode_set(void) {
	uint8_t resp;
	int retry_count = 0;

	do {
		if (retry_count++ == MAX_RETRY_COUNT) {
			return -1;
		}
		outb(KBD_DATA, KBD_COMMAND_SET_SCANCODE_SET);
		io_wait();
		resp = inb(KBD_DATA);
	} while (resp == KBD_RESP_RESEND);

	if (resp != KBD_RESP_ACK) {
		return -2;
	}

	retry_count = 0;
	do {
		if (retry_count++ == MAX_RETRY_COUNT) {
			return -3;
		}
		outb(KBD_DATA, DEFAULT_SCANCODE_SET);
		io_wait();
		resp = inb(KBD_DATA);
	} while (resp == KBD_RESP_RESEND);

	if (resp != KBD_RESP_ACK) {
		return -4;
	}

	return 0;
}

static keyboard_event_t keyboard_translate_scancode(uint8_t scancode) {
	static bool is_extended = false;
	static bool is_released = false;

	static unsigned int pause_sequence_idx = 0;
	static const uint8_t pause_sequence[8] = {0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77};

	static bool is_print_screen_pressed_pending = false;
	static bool is_print_screen_released_pending = false;

	keyboard_event_t event = {0};

	if (scancode == 0xE0) {
		is_extended = true;
		return event;
	}
	if (scancode == 0xF0) {
		is_released = true;
		return event;
	}

	if (is_print_screen_pressed_pending && scancode == 0x7C) {
		is_print_screen_pressed_pending = false;
		event.keycode = KEY_PRINT_SCREEN;
		event.is_released = false;
		return event;
	}
	if (is_print_screen_released_pending && scancode == 0x12) {
		is_print_screen_released_pending = false;
		event.keycode = KEY_PRINT_SCREEN;
		event.is_released = true;
		return event;
	}

	if (pause_sequence_idx > 0 || scancode == 0xE1) {
		if (pause_sequence_idx == 0 && scancode == 0xE1) {
			pause_sequence_idx = 1;
			return event;
		}
		else {
			if (scancode == pause_sequence[pause_sequence_idx]) {
				pause_sequence_idx++;
				if (pause_sequence_idx == sizeof(pause_sequence)) {
					event.keycode = KEY_PLAY_PAUSE;
					event.is_released = false;
					pause_sequence_idx = 0;
					is_extended = false;
					is_released = false;
					return event;
				}
				event.keycode = KEY_NONE;
				return event;
			}
			else {
				pause_sequence_idx = 0;
			}
		}
	}

	if (is_extended) {
		switch (scancode) {
			case 0x10:
				event.keycode = KEY_WWW_SEARCH;
				break;
			case 0x11:
				event.keycode = KEY_RIGHT_ALT;
				break;
			case 0x12:
				event.keycode = KEY_NONE;
				is_print_screen_pressed_pending = true;
				break;
			case 0x14:
				event.keycode = KEY_RIGHT_CTRL;
				break;
			case 0x15:
				event.keycode = KEY_PREVIOUS_TRACK;
				break;
			case 0x18:
				event.keycode = KEY_WWW_FAVORITES;
				break;
			case 0x1F:
				event.keycode = KEY_LEFT_GUI;
				break;
			case 0x20:
				event.keycode = KEY_WWW_REFRESH;
				break;
			case 0x21:
				event.keycode = KEY_VOLUME_DOWN;
				break;
			case 0x23:
				event.keycode = KEY_MUTE;
				break;
			case 0x27:
				event.keycode = KEY_RIGHT_GUI;
				break;
			case 0x28:
				event.keycode = KEY_WWW_STOP;
				break;
			case 0x2B:
				event.keycode = KEY_CALCULATOR;
				break;
			case 0x2F:
				event.keycode = KEY_APPS;
				break;
			case 0x30:
				event.keycode = KEY_WWW_FORWARD;
				break;
			case 0x32:
				event.keycode = KEY_VOLUME_UP;
				break;
			case 0x34:
				event.keycode = KEY_PLAY_PAUSE;
				break;
			case 0x37:
				event.keycode = KEY_POWER;
				break;
			case 0x38:
				event.keycode = KEY_WWW_BACK;
				break;
			case 0x3A:
				event.keycode = KEY_WWW_HOME;
				break;
			case 0x3B:
				event.keycode = KEY_STOP;
				break;
			case 0x3F:
				event.keycode = KEY_SLEEP;
				break;
			case 0x40:
				event.keycode = KEY_MY_COMPUTER;
				break;
			case 0x48:
				event.keycode = KEY_EMAIL;
				break;
			case 0x4A:
				event.keycode = KEY_KP_SLASH;
				break;
			case 0x4D:
				event.keycode = KEY_NEXT_TRACK;
				break;
			case 0x50:
				event.keycode = KEY_MEDIA_SELECT;
				break;
			case 0x5A:
				event.keycode = KEY_KP_ENTER;
				break;
			case 0x5E:
				event.keycode = KEY_WAKE;
				break;
			case 0x69:
				event.keycode = KEY_END;
				break;
			case 0x6B:
				event.keycode = KEY_CURSOR_LEFT;
				break;
			case 0x6C:
				event.keycode = KEY_HOME;
				break;
			case 0x70:
				event.keycode = KEY_INSERT;
				break;
			case 0x71:
				event.keycode = KEY_DELETE;
				break;
			case 0x72:
				event.keycode = KEY_CURSOR_DOWN;
				break;
			case 0x74:
				event.keycode = KEY_CURSOR_RIGHT;
				break;
			case 0x75:
				event.keycode = KEY_CURSOR_UP;
				break;
			case 0x7A:
				event.keycode = KEY_PAGE_DOWN;
				break;
			case 0x7C:
				event.keycode = KEY_NONE;
				is_print_screen_released_pending = true;
				break;
			case 0x7D:
				event.keycode = KEY_PAGE_UP;
				break;
			default:
				event.keycode = KEY_UNKNOWN;
				break;
		}
		event.is_released = is_released;
	}
	else {
		switch (scancode) {
			case 0x01:
				event.keycode = KEY_F9;
				break;
			case 0x03:
				event.keycode = KEY_F5;
				break;
			case 0x04:
				event.keycode = KEY_F3;
				break;
			case 0x05:
				event.keycode = KEY_F1;
				break;
			case 0x06:
				event.keycode = KEY_F2;
				break;
			case 0x07:
				event.keycode = KEY_F12;
				break;
			case 0x09:
				event.keycode = KEY_F10;
				break;
			case 0x0A:
				event.keycode = KEY_F8;
				break;
			case 0x0B:
				event.keycode = KEY_F6;
				break;
			case 0x0C:
				event.keycode = KEY_F4;
				break;
			case 0x0D:
				event.keycode = KEY_TAB;
				break;
			case 0x0E:
				event.keycode = KEY_GRAVE;
				break;
			case 0x11:
				event.keycode = KEY_LEFT_ALT;
				break;
			case 0x12:
				event.keycode = KEY_LEFT_SHIFT;
				break;
			case 0x14:
				event.keycode = KEY_LEFT_CTRL;
				break;
			case 0x15:
				event.keycode = KEY_Q;
				break;
			case 0x16:
				event.keycode = KEY_1;
				break;
			case 0x1A:
				event.keycode = KEY_Z;
				break;
			case 0x1B:
				event.keycode = KEY_S;
				break;
			case 0x1C:
				event.keycode = KEY_A;
				break;
			case 0x1D:
				event.keycode = KEY_W;
				break;
			case 0x1E:
				event.keycode = KEY_2;
				break;
			case 0x21:
				event.keycode = KEY_C;
				break;
			case 0x22:
				event.keycode = KEY_X;
				break;
			case 0x23:
				event.keycode = KEY_D;
				break;
			case 0x24:
				event.keycode = KEY_E;
				break;
			case 0x25:
				event.keycode = KEY_4;
				break;
			case 0x26:
				event.keycode = KEY_3;
				break;
			case 0x29:
				event.keycode = KEY_SPACE;
				break;
			case 0x2A:
				event.keycode = KEY_V;
				break;
			case 0x2B:
				event.keycode = KEY_F;
				break;
			case 0x2C:
				event.keycode = KEY_T;
				break;
			case 0x2D:
				event.keycode = KEY_R;
				break;
			case 0x2E:
				event.keycode = KEY_5;
				break;
			case 0x31:
				event.keycode = KEY_N;
				break;
			case 0x32:
				event.keycode = KEY_B;
				break;
			case 0x33:
				event.keycode = KEY_H;
				break;
			case 0x34:
				event.keycode = KEY_G;
				break;
			case 0x35:
				event.keycode = KEY_Y;
				break;
			case 0x36:
				event.keycode = KEY_6;
				break;
			case 0x3A:
				event.keycode = KEY_M;
				break;
			case 0x3B:
				event.keycode = KEY_J;
				break;
			case 0x3C:
				event.keycode = KEY_U;
				break;
			case 0x3D:
				event.keycode = KEY_7;
				break;
			case 0x3E:
				event.keycode = KEY_8;
				break;
			case 0x41:
				event.keycode = KEY_COMMA;
				break;
			case 0x42:
				event.keycode = KEY_K;
				break;
			case 0x43:
				event.keycode = KEY_I;
				break;
			case 0x44:
				event.keycode = KEY_O;
				break;
			case 0x45:
				event.keycode = KEY_ZERO;
				break;
			case 0x46:
				event.keycode = KEY_9;
				break;
			case 0x49:
				event.keycode = KEY_PERIOD;
				break;
			case 0x4A:
				event.keycode = KEY_SLASH;
				break;
			case 0x4B:
				event.keycode = KEY_L;
				break;
			case 0x4C:
				event.keycode = KEY_SEMICOLON;
				break;
			case 0x4D:
				event.keycode = KEY_P;
				break;
			case 0x4E:
				event.keycode = KEY_MINUS;
				break;
			case 0x52:
				event.keycode = KEY_APOSTROPHE;
				break;
			case 0x54:
				event.keycode = KEY_LEFT_BRACKET;
				break;
			case 0x55:
				event.keycode = KEY_EQUAL;
				break;
			case 0x58:
				event.keycode = KEY_CAPSLOCK;
				break;
			case 0x59:
				event.keycode = KEY_RIGHT_SHIFT;
				break;
			case 0x5A:
				event.keycode = KEY_ENTER;
				break;
			case 0x5B:
				event.keycode = KEY_RIGHT_BRACKET;
				break;
			case 0x5D:
				event.keycode = KEY_BACKSLASH;
				break;
			case 0x66:
				event.keycode = KEY_BACKSPACE;
				break;
			case 0x69:
				event.keycode = KEY_KP_1;
				break;
			case 0x6B:
				event.keycode = KEY_KP_4;
				break;
			case 0x6C:
				event.keycode = KEY_KP_7;
				break;
			case 0x70:
				event.keycode = KEY_KP_0;
				break;
			case 0x71:
				event.keycode = KEY_KP_PERIOD;
				break;
			case 0x72:
				event.keycode = KEY_KP_2;
				break;
			case 0x73:
				event.keycode = KEY_KP_5;
				break;
			case 0x74:
				event.keycode = KEY_KP_6;
				break;
			case 0x75:
				event.keycode = KEY_KP_8;
				break;
			case 0x76:
				event.keycode = KEY_ESCAPE;
				break;
			case 0x77:
				event.keycode = KEY_NUMLOCK;
				break;
			case 0x78:
				event.keycode = KEY_F11;
				break;
			case 0x79:
				event.keycode = KEY_KP_PLUS;
				break;
			case 0x7A:
				event.keycode = KEY_KP_3;
				break;
			case 0x7B:
				event.keycode = KEY_KP_MINUS;
				break;
			case 0x7C:
				event.keycode = KEY_KP_ASTERISK;
				break;
			case 0x7D:
				event.keycode = KEY_KP_9;
				break;
			case 0x7E:
				event.keycode = KEY_SCROLLLOCK;
				break;
			case 0x83:
				event.keycode = KEY_F7;
				break;
			default:
				event.keycode = KEY_UNKNOWN;
				break;
		}
		event.is_released = is_released;
	}

	is_extended = false;
	is_released = false;
	return event;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void keyboard_irq_handler(interrupt_frame_t *frame, uint64_t irq_num) {
#pragma GCC diagnostic pop
	uint8_t scancode = inb(KBD_DATA);
	keyboard_event_t event = keyboard_translate_scancode(scancode);
	if (event.keycode == KEY_NONE || event.keycode == KEY_UNKNOWN) {
		return;
	}

	keyboard_event_buff[event_buff_idx] = event;
	event_buff_idx = (event_buff_idx + 1) % MAX_KEYBOARD_BUFF_SIZE;
	event_count = event_count < MAX_KEYBOARD_BUFF_SIZE ? event_count + 1 : MAX_KEYBOARD_BUFF_SIZE;

	if (keyboard_callback) {
		keyboard_callback(keyboard_get_event());
	}
}

static void keyboard_flush_buff(void) {
	while (inb(KBD_STATUS) & 0x1) {
		(void)inb(KBD_DATA);
	}
}

static void ps2_controller_configure(void) {
	while (inb(KBD_STATUS) & 0x02);
	outb(KBD_COMMAND, KBD_COMMAND_WRITE_CCB);
	while (inb(KBD_STATUS) & 0x02);
	outb(KBD_DATA, PS2_CONFIG_BYTE);
}

int keyboard_init(void) {
	// TODO: execute self-test, disable AUX ...
	if (keyboard_disable_scanning()) {
		return -1;
	}

	keyboard_flush_buff();
	ps2_controller_configure();

	if (keyboard_set_default_scancode_set()) {
		return -2;
	}

	idt_install_irq_driver(1, keyboard_irq_handler);

	if (keyboard_enable_scanning()) {
		return -3;
	}

	return 0;
}

keyboard_event_t keyboard_get_event(void) {
	int idx = ((event_buff_idx - event_count) + MAX_KEYBOARD_BUFF_SIZE) % MAX_KEYBOARD_BUFF_SIZE;
	keyboard_event_t event = keyboard_event_buff[idx];
	event_count--;
	return event;
}

void keyboard_install_event_callback(void (*callback)(keyboard_event_t)) {
	keyboard_callback = callback;
}
