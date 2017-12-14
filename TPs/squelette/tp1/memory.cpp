#include "memory.h"

Memory::Memory(sc_core::sc_module_name name, unsigned int size) : sc_core::sc_module(name) {
    this->size = size;
    this->storage = new ensitlm::data_t[size];
}

tlm::tlm_response_status Memory::write(const ensitlm::addr_t &addr, const ensitlm::data_t &data) {
    // Debug print
    //std::cout << name() << ": Received write command for address (" << addr << ") and data (" << data << ")" << std::endl;

    // Verify address
    if (addr % 4 != 0 || addr > this->size * 4) {
        SC_REPORT_ERROR(this->name(), "Received write command with incorrect address");
        return tlm::tlm_response_status::TLM_ADDRESS_ERROR_RESPONSE;
    }

    // Actually write the data
    this->storage[addr / 4] = data;

    return tlm::tlm_response_status::TLM_OK_RESPONSE;
}

tlm::tlm_response_status Memory::read(const ensitlm::addr_t &addr, ensitlm::data_t &data) {
    // Debug print
    //std::cout << name() << ": Received read command for address (" << addr << ") and data (" << data << ")" << std::endl;

    // Verify address
    if (addr % 4 != 0 || addr > this->size * 4) {
        SC_REPORT_ERROR(this->name(), "Received read command with incorrect address");
        return tlm::tlm_response_status::TLM_ADDRESS_ERROR_RESPONSE;
    }

    // Actually read the data
    data = this->storage[addr / 4];

    return tlm::tlm_response_status::TLM_OK_RESPONSE;
}

Memory::~Memory() {
    delete this->storage;
}
