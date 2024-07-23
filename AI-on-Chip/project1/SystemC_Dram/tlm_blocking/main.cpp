#include "DRAM.h"
#include "DataLoader.h"
#include "DMA.h"

#include "PEArray.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
/*
void model1_layer1 (int in_h, int in_w, int in_c, int kernel, int num_kernel, sc_signal<bool> &dma_weight, sc_signal<bool> &dma_image) {
    dma_weight = true;
}
*/
int sc_main(int argc, char* argv[])
{

    DRAM *dram;
    DataLoader *dataloader;
    DMA *dma;

    PEArray *pearray;

    dram = new DRAM("dram");
    dataloader = new DataLoader("dataloader");
    dma = new DMA("dma");

    pearray = new PEArray("pearray");

    sc_signal<bool> loading_finished;
    sc_signal<bool> loading_start;
    sc_signal<bool> dma_en;
    sc_signal<bool> dma_weight;
    sc_signal<bool> dma_image;
    sc_signal<sc_uint<32> > dma_src_addr;

    //sc_signal<sc_uint<DATA_LENGTH> > data_out;
    sc_signal_rv<32> data_out;

    // -------------------- Generate .vcd file
    //sc_trace_file *my_trace_file;
    //my_trace_file = sc_create_vcd_trace_file ("pe_trace");
    //sc_trace(my_trace_file, data_out, "data_out");

    dataloader->socket.bind(dram->socket1);
    dataloader->loading_start(loading_start);
    dataloader->loading_finished(loading_finished);
    
    dma->init_dram_socket.bind(dram->socket2);
    dma->en(dma_en);
    dma->weight(dma_weight);
    dma->image(dma_image);
    dma->src_addr(dma_src_addr);

    //pearray->socket.bind(dram->socket2);
    pearray->data_out(data_out);
    
    sc_start();
    /*
    loading_start = true;
    dma_en = false;
    sc_start();
    
    if (loading_finished) {
        cout << "Loading Finished!" << endl;
        loading_start = false;
        //dma_en = true;
        //model1_layer1(416, 416, 3, 3, 16, dma_weight, dma_image);
        //sc_start();
    }
    */
    //sc_close_vcd_trace_file (my_trace_file);
    
    return 0;
}