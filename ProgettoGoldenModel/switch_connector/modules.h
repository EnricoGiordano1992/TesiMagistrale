#include <systemc.h>
#include <ReChannel.h>
#include "specularity.h"

// The port map for automatic binding of the reconfigurable modules:

RC_PORTMAP(
    dynPortMap, 4,            // name and size of the port map
    RC_PORT_TYPES(            // a list of port types (order is relevant)
        sc_fifo_in<int>,      // port 0
        sc_fifo_in<int>,      // port 1
        sc_fifo_out<int>,     // port 2
        sc_fifo_out<int>      // port 3
    )
);

// note: both input fifos are read before output is produced
RC_RECONFIGURABLE_MODULE(Lying_configuration)
{
    rc_fifo_in<int>  in1;
    rc_fifo_in<int>  in2;
    rc_fifo_out<int> out1;
    rc_fifo_out<int> out2;

    STATES STATUS, NEXT_STATUS;

    dynPortMap pm;

    RC_RECONFIGURABLE_CTOR(Lying_configuration),
        pm(in1, in2, out1, out2) // initialise the port map with the ports
    {
        // add the port map to the module
        rc_add_portmap(pm);

        RC_THREAD(proc);
    }

private:
    void proc();
};



RC_RECONFIGURABLE_MODULE(Erect_configuration)
{
    rc_fifo_in<int>  in1;
    rc_fifo_in<int>  in2;
    rc_fifo_out<int> out1;
    rc_fifo_out<int> out2;

    STATES STATUS, NEXT_STATUS;

    dynPortMap pm;

    RC_RECONFIGURABLE_CTOR(Erect_configuration),
        pm(in1, in2, out1, out2) // initialise the port map with the ports
    {
        // add the port map to the module
        rc_add_portmap(pm);

        RC_THREAD(proc);
    }

private:
    void proc();
};
