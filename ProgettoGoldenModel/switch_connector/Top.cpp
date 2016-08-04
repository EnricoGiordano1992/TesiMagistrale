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

    while(1){
      STATUS = S_INIT;
      in1_fifo.write(STATUS);
      command_executed = false;
      RC_COUTL("Top: input written (" << STATUS << ")"
          " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
      wait(1000, SC_MS);

      while(!command_executed);
      STATUS = S_STOP;
      in1_fifo.write(STATUS);
      command_executed = false;
      RC_COUTL("Top: input written (" << STATUS << ")"
      " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
      wait(1000, SC_MS);

      for (int i=0; i < 5; ++i) {
          while(!command_executed);
          STATUS = S_WALK;
          in1_fifo.write(STATUS);
          command_executed = false;
          RC_COUTL("Top: input written (" << STATUS << ")"
          " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
          wait(1000, SC_MS);
      }

      while(!command_executed);

      STATUS = S_STOP;
      in1_fifo.write(STATUS);
      command_executed = false;
      RC_COUTL("Top: input written (" << STATUS << ")"
      " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
      wait(1000, SC_MS);

      while(!command_executed);
      STATUS = S_RECONFIG;
      in1_fifo.write(STATUS);
      command_executed = false;
      RC_COUTL("Top: input written (" << STATUS << ")"
      " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
      wait(1000, SC_MS);
    }
}

void Top::control_proc()
{
    while(1){
      activate(m1);
      robot_config = LYING;
      while(STATUS == S_RECONFIG)
        wait(1000, SC_MS);
      while(STATUS != S_RECONFIG)
        wait(1000, SC_MS);
      activate(m2);
      robot_config = ERECT;
      while(STATUS == S_RECONFIG)
        wait(1000, SC_MS);
      while(STATUS != S_RECONFIG)
        wait(1000, SC_MS);
    }
}

void Top::monitor_proc()
{
    int i=0;
    while(true) {
        ++i;
        int x = out1_fifo.read();
//        int y = out2_fifo.read();
        RC_COUTL("Top: " << i << ". output read (" << x << ")"
            " (t=" << sc_time_stamp() << ")");
        if(x == static_cast<int>(STATUS))
          command_executed = true;
    }
}
