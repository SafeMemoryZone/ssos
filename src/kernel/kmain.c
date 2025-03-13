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

	screen_init(framebuffer_request.response->framebuffers[0]);
	screen_set_font_scale(2);
	screen_print("[INFO] Initilized screen.\n");
	idt_init();
	screen_print("[INFO] Initilized IDT.\n");
	pic_init();
	screen_print("[INFO] Initilized PIC.\n");
	enable_interrupts();
	screen_print("[INFO] Enabled interrupts.\n");

	if (keyboard_init()) {
		screen_print("[FATAL ERROR] Failed to initilize keyboard driver. Stopping.\n");
		stop();
	}

	if (paging_init_frame_allocator(*memory_map_request.response->entries,
	                                memory_map_request.response->entry_count,
	                                hhdm_request.response->offset)) {
		screen_print("[FATAL ERROR] Failed to initilize page frame allocator. Stopping.\n");
		stop();
	}
	else {
		screen_print("[INFO] Initilized page frame allocator.\n");
	}

	if (kalloc_init()) {
		screen_print("[FATAL ERROR] Failed to initilize kernel memory allocator. Stopping.\n");
		stop();
	}
	else {
		screen_print("[INFO] Initilized kernel memory allocator.\n");
	}

	shell_init();
	screen_print("[WARNING] Kernel fallback executed. Stopping.\n");
	stop();
}
