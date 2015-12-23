// file sommatore.cpp
#include "sommatore_rc.hh"

// Implementazione del metodo somma dichiarato nel modulo
void sommatore_rc::somma() {

	if(is_ready.read() == 1) {
		cout.write(static_cast< sc_int<32> >( a.read() ) + static_cast< sc_int<32> >( b.read() ));
		result_isready.write(1);
	}
}