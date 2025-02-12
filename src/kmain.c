#include <stddef.h>

#include "limine.h"
#include "misc.h"

__attribute__((used,
               section(".limine_requests_start"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;
__attribute__((used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

void kmain(void) {
	if (!LIMINE_BASE_REVISION_SUPPORTED) {
		stop();
	}

	stop();
}
