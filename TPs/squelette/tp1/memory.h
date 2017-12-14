#include "ensitlm.h"
#include "bus.h"

struct Memory : sc_core::sc_module {
    ensitlm::target_socket<Memory> target;

    ensitlm::data_t *storage;
    unsigned int size;

    tlm::tlm_response_status write(const ensitlm::addr_t &addr, const ensitlm::data_t &data);

    tlm::tlm_response_status read(const ensitlm::addr_t &addr, ensitlm::data_t &data);

    Memory(sc_core::sc_module_name name, unsigned int size);

    ~Memory();
};