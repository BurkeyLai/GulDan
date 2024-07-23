#ifndef DATALOADER_H
#define DATALOADER_H
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "ps_config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(DataLoader){
    tlm_utils::simple_initiator_socket<DataLoader> socket;

    SC_CTOR(DataLoader) : socket("socket"), addressCounter(0){
        SC_THREAD(thread_process);
        dont_initialize();
        sensitive << loading_start;
    }
    void master_access(uint32_t addr, int data, int length);
    void file_process(char *file_name);
    void thread_process();
    int data;
    uint32_t addressCounter;
    struct Layer {
        char *input_file, *output_file, *weight_file;
        int in_H, in_W, in_C, out_H, out_W, out_C, kernel_size, filter_num;
    };
    vector<Layer> layers;
    sc_in<bool> loading_start;
    sc_out<bool> loading_finished;
};
#endif