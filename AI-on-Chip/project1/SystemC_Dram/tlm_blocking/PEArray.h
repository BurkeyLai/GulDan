#ifndef PEARRAY_H
#define PEARRAY_H
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "ps_config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <time.h>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#define PE_NUM 16
#define REG_SIZE 5 // May use a sc_in to set this value
#define IMAGE_ROW 104

SC_MODULE(PEArray) {
public:
    //tlm_utils::simple_initiator_socket<PEArray> socket;

    SC_CTOR(PEArray) //: socket("socket") 
    {
        for(int i = 0; i < PE_NUM; ++i) {
            PE pe;
            for(int j = 0; j < REG_SIZE; ++j) {
                pe.weight_reg.push_back(0);
                pe.image_reg.push_back(0);
            }
            for(int j = 0; j < IMAGE_ROW; ++j) {
                pe.image_row_data.push_back(0);
            }
            pe_vector.push_back(pe);
        }
        
        //output_map.resize(414 * 414 * 16, 0);

        SC_THREAD(pe_process);
        
    }
    ~PEArray() {
        pe_vector.clear();
    }
    //sc_out<sc_uint<DATA_LENGTH> > data_out;
    sc_out_rv<32> data_out;
    
private:
    void pe_process();
    void file_process(char *file_name, int mode) {
        ifstream fp_in;
        fp_in.open(file_name, ios::in | ios::binary);
        if (fp_in.is_open()) {
            //cout << "File opened for reading." << endl;
            fp_in.seekg(0, ios::beg); // put file pointer to beginning of file
        } else {
            cout << "Unable to open " << file_name << "." << endl;
        }
        string line;
        if (mode == 0) {
            while (getline(fp_in, line)) {
                weight_data.push_back( atoi(line.c_str()) );
                //cout << "Address: " << addressCounter * 4 << " data: " << data << "\n";
                //addressCounter++;
            }
        } else if (mode == 1) {
            while (getline(fp_in, line)) {
                image_data.push_back( atoi(line.c_str()) );
            }
        } else {
            while (getline(fp_in, line)) {
                gt_data.push_back( atoi(line.c_str()) );
            }
        }

        fp_in.close();
    }
    
    struct PE {
        vector<int> weight_reg;
        vector<int> image_reg;
        vector<int> image_row_data;
        vector<int> output_data;
    };
    
    vector<PE> pe_vector;
    vector<int> gt_data;
    vector<int> image_data; 
    vector<int> weight_data; // 432
    vector<int> sram_tile; // 104 * 104
    vector<int> output_map;

};
#endif