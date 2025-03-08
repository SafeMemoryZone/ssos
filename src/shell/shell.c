#include "shell.h"

#include <stddef.h>

#include "../drivers/screen.h"
#include "drivers/keyboard.h"
#include "mem/mem.h"
#include "misc.h"

static bool prompt_comfirmed;
static char *curr_line;
static size_t curr_line_capacity;
static size_t curr_line_size;

void terminal_input_callback(keyboard_event_t event) {
	static bool shift_is_held = false;
	char c = 0;

    // TODO: expand the callback
	if (event.is_released) {
		return;
	}

	switch (event.keycode) {
		case KEY_A:
			c = shift_is_held ? 'A' : 'a';
			break;
		case KEY_B:
			c = shift_is_held ? 'B' : 'b';
			break;
		case KEY_C:
			c = shift_is_held ? 'C' : 'c';
			break;
		case KEY_D:
			c = shift_is_held ? 'D' : 'd';
			break;
		case KEY_E:
			c = shift_is_held ? 'E' : 'e';
			break;
		case KEY_F:
			c = shift_is_held ? 'F' : 'f';
			break;
		case KEY_G:
			c = shift_is_held ? 'G' : 'g';
			break;
		case KEY_H:
			c = shift_is_held ? 'H' : 'h';
			break;
		case KEY_I:
			c = shift_is_held ? 'I' : 'i';
			break;
		case KEY_J:
			c = shift_is_held ? 'J' : 'j';
			break;
		case KEY_K:
			c = shift_is_held ? 'K' : 'k';
			break;
		case KEY_L:
			c = shift_is_held ? 'L' : 'l';
			break;
		case KEY_M:
			c = shift_is_held ? 'M' : 'm';
			break;
		case KEY_N:
			c = shift_is_held ? 'N' : 'n';
			break;
		case KEY_O:
			c = shift_is_held ? 'O' : 'o';
			break;
		case KEY_P:
			c = shift_is_held ? 'P' : 'p';
			break;
		case KEY_Q:
			c = shift_is_held ? 'Q' : 'q';
			break;
		case KEY_R:
			c = shift_is_held ? 'R' : 'r';
			break;
		case KEY_S:
			c = shift_is_held ? 'S' : 's';
			break;
		case KEY_T:
			c = shift_is_held ? 'T' : 't';
			break;
		case KEY_U:
			c = shift_is_held ? 'U' : 'u';
			break;
		case KEY_V:
			c = shift_is_held ? 'V' : 'v';
			break;
		case KEY_W:
			c = shift_is_held ? 'W' : 'w';
			break;
		case KEY_X:
			c = shift_is_held ? 'X' : 'x';
			break;
		case KEY_Y:
			c = shift_is_held ? 'Y' : 'y';
			break;
		case KEY_Z:
			c = shift_is_held ? 'Z' : 'z';
			break;
		case KEY_LEFT_SHIFT:
			shift_is_held = !event.is_released;
			break;
		case KEY_RIGHT_SHIFT:
			shift_is_held = !event.is_released;
			break;
		case KEY_ENTER:
		case KEY_KP_ENTER:
			prompt_comfirmed = true;
			break;
		default:
			break;
	}

	if (c) {
		kputch(c);

		if (curr_line_size + 1 > curr_line_capacity) {
			size_t new_capacity = curr_line_capacity ? curr_line_capacity * 2 + 1 : 1;
			char *new_line = kmalloc(new_capacity);
			if (!new_line) {
				kprint("[FATAL ERROR] Out of memory.\n");
				stop();
			}
			if (curr_line) {
				memcpy(new_line, curr_line, curr_line_size);
				kfree(curr_line);
			}
			curr_line = new_line;
			curr_line_capacity = new_capacity;
		}
		curr_line[curr_line_size++] = c;
	}
}

void display_prompt(void) { kprint("\n > "); }

void reset_prompt_buff(void) {
	curr_line_capacity = 0;
	curr_line_size = 0;
	if (curr_line) {
		kfree(curr_line);
	}
	curr_line = NULL;
}

void start_shell(void) {
	install_keyboard_event_callback(terminal_input_callback);
	for (;;) {
		display_prompt();
		for (;;) {
			wait_for_interrupts();
			if (prompt_comfirmed) {
				prompt_comfirmed = false;
				reset_prompt_buff();
				break;
			}
		}
	}
}
