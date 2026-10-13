#include "punkos/math.h"
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] =
    LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] =
    LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] =
    LIMINE_REQUESTS_END_MARKER;

void kernel_main(void) {

    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision)) {
        for (;;) {
        }
    }

    if (framebuffer_request.response == NULL ||  framebuffer_request.response->framebuffer_count < 1) {
        for (;;) {
        }
    }

    struct limine_framebuffer *framebuffer =
        framebuffer_request.response->framebuffers[0];

    if (framebuffer->memory_model != LIMINE_FRAMEBUFFER_RGB ||
        framebuffer->bpp != 32) {
        for (;;) {
        }
    }

    volatile uint32_t *pixels = framebuffer->address;

    for (uint64_t y = 0; y < framebuffer->height; y++) {
        for (uint64_t x = 0; x < framebuffer->width; x++) {
            uint64_t index = y * (framebuffer->pitch / 4) + x;
            pixels[index] = 0x00ff00ff;
        }
    }

    volatile int resultadd = add(2,3);
    volatile int resultsub = sub(5,2);
    volatile int resultmult = mult(3,4);
    volatile int resultdivide = divide(10,2);

    (void)resultadd;
    (void)resultsub;
    (void)resultmult;
    (void)resultdivide;

    for (;;) {
        // Boucle principale du noyau

    }
}