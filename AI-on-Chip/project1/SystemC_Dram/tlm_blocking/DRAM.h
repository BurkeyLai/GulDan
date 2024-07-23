#ifndef DRAM_H
#define DRAM_H

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "ps_config.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(DRAM){

    tlm_utils::simple_target_socket<DRAM> socket1;
    tlm_utils::simple_target_socket<DRAM> socket2;

    uint32_t mem[MEM_SIZE/4]; // 256MB (32 * 0x2000000 / 4 = 256 * 1024 * 1024)

    // DRAM timing
    int calculate_delay(int addr);
    uint32_t row_hit_miss_latency(int addr);
    int get_row_index(int);
    void access(bool,uint32_t,unsigned int ,uint32_t*);
    int row_index;
    void b_transport(tlm::tlm_generic_payload&, sc_time&);

    SC_CTOR(DRAM) : socket1("socket1")
                  , socket2("socket2")
    {
        // Register callback for incoming b_transport interface method call
        socket1.register_b_transport(this, &DRAM::b_transport);
        socket2.register_b_transport(this, &DRAM::b_transport);
        row_index = 0xffffffff; // Initial value
        for(int i=0; i<(MEM_SIZE/4); i++){
            mem[i] = 0;
        }
    }
    
    //sc_in<sc_uint<ADDR_LENGTH> > dram_addr_i;// DRAM address
    //sc_in<sc_uint<DATA_LENGTH> > data;
    //sc_in<sc_uint<1> > wr_enable;
    //sc_out<sc_uint<DATA_LENGTH> > dram_dataout_i;
};
#endif