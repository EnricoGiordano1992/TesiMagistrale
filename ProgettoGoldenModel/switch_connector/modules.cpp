#include "modules.h"

void Lying_configuration::proc()
{
    int i = 0; // initialise counter i
    while(true) {
        int data1 = in1.read();
        // start transaction after first data has been read
        RC_TRANSACTION {
            ++i;
            RC_COUTL("Lying_configuration: 1. input read: " << data1
                << " (t=" << sc_time_stamp() << ")");
            RC_COUTL("Lying_configuration: START WORKING #" << i
                << " (t=" << sc_time_stamp() << ")");
            int data2 = in2.read();
            RC_COUTL("Lying_configuration: 2. input read: " << data2
                << " (t=" << sc_time_stamp() << ")");
            int maximum = (data1 >= data2 ? data1 : data2);
            wait(10, SC_NS);
            out1.write(maximum);
            out2.write(maximum);
            RC_COUTL("Lying_configuration: FINISHED #" << i
                << " (t=" << sc_time_stamp() << ")");
        } // end transaction
        // continue while loop
    }
}
