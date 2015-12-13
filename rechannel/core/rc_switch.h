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
#ifndef RC_SWITCH_H_
#define RC_SWITCH_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/util/rc_object_handle.h"

namespace ReChannel {

class rc_reconfigurable;
class rc_switch_connector_base;
class rc_interface_filter;

class rc_switch
{
    friend class rc_reconfigurable;
    friend class rc_switch_connector_base;

public:

    enum state_type { UNDEF=0, OPEN, CLOSED };

    enum { STATE_COUNT=3 };

    typedef std::vector<rc_interface_filter*> filter_chain;

public:

    virtual std::string get_switch_kind() const = 0;

    virtual std::string get_switch_name() const = 0;

    virtual state_type get_switch_state() const = 0;

    virtual unsigned int get_transaction_count() const = 0;

    virtual sc_interface* get_static_interface() const = 0;

    virtual sc_interface* get_dynamic_interface() const = 0;

    virtual rc_reconfigurable* get_current_reconfigurable() const = 0;

    virtual bool is_locked() const = 0;

    virtual bool is_registered(const rc_reconfigurable& reconf) const = 0;

    virtual bool is_registered(const sc_interface& dyn_if) const = 0;

protected:

    virtual void bind_static_object(
        const rc_object_handle& obj_to_bind) = 0;

    virtual void bind_dynamic_object(
        const rc_object_handle& obj_to_bind) = 0;

    virtual void open() = 0;

    virtual void open(
        rc_reconfigurable& reconf,
        const filter_chain& filters = filter_chain()) = 0;

    virtual void close() = 0;

    virtual void set_undefined() = 0;

    virtual void refresh_notify() = 0;

    virtual void register_reconfigurable(
        rc_reconfigurable& reconf, sc_interface& dyn_if) = 0;

    virtual void unregister_reconfigurable(
        rc_reconfigurable& reconf) = 0;

    virtual sc_interface* get_registered_interface(
        rc_reconfigurable& reconf) const = 0;

    virtual bool is_lock_owner(const rc_reconfigurable& reconf) const = 0;

    virtual bool set_locked(rc_reconfigurable& lock_owner, bool lock) = 0;

    virtual ~rc_switch() {}
};

}

#endif //RC_SWITCH_H_

//
// $Id: rc_switch.h,v 1.14 2007/12/20 20:40:53 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_switch.h,v $
//
