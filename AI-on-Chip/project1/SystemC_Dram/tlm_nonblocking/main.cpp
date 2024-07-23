//#include "top_module.h"

#include "DRAM_wrapper.h"
#include "DRAM.h"

int sc_main(int argc, char* argv[])
{
  DRAM_wrapper *dram_wrapper; //initiator
  DRAM    *dram;   //target

//signal
  sc_signal<sc_uint<ADDR_LENGTH>> m_addr;
  sc_signal<sc_uint<DATA_LENGTH>> wr_data;
  sc_signal<sc_uint<DATA_LENGTH>> data_out;
  sc_signal<sc_uint<1>> wr_enable;

  dram_wrapper = new DRAM_wrapper("dram_wrapper");
  dram = new DRAM("dram");

  dram_wrapper->dram_addr_i(m_addr);
  dram_wrapper->data(wr_data);
  dram_wrapper->wr_enable(wr_enable);
  dram_wrapper->dram_dataout_i(data_out);
  dram_wrapper->socket.bind(dram->socket);

//write data to memory
  m_addr.write(0x00000008);
  wr_data.write(0x0C);
  wr_enable.write(1);
  sc_start(500, SC_NS);
  cout << "Address: 0x00000000 " << "data: " << dram->mem[0] << "\n";
  cout << "Address: 0x00000004 " << "data: " << dram->mem[1] << "\n";
  cout << "Address: 0x00000008 " << "data: " << dram->mem[2] << "\n";

//read data from memory
  m_addr.write(0x00000008);
  wr_enable.write(0);
  sc_start(500, SC_NS);
  cout << "Address: 0x00000008 " << "data: " << data_out << "\n";
  cout << "\n*****    Finish     *****\n";

  return 0;
}
