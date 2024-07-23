#ifndef DMA_H
#define DMA_H
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "ps_config.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(DMA){
    tlm_utils::simple_initiator_socket<DMA> init_dram_socket;
    //tlm_utils::simple_initiator_socket<DMA> init_sram_socket;

    SC_CTOR(DMA) : init_dram_socket("init_dram_socket")
                 //, init_sram_socket("init_sram_socket") 
    {
        SC_THREAD(dma_copy);
        dont_initialize();
        sensitive << en;
    }
    void dma_copy();
    uint32_t  buf[1];
    
    sc_in<bool> en;
    sc_in<bool> weight;
    sc_in<bool> image;
    //sc_out<bool> sram_en;
    sc_in<sc_uint<ADDR_LENGTH> > src_addr;// DRAM address
};
#endif