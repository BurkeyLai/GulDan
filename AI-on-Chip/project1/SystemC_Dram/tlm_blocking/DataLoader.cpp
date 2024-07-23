#include "DataLoader.h"

void DataLoader::master_access(uint32_t addr, int data, int length) {
    tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
    tlm::tlm_command cmd = tlm::TLM_WRITE_COMMAND;
    sc_time delay = sc_time(0, SC_NS);
    trans->set_command( cmd );
    trans->set_address( addr * 4 );
    trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
    trans->set_data_length( length );
    trans->set_streaming_width( length ); // = data_length to indicate no streaming
    trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
    trans->set_dmi_allowed( false ); // Mandatory initial value
    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value
    socket->b_transport( *trans, delay );  // Blocking transport call
}

void DataLoader::file_process(char *file_name) {
    ifstream fp_in;
    fp_in.open(file_name, ios::in | ios::binary);
    if (fp_in.is_open()) {
        cout << "File opened for reading." << endl;
        fp_in.seekg(0, ios::beg); // put file pointer to beginning of file
    } else {
        cout << "Unable to open " << file_name << "." << endl;
        //break;
    }
    string line;
    //int img_begin_addr = addressCounter;
    while (getline(fp_in, line)) {
        data = atoi(line.c_str());
        //cout << "Address: " << addressCounter * 4 << " data: " << data << "\n";
        master_access(addressCounter, data, 1);
        addressCounter++;
    }
    //int img_end_addr = addressCounter;
    fp_in.close();
}

void DataLoader::thread_process() {
    loading_finished = false;
    Layer L;
    L.weight_file = "/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer1/layer1_weight.txt";
    L.input_file = "/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer1/layer1_input.txt";
    //L.output_file = "/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer1/layer1_output.txt";
    layers.push_back(L);

    for (int i = 0; i < layers.size(); ++i) {
        file_process(layers[i].weight_file);
        file_process(layers[i].input_file);
        //file_process(layers[i].output_file);
    }
    loading_finished = true;
}