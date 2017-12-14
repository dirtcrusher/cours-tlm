// vim: set noexpandtab:tabstop=4:softtabstop=0:shiftwidth=4

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "ensitlm.h"
#include "bus.h"

struct Generator : sc_core::sc_module {
	// Initiator socket
	ensitlm::initiator_socket<Generator> initiator;

	// Interrupt signal and event
	sc_core::sc_in<bool> irq;
	sc_core::sc_event interrupt_event;

	// Main thread which does the signal generation
	void main_thread();

	// Management of the interrupt signal and event
	void wait_for_interrupt();

	// Main constructor
	SC_CTOR(Generator);
};

#endif
