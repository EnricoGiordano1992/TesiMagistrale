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
    staticPortMap, ___RC_TOP_PORT_N___,     // name and size of the port map
    RC_PORT_TYPES(        // a list of port/export or channel types
      ___RC_TOP_PORT_TYPE___
    )
);

SC_MODULE(Top)
{
    bool command_executed = false;
    // channels
    ___RC_TOP_CHANNELS_PORT_TYPE___

    // portals
    ___RC_TOP_PORTAL_PORT_TYPE___

    // reconfiguration control
    rc_control ctrl;

    // reconfigurable modules
    ___RC_RECONFIGURABLE_MODULES_DEF___

    // port map for static side binding
    staticPortMap stat_portmap;

    // switch connector
    rc_switch_connector<dynPortMap> connector;

    STATES STATUS, NEXT_STATUS;
    ___RC_TOP_VARIABLES___

    SC_CTOR(Top)
        : ___RC_CTOR_INITIALIZER___
    {
        // automatically connect portals with the static side
        connector.bind_static(stat_portmap);

        // automatically connect portals with the dynamic side
        ___RC_BINDING_DYNAMIC_CONNECTOR___

        // set module's loading times
        ___RC_LOADING_LOADING_TIMES___

        // setup the reconfiguration controller
        ___RC_ADD_MODULES___

        // m1 shall be initially active
        ___RC_FIRST_MODULE___

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

    void producer_proc();

    void control_proc();

    void monitor_proc();

private:

    // currently active reconfigurable module
    rc_reconfigurable* active_module;
};
