#include "DRAM.h"

void DRAM::b_transport( tlm::tlm_generic_payload& trans, sc_time& delay ){

    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64   addr = trans.get_address();
    unsigned char*  ptr = trans.get_data_ptr();
    unsigned int   len   = trans.get_data_length();
    
    delay = sc_time(0, SC_NS); // Accept delay
    int dram_delay = calculate_delay(addr);
    delay = delay + sc_time(dram_delay, SC_NS); // Latency
    wait(delay);
    if ( cmd == tlm::TLM_READ_COMMAND ){
        memcpy(ptr, &mem[addr/4], len);
    }
    else if ( cmd == tlm::TLM_WRITE_COMMAND ){
        memcpy(&mem[addr/4], ptr, len);
    }
}

// TBD: THE tRAS Is not used .should  consider it
int DRAM::calculate_delay(int addr)// CACULATE DRAM DELAY
{
    //return 0;
    return magic_delay * row_hit_miss_latency(addr) / CLK_CYCLE * CLK_CYCLE;
}

//Check DRAM Row hit or miss
uint32_t DRAM::row_hit_miss_latency(int addr)
{
    uint32_t dram_latency;

    if(row_index == 0xffffffff) { // the first row open
        dram_latency = tRAS + tRCD + tCL; // 39 + 15 + 15 = 69
    } else if(row_index == get_row_index(addr)) { //row hit
        dram_latency = tCL; // 15
    } else { // row miss
        dram_latency = tRP + tRCD + tCL; // 15 + 15 + 15 = 45
    }
    row_index = get_row_index(addr);

    return dram_latency;
}

int DRAM::get_row_index(int addr)
{
    return (addr>>(COLUMN_BIT + BANK_BIT + 2)); // 10 + 8 + 2 = 20
}