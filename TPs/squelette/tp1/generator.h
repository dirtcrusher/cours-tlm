#include "ensitlm.h"
#include "bus.h"

struct Generator : sc_core::sc_module {
    ensitlm::initiator_socket<Generator> socket;

    void thread(void);

    SC_CTOR(Generator);
};