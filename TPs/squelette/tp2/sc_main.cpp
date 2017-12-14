// vim: set noexpandtab:tabstop=4:softtabstop=0:shiftwidth=4

#include "generator.h"
#include "memory.h"
#include "mem_maps.h"
#include "LCDC.h"
#include "ROM.h"

using namespace sc_core;

#define FPS 60

int sc_main(int argc, char **argv) {
	// Ignore program arguments
	(void) argc;
	(void) argv;

	// Instantiate modules
	Generator gen("Generator");
	Memory mem("Memory", MEMORY_TOTAL_SIZE);
	Bus bus("Bus");
	LCDC lcd_controller("LCD_Controller", sc_time::from_seconds(1.0 / FPS));
	ROM rom("ROM");

	// Address mappings
	bus.map(mem.target, MEMORY_START_ADDR, MEMORY_TOTAL_SIZE);
	bus.map(lcd_controller.target_socket, LCDC_REGS_START_ADDR, 12);
	bus.map(rom.socket, ROM_START_ADDR, ROM_SIZE);

	// Bind module targets to bus initiator
	bus.initiator.bind(mem.target);
	bus.initiator.bind(lcd_controller.target_socket);
	bus.initiator.bind(rom.socket);

	// Bind module initiators to bus target
	bus.target.bind(lcd_controller.initiator_socket);
	bus.target.bind(gen.initiator);

	// Connect interrupt signal
	sc_signal<bool, SC_MANY_WRITERS> irq_signal("IRQ");
	gen.irq.bind(irq_signal);
	lcd_controller.display_int.bind(irq_signal);

	// Start simulation
	sc_start();
	return 0;
}
