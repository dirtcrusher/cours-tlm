#include "generator.h"

Generator::Generator(sc_core::sc_module_name name) : sc_core::sc_module(name) {
    SC_THREAD(thread);
}

void Generator::thread() {

    // Write data
    for (int i = 0; i < 512; ++i) {
        auto status = this->socket.write(0x10000000 + 4 * i, 512 - i);
        if (status != tlm::tlm_response_status::TLM_OK_RESPONSE) {
            SC_REPORT_ERROR(this->name(), "Received incorrect write response");
        }
    }

    // Read data and verify
    ensitlm::data_t data;
    for (int i = 0; i < 512; ++i) {
        auto status = this->socket.read(0x10000000 + 4 * i, data);
        if (status != tlm::tlm_response_status::TLM_OK_RESPONSE) {
            SC_REPORT_ERROR(this->name(), "Received incorrect read response");
        }
        if (data != (unsigned int) (512 - i)) {
            SC_REPORT_ERROR(this->name(), "Received incorrect data");
        }
    }
}