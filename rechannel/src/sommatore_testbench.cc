#include "sommatore_testbench.hh"

sommatore_testbench::sommatore_testbench(sc_module_name name_)
: sc_module(name_)
{

	SC_THREAD(run);
	sensitive << result;
}

void sommatore_testbench::run()
{

	int numero1, numero2, new_result = 0;

	cout<<"Calcolo la somma tra 2 e 3"<<endl;

	numero1 = 2;
	numero2 = 3;

	cout << endl;
	cout << "\t " << numero1 << " \t + " << endl;
	cout << "\t " << numero2 << " \t = " << endl;
	cout << "-----------------------" << endl;

	number_a.write(numero1);
	number_b.write(numero2);

	isready.write(1);
	//wait();

	while(result_isready.read() != 1) wait();
	new_result = (result.read()).to_int();
	cout << "\t " << new_result << endl;

	isready.write(0);
	number_a.write(0);
	number_b.write(0);

	sc_stop();
}


