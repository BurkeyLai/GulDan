#include "PEArray.h"
void PEArray::pe_process() {
    //cout << "HI! " << sc_time_stamp() << endl;
    int num_kernel = 16, kernel_size = 5, image_channel = 3;
    int partial_sum = 0;
    int p = 0, w = 0;
    int row_sum = 0;
    int conv_sum = 0;
    clock_t start, finish;
    /*
    if (kernel_size == 3) {
        //cout << sc_time_stamp() << endl;
        
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer1/layer1_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer1/layer1_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer1/layer1_output.txt", 2);
        cout << "Input Map:     416 * 416 * 3" << endl;
        cout << "Kernel Size:   3 * 3" << endl;
        cout << "Kernel Number: 16" << endl;
        start = clock();
        for (int n = 0; n < 16; ++n) {
            //
            for (int h = 0; h < 416 - 2; ++h) {
                for (int w = 0; w < 416 - 2; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 3; ++i) {
                        for (int j = h; j < h + 3; ++j) {
                            for (int k = w; k < w + 3; ++k) {
                                block.push_back(image_data[i * 416 * 416 + j * 416 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 27; ++i) {
                        conv_sum += weight_data[n * 27 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        finish = clock();
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        
        //cout << (double)(finish - start) / CLOCKS_PER_SEC << endl; 
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
        
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer2/layer2_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer2/layer2_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer2/layer2_output.txt", 2);
        cout << "Input Map:     104 * 104 * 32" << endl;
        cout << "Kernel Size:   3 * 3" << endl;
        cout << "Kernel Number: 64" << endl;
        start = clock();
        for (int n = 0; n < 64; ++n) {
            //
            for (int h = 0; h < 104 - 2; ++h) {
                for (int w = 0; w < 104 - 2; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 32; ++i) {
                        for (int j = h; j < h + 3; ++j) {
                            for (int k = w; k < w + 3; ++k) {
                                block.push_back(image_data[i * 104 * 104 + j * 104 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 32 * 3 * 3; ++i) {
                        conv_sum += weight_data[n * 32 * 3 * 3 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        finish = clock();
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        
        //cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
        
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer3/layer3_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer3/layer3_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer3/layer3_output.txt", 2);
        cout << "Input Map:     52 * 52 * 64" << endl;
        cout << "Kernel Size:   3 * 3" << endl;
        cout << "Kernel Number: 128" << endl;
        start = clock();
        for (int n = 0; n < 128; ++n) {
            //
            for (int h = 0; h < 52 - 2; ++h) {
                for (int w = 0; w < 52 - 2; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 64; ++i) {
                        for (int j = h; j < h + 3; ++j) {
                            for (int k = w; k < w + 3; ++k) {
                                block.push_back(image_data[i * 52 * 52 + j * 52 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 64 * 3 * 3; ++i) {
                        conv_sum += weight_data[n * 64 * 3 * 3 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        finish = clock();
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        
        //cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
        
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer4/layer4_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer4/layer4_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model1/layer4/layer4_output.txt", 2);
        cout << "Input Map:     13 * 13 * 512" << endl;
        cout << "Kernel Size:   3 * 3" << endl;
        cout << "Kernel Number: 1024" << endl;
        start = clock();
        for (int n = 0; n < 1024; ++n) {
            //
            for (int h = 0; h < 13 - 2; ++h) {
                for (int w = 0; w < 13 - 2; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 512; ++i) {
                        for (int j = h; j < h + 3; ++j) {
                            for (int k = w; k < w + 3; ++k) {
                                block.push_back(image_data[i * 13 * 13 + j * 13 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 512 * 3 * 3; ++i) {
                        conv_sum += weight_data[n * 512 * 3 * 3 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        finish = clock();
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            //cout << output_map[o] << endl;
            //if (o == 10) {
            //    break;
            //}
            
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        
        //cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
    }
    */
    
    if (kernel_size == 5) {
        //cout << sc_time_stamp() << endl;
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer1/layer1_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer1/layer1_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer1/layer1_output.txt", 2);
        cout << "Input Map:     416 * 416 * 3" << endl;
        cout << "Kernel Size:   5 * 5" << endl;
        cout << "Kernel Number: 16" << endl;
        for (int n = 0; n < 16; ++n) {
            //
            for (int h = 0; h < 416 - 4; ++h) {
                for (int w = 0; w < 416 - 4; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 3; ++i) {
                        for (int j = h; j < h + 5; ++j) {
                            for (int k = w; k < w + 5; ++k) {
                                block.push_back(image_data[i * 416 * 416 + j * 416 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 3 * 5 * 5; ++i) {
                        conv_sum += weight_data[n * 3 * 5 * 5 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer2/layer2_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer2/layer2_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer2/layer2_output.txt", 2);
        cout << "Input Map:     104 * 104 * 32" << endl;
        cout << "Kernel Size:   5 * 5" << endl;
        cout << "Kernel Number: 64" << endl;
        for (int n = 0; n < 64; ++n) {
            //
            for (int h = 0; h < 104 - 4; ++h) {
                for (int w = 0; w < 104 - 4; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 32; ++i) {
                        for (int j = h; j < h + 5; ++j) {
                            for (int k = w; k < w + 5; ++k) {
                                block.push_back(image_data[i * 104 * 104 + j * 104 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 32 * 5 * 5; ++i) {
                        conv_sum += weight_data[n * 32 * 5 * 5 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer3/layer3_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer3/layer3_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer3/layer3_output.txt", 2);
        cout << "Input Map:     52 * 52 * 64" << endl;
        cout << "Kernel Size:   5 * 5" << endl;
        cout << "Kernel Number: 128" << endl;
        for (int n = 0; n < 128; ++n) {
            //
            for (int h = 0; h < 52 - 4; ++h) {
                for (int w = 0; w < 52 - 4; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 64; ++i) {
                        for (int j = h; j < h + 5; ++j) {
                            for (int k = w; k < w + 5; ++k) {
                                block.push_back(image_data[i * 52 * 52 + j * 52 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 64 * 5 * 5; ++i) {
                        conv_sum += weight_data[n * 64 * 5 * 5 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer4/layer4_weight.txt", 0);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer4/layer4_input.txt", 1);
        file_process("/media/mec-lab/新增磁碟區/LBX/課程/人工智慧晶片設計與應用/example/project1_new/SystemC_Dram/DRAM_INPUT/model2/layer4/layer4_output.txt", 2);
        cout << "Input Map:     13 * 13 * 512" << endl;
        cout << "Kernel Size:   5 * 5" << endl;
        cout << "Kernel Number: 1024" << endl;
        for (int n = 0; n < 1024; ++n) {
            //
            for (int h = 0; h < 13 - 4; ++h) {
                for (int w = 0; w < 13 - 4; ++w) {
                    vector<int> block;
                    for (int i = 0; i < 512; ++i) {
                        for (int j = h; j < h + 5; ++j) {
                            for (int k = w; k < w + 5; ++k) {
                                block.push_back(image_data[i * 13 * 13 + j * 13 + k]);
                            }
                        }
                    }
                    conv_sum = 0;
                    for (int i = 0; i < 512 * 5 * 5; ++i) {
                        conv_sum += weight_data[n * 512 * 5 * 5 + i] * block[i];
                    }
                    //cout << conv_sum << endl;
                    output_map.push_back(conv_sum);
                    //data_out = conv_sum;
                    block.clear();
                }
            }
        }
        
        cout << "Comparing Output..." << endl;
        for (int o = 0; o < output_map.size(); ++o) {
            //cout << output_map[o] << endl;
            if (output_map[o] != gt_data[o]) {
                cout << output_map[o] << " and " << gt_data[o] << " are different!" << endl;
                break;
            }
            //cout << output_map[o] << endl;
            //if (o == 10) {
            //    break;
            //}
            
            if (o == output_map.size() - 1) {
                cout << "****************" << endl;
                cout << "*** Correct! ***" << endl;
                cout << "****************" << endl;
            }
        }
        output_map.clear();
        image_data.clear();
        weight_data.clear();
        gt_data.clear();
        cout << "-----------------------------" << endl;
        //cout << sc_time_stamp() << endl;
    }
    
}

/*
        for (int c = 0; c < 3; ++c) {
            for (int h = 0; h < IMAGE_ROW; ++h) {
                for (int w = 0; w < IMAGE_ROW; ++w) {
                    sram_tile.push_back(image_data.at(c * IMAGE_ROW * IMAGE_ROW + h * IMAGE_ROW * 4 + w));
                }
            }
            
            for (int n = 0; n < pe_vector.size(); ++n) {
                for (int i = 0; i < IMAGE_ROW - 2; ++i) {
                    pe_vector[n].output_data.resize(IMAGE_ROW - 2, 0);
                    for (int j = 0; j < kernel_size; ++j) {
                        for (int k = 0; k < kernel_size; ++k) {
                            pe_vector[n].weight_reg[k] = weight_data[n * kernel_size * kernel_size * image_channel + j * kernel_size + k];
                        }
                        
                        for (int k = 0; k < IMAGE_ROW; ++k) {
                            pe_vector[n].image_row_data[k] = sram_tile[(i + j) * IMAGE_ROW + k];
                        }
                        for (int k = 0; k < IMAGE_ROW - 2; ++k) {
                            row_sum = 0;
                            if (k == 0) {
                                for (int l = 0; l < kernel_size; ++l) {
                                    pe_vector[n].image_reg[l] = pe_vector[n].image_row_data[l];
                                }
                            } else {
                                pe_vector[n].image_reg[0] = pe_vector[n].image_reg[1];
                                pe_vector[n].image_reg[1] = pe_vector[n].image_reg[2];
                                pe_vector[n].image_reg[2] = pe_vector[n].image_row_data[k + 2];
                            }
                            for (int l = 0; l < kernel_size; ++l) {
                                row_sum += pe_vector[n].image_reg[l] * pe_vector[n].weight_reg[l];
                            }
                            pe_vector[n].output_data[k] += row_sum;
                        }
                    }
                    
                    for (int j = 0; j < pe_vector[0].output_data.size(); ++j) {
                        //cout << pe_vector[0].output_data[j] << endl;
                        output_map[n * 414 * 414 + i * 414 + j] += pe_vector[0].output_data[j];
                    }
                    pe_vector[n].output_data.clear();
                }
                //break;
            }
        }
        */