#include "Top.h"
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

___TOP_PRE_CODE___

void Top::activate(rc_reconfigurable& module)
{
    if (active_module != &module) {
        if (active_module != NULL) {
          #ifdef DEBUG_ACTIVATION_TOP
            RC_COUTL("Top: unloading module"
                " '" << active_module->rc_get_name() << "'..."
                " (t=" << sc_time_stamp() << ")");
          #endif
            ctrl.unload(*active_module);

            #ifdef DEBUG_ACTIVATION_TOP
            RC_COUTL("Top: module '"
                << active_module->rc_get_name() << "' unloaded."
                " (t=" << sc_time_stamp() << ")\n");
            #endif
        }

        #ifdef DEBUG_ACTIVATION_TOP
        RC_COUTL("Top: activating module"
            " '" << module.rc_get_name() << "'..."
            " (t=" << sc_time_stamp() << ")");
        #endif

        ctrl.activate(module);
        active_module = &module;

        #ifdef DEBUG_ACTIVATION_TOP
        RC_COUTL("Top: module '"
            << module.rc_get_name() << "' activated."
            " (t=" << sc_time_stamp() << ")");
        #endif
    }
}

void Top::producer_proc()
{
    ___TOP_PRODUCER_VARIABLES___

    ___TOP_PRODUCER_CODE___
}

void Top::control_proc()
{
    ___TOP_CONTROLLER_VARIABLES___

    ___TOP_CONTROLLER_CODE___
}

void Top::monitor_proc()
{
    ___TOP_MONITOR_VARIABLES___

    ___TOP_MONITOR_CODE___
}
