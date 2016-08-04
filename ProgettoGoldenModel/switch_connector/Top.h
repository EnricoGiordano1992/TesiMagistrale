#include <systemc.h>
#include <ReChannel.h>
#include "specularity.h"

/*
 * The reconfigurable modules and the dynamic side's port map
 * are declared in this file:
 */
#include "modules.h"

// The port map for automatic binding of the static side:
// (note: exports and channels can be mapped, too)

RC_PORTMAP(
    staticPortMap, 4,     // name and size of the port map
    RC_PORT_TYPES(        // a list of port/export or channel types
        sc_fifo<int>,     // channel 0
        sc_fifo<int>,     // channel 1
        sc_fifo<int>,     // channel 2
        sc_fifo<int>      // channel 3
    )
);

SC_MODULE(Top)
{
    bool command_executed = false;
    // channels
    sc_fifo<int> in1_fifo;
    sc_fifo<int> in2_fifo;
    sc_fifo<int> out1_fifo;
    sc_fifo<int> out2_fifo;

    // portals
    rc_portal<sc_fifo_in<int> >  in1_portal;
    rc_portal<sc_fifo_in<int> >  in2_portal;
    rc_portal<sc_fifo_out<int> > out1_portal;
    rc_portal<sc_fifo_out<int> > out2_portal;

    // reconfiguration control
    rc_control ctrl;

    // reconfigurable modules
    Lying_configuration m1;
    Erect_configuration m2;

    // port map for static side binding
    staticPortMap stat_portmap;

    // switch connector
    rc_switch_connector<dynPortMap> connector;

    STATES STATUS, NEXT_STATUS;
    ROBOT_CONFIG robot_config;

    SC_CTOR(Top)
        : in1_fifo(1), in2_fifo(1), out1_fifo(1), out2_fifo(1),
          ctrl("ctrl"), m1("Lying_configuration"), m2("Erect_configuration"),
          // initialise the static port map
          stat_portmap(in1_fifo, in2_fifo, out1_fifo, out2_fifo),
          // initialise the switch connector
          connector("connector", in1_portal, in2_portal, out1_portal, out2_portal),
          active_module(NULL)
    {
        // automatically connect portals with the static side
        connector.bind_static(stat_portmap);

        // automatically connect portals with the dynamic side
        connector.bind_dynamic(m1);
        connector.bind_dynamic(m2);

        // set module's loading times
        m1.rc_set_delay(RC_LOAD, sc_time(100, SC_NS));
        m2.rc_set_delay(RC_LOAD, sc_time(100, SC_NS));

        // setup the reconfiguration controller
        ctrl.add(m1+m2);

        // m1 shall be initially active
        ctrl.activate(m1);
        active_module = &m1;

        // producer thread sending the test data
        SC_THREAD(producer_proc);

        // reconfiguration control
        SC_THREAD(control_proc);

        // monitors the output of the modules
        SC_THREAD(monitor_proc);
    }

private:

    // unloads currently active module, activates given module
    void activate(rc_reconfigurable& module);

    // returns a random number in the range [0..99]
    inline int rnd100()
        { return rand() % 100; }

    void producer_proc();

    void control_proc();

    void monitor_proc();

private:

    // currently active reconfigurable module
    rc_reconfigurable* active_module;
};
