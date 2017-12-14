// vim: set noexpandtab:tabstop=4:softtabstop=0:shiftwidth=4

#include "generator.h"
#include "mem_maps.h"
#include "LCDC_registermap.h"

// To simplify the code
using namespace sc_core;
using namespace std;

// Main constructor
Generator::Generator(sc_module_name name) : sc_module(name) {
	SC_THREAD(main_thread);
	SC_METHOD(wait_for_interrupt);
}

// Main thread
void Generator::main_thread() {
	// Initialize LCDC
	initiator.write(
		LCDC_REGS_START_ADDR + LCDC_ADDR_REG,
		MEMORY_VIDEO_START_ADDR
	);
	initiator.write(LCDC_REGS_START_ADDR + LCDC_START_REG, 0);

	// Variables needed for the animation
	int animation_offset = 0;
	uint32_t tmp_shader;

	// Main animation loop
	while (true) {
		// Wait for the interruption event
		initiator.write(LCDC_REGS_START_ADDR + LCDC_INT_REG, 0);
		wait(interrupt_event);

		// Update image in 3 easy steps:
		// 1. Fetch pixels from ROM
		// 2. Apply shader
		// 3. Write pixels to video buffer

		ensitlm::data_t pixels_rom, tmp;
		uint8_t pixels[8];
		ensitlm::addr_t addr_offset = 0;
		for (int y = 0; y < 240; ++y) {
			for (int x = 0; x < 40; ++x) {
				// Read pixels
				initiator.read(ROM_START_ADDR + addr_offset, pixels_rom);
				pixels[0] = (uint8_t) ((pixels_rom & 0xF0000000) >> 24);
				pixels[1] = (uint8_t) ((pixels_rom & 0x0F000000) >> 20);
				pixels[2] = (uint8_t) ((pixels_rom & 0x00F00000) >> 16);
				pixels[3] = (uint8_t) ((pixels_rom & 0x000F0000) >> 12);
				pixels[4] = (uint8_t) ((pixels_rom & 0x0000F000) >> 8);
				pixels[5] = (uint8_t) ((pixels_rom & 0x00000F00) >> 4);
				pixels[6] = (uint8_t) ((pixels_rom & 0x000000F0) >> 0);
				pixels[7] = (uint8_t) ((pixels_rom & 0x0000000F) << 4);

				// Apply shader
				for (int j = 0; j < 8; j++) {
					const int max_shade = 0xA0;
					const double magic_value1 = 160.;
					// "MAGIC" below, do not touch!
					// You can play around with the constants above
					tmp_shader = (uint32_t) (max_shade * abs(sin(
						(x * 8 + j + animation_offset) / magic_value1
					)));

					if (pixels[j] > tmp_shader) {
						pixels[j] -= tmp_shader;
					} else {
						pixels[j] = 0;
					}
				}

				// Write pixels to memory
				tmp = (pixels[3] << 0) |
				      (pixels[2] << 8) |
				      (pixels[1] << 16) |
				      (pixels[0] << 24);
				initiator.write(
					MEMORY_VIDEO_START_ADDR + 2 * addr_offset,
					tmp
				);
				tmp = (pixels[7] << 0) |
				      (pixels[6] << 8) |
				      (pixels[5] << 16) |
				      (pixels[4] << 24);
				initiator.write(
					MEMORY_VIDEO_START_ADDR + 2 * addr_offset + 4,
					tmp
				);

				// Go to next group of pixels
				addr_offset += 4;
			}
		}

		// Progress animation
		animation_offset++;

		// This constant is linked to the previous magic value of the shader
		// If one is modified, the other should also be modified or else
		// The animation will have "jumps" at certain times
		const int magic_value2 = 500;

		// Loop animation
		if (animation_offset > magic_value2) {
			animation_offset = 0;
		}
	}
}

// Interrupt management
void Generator::wait_for_interrupt() {
	// Wait for positive edge on the interrupt signal
	next_trigger(irq.posedge_event());
	cout << name() << ": Received interrupt signal" << endl;
	// Notify main thread about interrupt
	interrupt_event.notify();
}

