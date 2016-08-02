#include "Top.h"

void Top::activate(rc_reconfigurable& module)
{
    if (active_module != &module) {
        if (active_module != NULL) {
            RC_COUTL("Top: unloading module"
                " '" << active_module->rc_get_name() << "'..."
                " (t=" << sc_time_stamp() << ")");

            ctrl.unload(*active_module);

            RC_COUTL("Top: module '"
                << active_module->rc_get_name() << "' unloaded."
                " (t=" << sc_time_stamp() << ")\n");
        }

        RC_COUTL("Top: activating module"
            " '" << module.rc_get_name() << "'..."
            " (t=" << sc_time_stamp() << ")");

        ctrl.activate(module);
        active_module = &module;

        RC_COUTL("Top: module '"
            << module.rc_get_name() << "' activated."
            " (t=" << sc_time_stamp() << ")");
    }
}

void Top::producer_proc()
{
    for (int i=0; i < 50; ++i) {
        int x = rnd100();
        int y = rnd100();
        in1_fifo.write(x);
        in2_fifo.write(y);
        RC_COUTL("Top: input written (" << x << ", " << y << ")"
            " (t=" << sc_time_stamp() << ")");
        wait(10, SC_NS);
    }
}

void Top::control_proc()
{
    wait(32, SC_NS);
    activate(m1);
}

void Top::monitor_proc()
{
    int i=0;
    while(true) {
        ++i;
        int x = out1_fifo.read();
        int y = out2_fifo.read();
        RC_COUTL("Top: " << i << ". output read (" << x << ", " << y << ")"
            " (t=" << sc_time_stamp() << ")");
    }
}
