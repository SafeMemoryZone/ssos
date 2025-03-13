#include "shell.h"

#include <stddef.h>

#include "../drivers/screen.h"
#include "drivers/keyboard.h"
#include "mem/mem.h"
#include "misc.h"

static bool shell_prompt_confirmed;
static char *shell_current_line;
static size_t shell_line_capacity;
static size_t shell_line_size;

void shell_input_callback(keyboard_event_t event) {
	static bool shell_shift_is_held = false;
	char c = 0;

	// TODO: expand the callback
	if (event.is_released) {
		return;
	}

	switch (event.keycode) {
		case KEY_A:
			c = shell_shift_is_held ? 'A' : 'a';
			break;
		case KEY_B:
			c = shell_shift_is_held ? 'B' : 'b';
			break;
		case KEY_C:
			c = shell_shift_is_held ? 'C' : 'c';
			break;
		case KEY_D:
			c = shell_shift_is_held ? 'D' : 'd';
			break;
		case KEY_E:
			c = shell_shift_is_held ? 'E' : 'e';
			break;
		case KEY_F:
			c = shell_shift_is_held ? 'F' : 'f';
			break;
		case KEY_G:
			c = shell_shift_is_held ? 'G' : 'g';
			break;
		case KEY_H:
			c = shell_shift_is_held ? 'H' : 'h';
			break;
		case KEY_I:
			c = shell_shift_is_held ? 'I' : 'i';
			break;
		case KEY_J:
			c = shell_shift_is_held ? 'J' : 'j';
			break;
		case KEY_K:
			c = shell_shift_is_held ? 'K' : 'k';
			break;
		case KEY_L:
			c = shell_shift_is_held ? 'L' : 'l';
			break;
		case KEY_M:
			c = shell_shift_is_held ? 'M' : 'm';
			break;
		case KEY_N:
			c = shell_shift_is_held ? 'N' : 'n';
			break;
		case KEY_O:
			c = shell_shift_is_held ? 'O' : 'o';
			break;
		case KEY_P:
			c = shell_shift_is_held ? 'P' : 'p';
			break;
		case KEY_Q:
			c = shell_shift_is_held ? 'Q' : 'q';
			break;
		case KEY_R:
			c = shell_shift_is_held ? 'R' : 'r';
			break;
		case KEY_S:
			c = shell_shift_is_held ? 'S' : 's';
			break;
		case KEY_T:
			c = shell_shift_is_held ? 'T' : 't';
			break;
		case KEY_U:
			c = shell_shift_is_held ? 'U' : 'u';
			break;
		case KEY_V:
			c = shell_shift_is_held ? 'V' : 'v';
			break;
		case KEY_W:
			c = shell_shift_is_held ? 'W' : 'w';
			break;
		case KEY_X:
			c = shell_shift_is_held ? 'X' : 'x';
			break;
		case KEY_Y:
			c = shell_shift_is_held ? 'Y' : 'y';
			break;
		case KEY_Z:
			c = shell_shift_is_held ? 'Z' : 'z';
			break;
		case KEY_LEFT_SHIFT:
			shell_shift_is_held = true;
			break;
		case KEY_RIGHT_SHIFT:
			shell_shift_is_held = true;
			break;
		case KEY_ENTER:
		case KEY_KP_ENTER:
			shell_prompt_confirmed = true;
			break;
		default:
			break;
	}

	if (c) {
		screen_putch(c);

		if (shell_line_size + 1 > shell_line_capacity) {
			size_t new_capacity = shell_line_capacity ? shell_line_capacity * 2 + 1 : 1;
			char *new_line = kmalloc(new_capacity);
			if (!new_line) {
				screen_print("[FATAL ERROR] Out of memory.\n");
				stop();
			}
			if (shell_current_line) {
				memcpy(new_line, shell_current_line, shell_line_size);
				kfree(shell_current_line);
			}
			shell_current_line = new_line;
			shell_line_capacity = new_capacity;
		}
		shell_current_line[shell_line_size++] = c;
	}
}

void shell_display_prompt(void) { screen_print("\n > "); }

void shell_reset_prompt_buffer(void) {
	shell_line_capacity = 0;
	shell_line_size = 0;
	if (shell_current_line) {
		kfree(shell_current_line);
	}
	shell_current_line = NULL;
}

void shell_init(void) {
	// Simple shell for now - no commands yet
	keyboard_install_event_callback(shell_input_callback);

	for (;;) {
		shell_display_prompt();
		for (;;) {
			wait_for_interrupts();
			if (shell_prompt_confirmed) {
				shell_prompt_confirmed = false;
				shell_reset_prompt_buffer();
				break;
			}
		}
	}
}
