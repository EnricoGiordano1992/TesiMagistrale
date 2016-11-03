#include <systemc.h>
#include <ReChannel.h>
#include "specularity.h"

// The port map for automatic binding of the reconfigurable modules:

RC_PORTMAP(
    dynPortMap, ___RC_MODULES_PORT_N___,            // name and size of the port map
    RC_PORT_TYPES(            // a list of port types (order is relevant)
___RC_MODULES_PORT_TYPE___
    )
);

// note: both input fifos are read before output is produced
___RC_RECONFIGURABLE_MODULES_CONSTRUCTORS___
