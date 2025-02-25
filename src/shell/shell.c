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

	if (curr_line_size + 1 > curr_line_capacity) {
		char *new_line = kmalloc(curr_line_capacity * 2 + 1);
		if (!curr_line_size) {
			goto assign;
		}
		memcpy(new_line, curr_line, curr_line_size);
		kfree(curr_line);
	assign:
		curr_line = new_line;
		curr_line_capacity = curr_line_capacity * 2 + 1;
	}

	bool new_char = false;

	switch (event.keycode) {
		case KEY_A:
			curr_line[curr_line_size++] = shift_is_held ? 'A' : 'a';
			new_char = true;
			break;
		case KEY_B:
			curr_line[curr_line_size++] = shift_is_held ? 'B' : 'b';
			new_char = true;
			break;
		case KEY_C:
			curr_line[curr_line_size++] = shift_is_held ? 'C' : 'c';
			new_char = true;
			break;
		case KEY_D:
			curr_line[curr_line_size++] = shift_is_held ? 'D' : 'd';
			new_char = true;
			break;
		case KEY_E:
			curr_line[curr_line_size++] = shift_is_held ? 'E' : 'e';
			new_char = true;
			break;
		case KEY_F:
			curr_line[curr_line_size++] = shift_is_held ? 'F' : 'f';
			new_char = true;
			break;
		case KEY_G:
			curr_line[curr_line_size++] = shift_is_held ? 'G' : 'g';
			new_char = true;
			break;
		case KEY_H:
			curr_line[curr_line_size++] = shift_is_held ? 'H' : 'h';
			new_char = true;
			break;
		case KEY_I:
			curr_line[curr_line_size++] = shift_is_held ? 'I' : 'i';
			new_char = true;
			break;
		case KEY_J:
			curr_line[curr_line_size++] = shift_is_held ? 'J' : 'j';
			new_char = true;
			break;
		case KEY_K:
			curr_line[curr_line_size++] = shift_is_held ? 'K' : 'k';
			new_char = true;
			break;
		case KEY_L:
			curr_line[curr_line_size++] = shift_is_held ? 'L' : 'l';
			new_char = true;
			break;
		case KEY_M:
			curr_line[curr_line_size++] = shift_is_held ? 'M' : 'm';
			new_char = true;
			break;
		case KEY_N:
			curr_line[curr_line_size++] = shift_is_held ? 'N' : 'n';
			new_char = true;
			break;
		case KEY_O:
			curr_line[curr_line_size++] = shift_is_held ? 'O' : 'o';
			new_char = true;
			break;
		case KEY_P:
			curr_line[curr_line_size++] = shift_is_held ? 'P' : 'p';
			new_char = true;
			break;
		case KEY_Q:
			curr_line[curr_line_size++] = shift_is_held ? 'Q' : 'q';
			new_char = true;
			break;
		case KEY_R:
			curr_line[curr_line_size++] = shift_is_held ? 'R' : 'r';
			new_char = true;
			break;
		case KEY_S:
			curr_line[curr_line_size++] = shift_is_held ? 'S' : 's';
			new_char = true;
			break;
		case KEY_T:
			curr_line[curr_line_size++] = shift_is_held ? 'T' : 't';
			new_char = true;
			break;
		case KEY_U:
			curr_line[curr_line_size++] = shift_is_held ? 'U' : 'u';
			new_char = true;
			break;
		case KEY_V:
			curr_line[curr_line_size++] = shift_is_held ? 'V' : 'v';
			new_char = true;
			break;
		case KEY_W:
			curr_line[curr_line_size++] = shift_is_held ? 'W' : 'w';
			new_char = true;
			break;
		case KEY_X:
			curr_line[curr_line_size++] = shift_is_held ? 'X' : 'x';
			new_char = true;
			break;
		case KEY_Y:
			curr_line[curr_line_size++] = shift_is_held ? 'Y' : 'y';
			new_char = true;
			break;
		case KEY_Z:
			curr_line[curr_line_size++] = shift_is_held ? 'Z' : 'z';
			new_char = true;
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
			curr_line[curr_line_size++] = 0;
			break;
		default:
			break;
	}

	if (new_char) {
		kputch(curr_line[curr_line_size - 1]);
	}
}

void display_prompt(void) { kprint(" > "); }

void reset_prompt_buff(void) {
	curr_line_capacity = 0;
	curr_line_size = 0;
	kfree(curr_line);
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
