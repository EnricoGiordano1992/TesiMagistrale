#include <systemc.h>
#include <ReChannel.h>
#include "specularity.h"

// The port map for automatic binding of the reconfigurable modules:

RC_PORTMAP(
    dynPortMap, 4,            // name and size of the port map
    RC_PORT_TYPES(            // a list of port types (order is relevant)
		sc_fifo<int>,
		sc_fifo<int>,
		sc_fifo<int>,
		sc_fifo<int>,

    )
);

// note: both input fifos are read before output is produced
RC_RECONFIGURABLE_MODULE(Lying_configuration)
{
	sc_fifo<int> in1;
	sc_fifo<int> in2;
	sc_fifo<int> out1;
	sc_fifo<int> out2;
	STATES STATUS, NEXT_STATUS; 
	dynPortMap pm;
	RC_RECONFIGURABLE_CTOR(Lying_configuration),
		pm(out2) // initialise the port map with the ports
		{
		rc_add_portmap(pm);
		RC_THREAD(proc);
		}private:
    void proc();
};
RC_RECONFIGURABLE_MODULE(Erect_configuration)
{
	sc_fifo<int> in1;
	sc_fifo<int> in2;
	sc_fifo<int> out1;
	sc_fifo<int> out2;
	STATES STATUS, NEXT_STATUS; 
	dynPortMap pm;
	RC_RECONFIGURABLE_CTOR(Lying_configuration),
		pm(out2) // initialise the port map with the ports
		{
		rc_add_portmap(pm);
		RC_THREAD(proc);
		}private:
    void proc();
};

