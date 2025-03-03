#include <stddef.h>

#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "interrupts/idt.h"
#include "interrupts/pic.h"
#include "limine.h"
#include "mem/mem.h"
#include "mem/paging.h"
#include "misc.h"
#include "shell/shell.h"

__attribute__((used,
               section(".limine_requests_start"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;
__attribute__((used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

__attribute__((used, section(".limine_requests"))) static volatile struct limine_memmap_request
    memory_map_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

__attribute__((
    used, section(".limine_requests"))) static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST, .revision = 0};

void kmain(void) {
	if (!LIMINE_BASE_REVISION_SUPPORTED) {
		stop();
	}

	// Don't support multiple framebuffers
	if (!memory_map_request.response || !hhdm_request.response || !framebuffer_request.response ||
	    framebuffer_request.response->framebuffer_count > 1) {
		stop();
	}

	init_screen(framebuffer_request.response->framebuffers[0]);
	set_font_scale(2);
	kprint("[INFO] Initilized screen.\n");
	init_idt();
	kprint("[INFO] Initilized IDT.\n");
	init_PIC();
	kprint("[INFO] Initilized PIC.\n");
	enable_interrupts();
	kprint("[INFO] Enabled interrupts.\n");

	if (init_keyboard()) {
		kprint("[FATAL ERROR] Failed to initilize keyboard driver. Stopping.\n");
		stop();
	}

	if (init_page_frame_allocator(*memory_map_request.response->entries,
	                              memory_map_request.response->entry_count,
	                              hhdm_request.response->offset)) {
		kprint("[FATAL ERROR] Failed to initilize page frame allocator. Stopping.\n");
		stop();
	}
	else {
		kprint("[INFO] Initilized page frame allocator.\n");
	}

	if (init_alloc()) {
		kprint("[FATAL ERROR] Failed to initilize kernel memory allocator. Stopping.\n");
		stop();
	}
	else {
		kprint("[INFO] Initilized kernel memory allocator.\n");
	}

	start_shell();
	stop();
}
