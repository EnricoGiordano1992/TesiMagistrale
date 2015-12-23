#ifndef SOMMATORE_RC_H
#define SOMMATORE_RC_H

#include <systemc.h>
#include <ReChannel.h>

RC_MODULE(sommatore_rc) {

	

	rc_reset<int>(i,0);// reset i to 0
	rc_preserve<sc_signal<int> > (j); // preserve j

	set_loading_time(sc_time(20,SC_MS)); // module needs 20 milliseconds to load
	set_activation_time((sc_time(1.5,SC_MS)); // and 1.5 milliseconds to activate
	set_removal_time(sc_time(2,SC_MS)); // preserving variables takes some time

}

#endif