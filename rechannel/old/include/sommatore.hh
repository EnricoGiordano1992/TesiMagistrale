#ifndef SOMMATORE_H
#define SOMMATORE_H

#include <systemc.h>
//Dichiarazione del modulo
SC_MODULE(sommatore) {

	sc_in<sc_lv<32>> a, b;
	sc_in<sc_uint<1>> is_ready;  
	sc_out<sc_lv<32>> cout;
	sc_out<sc_uint<1>> result_isready;

	void somma();      //metodo che realizza la somma

	// Dichiarazione del costruttore
	SC_CTOR(sommatore) {

		// Dichiarazione di un processo di tipo
		// SC_METHOD sensibile agli ingressi
		SC_METHOD(somma);
		sensitive << a << b << is_ready;

	};
};

#endif