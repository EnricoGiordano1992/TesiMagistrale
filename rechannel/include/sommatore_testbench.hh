#ifndef SOMMATORE_TESTBENCH_H
#define SOMMATORE_TESTBENCH_H

#define PERIOD 10

#include <systemc.h>

class sommatore_testbench : public sc_core::sc_module
{

private:

	void run();

public:

	//flag di input per sapere se sono pronti i numeri da sommare
	sc_out<sc_uint<1> >    isready;

	//numeri di input da moltiplicare
	sc_out<sc_lv<32>>   	number_a;
	sc_out<sc_lv<32>>	  	number_b;

	//flag di output per sapere quando e' pronto l'output
	sc_in<sc_uint<1> >   result_isready;

	sc_in<sc_lv<32>>  		result;

	SC_HAS_PROCESS(sommatore_testbench);
	sommatore_testbench(sc_module_name name);

};

#endif
