

#include "generator.h"
#include "memory.h"

int sc_main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    Generator gen("Generator1");
    Memory mem("Memory1", 512);
    Bus bus("Bus");

    bus.map(mem.target, 0x10000000, 0x100000FF);

    gen.socket.bind(bus.target);
    bus.initiator.bind(mem.target);

    sc_core::sc_start();
    return 0;
}