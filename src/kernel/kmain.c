#include <stddef.h>

#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "interrupts/idt.h"
#include "interrupts/pic.h"
#include "limine.h"
#include "mem/paging.h"
#include "misc.h"
#include "shell/shell.h"

__attribute__((used,
               section(".limine_requests_start"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;
__attribute__((used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

__attribute__((
    used, section(".limine_requests"))) static volatile struct limine_memmap_request memory_map = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};

void kmain(void) {
	if (!LIMINE_BASE_REVISION_SUPPORTED) {
		stop();
	}

	// Don't support multiple framebuffers
	if (!framebuffer_request.response || framebuffer_request.response->framebuffer_count > 1) {
		stop();
	}

	init_screen(framebuffer_request.response->framebuffers[0]);
	set_font_scale(2);
	init_idt();
	init_PIC();
	enable_interrupts();
	init_keyboard();
	init_page_frame_allocator(*memory_map.response->entries, memory_map.response->entry_count);
	// start_shell();

	stop();
}
