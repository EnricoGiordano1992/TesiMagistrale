#include "sommatore_testbench.hh"
#include "sommatore.hh"
#include "ReChannel.h"

int sc_main(int argc, char **argv)
{
    
  sc_signal < sc_uint<1> >      is_ready;
  sc_signal<sc_lv<32> >   p_In_data1;
  sc_signal<sc_lv<32> >   p_In_data2;
  sc_signal<sc_uint<1> >  p_In_enable;
  sc_signal<sc_uint<1> >  p_Out_enable;
  sc_signal<sc_lv<32> >   p_result;
  p_Out_enable.write(0);
  
  sommatore_testbench        i_sommatore_testbench("sommatore_testbench");             // source module
  sommatore                  i_sommatore("sommatore");    		// root RTL module

  i_sommatore.a(p_In_data1);
  i_sommatore.b(p_In_data2);
  i_sommatore.is_ready(is_ready);
  i_sommatore.cout(p_result);
  i_sommatore.result_isready(p_Out_enable);
  
  i_sommatore_testbench.isready(is_ready);
  i_sommatore_testbench.number_a(p_In_data1);
  i_sommatore_testbench.number_b(p_In_data2);
  i_sommatore_testbench.result(p_result);
  i_sommatore_testbench.result_isready(p_Out_enable);
  
  sc_start();

  return 0;
};

