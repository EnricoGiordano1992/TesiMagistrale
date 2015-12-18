// vim:set et sts=4 ts=4 tw=75 sw=4 ai ci cin cino=g0,t0:
/*
 * Copyright (C) 2007, Technical Computer Science Group,
 *                     University of Bonn
 *
 * This file is part of the ReChannel library.
 *
 * The ReChannel library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Andreas Raabe and Armin Felke. Implementation by Armin Felke.
 *          {raabe, felke}@cs.uni-bonn.de
 */
namespace ReChannel {
} // namespace ReChannel

// RC_MAKE_DOXYGEN_DOCS shall be defined if the
// doxygen documentation is created.

#if defined(RC_MAKE_DOXYGEN_DOCS)
/* The available compiler switches. */

#define RC_DISABLE_COUT

#define RC_USE_NON_OSCI_KERNEL

#define RC_DISABLE_CTHREAD_MOBILITY

#define RC_USE_SHARED_METHOD_DRIVER

#define RC_SIGNAL_WRITE_CHECK_DISABLED

#define RC_SYNTHESIS_MODE
#endif // defined(RC_MAKE_DOXYGEN_DOCS)

#ifndef RECHANNEL_INCLUDED
#define RECHANNEL_INCLUDED

#if !defined(RC_SYNTHESIS_MODE)

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_process.h"
#include "ReChannel/core/rc_interface_filter.h"
#include "ReChannel/core/rc_switch.h"
#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_reconfigurable_module.h"
#include "ReChannel/core/rc_transaction_counter.h"
#include "ReChannel/core/rc_control.h"
#include "ReChannel/core/rc_portmap.h"
#include "ReChannel/core/rc_switch_connector.h"

#include "ReChannel/util/rc_throwable.h"
#include "ReChannel/util/rc_report.h"
#include "ReChannel/util/rc_driver_object.h"
#include "ReChannel/util/rc_port_traits.h"

#include "ReChannel/communication/accessors/rc_accessor.h"
#include "ReChannel/communication/accessors/rc_signal_accessors.h"
#include "ReChannel/communication/accessors/rc_fifo_accessors.h"
#include "ReChannel/communication/accessors/rc_mutex_accessor.h"
#include "ReChannel/communication/accessors/rc_semaphore_accessor.h"
#include "ReChannel/communication/accessors/rc_event_queue_accessor.h"

#include "ReChannel/communication/portals/rc_portal.h"
#include "ReChannel/communication/portals/rc_signal_portals.h"
#include "ReChannel/communication/portals/rc_resolved_portals.h"
#include "ReChannel/communication/portals/rc_rv_portals.h"
#include "ReChannel/communication/portals/rc_fifo_portals.h"
#include "ReChannel/communication/portals/rc_mutex_portal.h"
#include "ReChannel/communication/portals/rc_semaphore_portal.h"
#include "ReChannel/communication/portals/rc_event_queue_portal.h"

#include "ReChannel/communication/exportals/rc_exportal.h"
#include "ReChannel/communication/exportals/rc_signal_exportals.h"
#include "ReChannel/communication/exportals/rc_fifo_exportals.h"
#include "ReChannel/communication/exportals/rc_mutex_exportal.h"
#include "ReChannel/communication/exportals/rc_semaphore_exportal.h"
#include "ReChannel/communication/exportals/rc_event_queue_exportal.h"

#include "ReChannel/communication/filters/rc_abstract_filter.h"
#include "ReChannel/communication/filters/rc_abstract_prim_filter.h"
#include "ReChannel/communication/filters/rc_fifo_filters.h"
#include "ReChannel/communication/filters/rc_mutex_filter.h"
#include "ReChannel/communication/filters/rc_semaphore_filter.h"

#include "ReChannel/components/rc_components.h"

using namespace ReChannel;

#else

#include "ReChannel/synthesis_header.h"

#endif 

#endif //RECHANNEL_INCLUDED

//
// $Id: ReChannel.h,v 1.17 2007/12/20 20:43:59 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel.h,v $
//

