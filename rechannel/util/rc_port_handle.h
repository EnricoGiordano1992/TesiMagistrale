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
#ifndef RC_PORT_HANDLE_H_
#define RC_PORT_HANDLE_H_

#include <systemc.h>

#include "ReChannel/util/rc_hash_map.h"
#include "ReChannel/util/rc_report.h"

namespace ReChannel {

namespace internals {
namespace port_handle {

class port_ref
{
public:
    virtual sc_interface* get_interface() const = 0;

    virtual sc_port_base& get_port_base() const = 0;

protected:
    virtual ~port_ref() { }
};

template<class PORT>
class port_ref_t
    : virtual public port_ref
{
public:
    port_ref_t(PORT& port_)
        : p_port(port_)
        { }
    virtual sc_interface* get_interface() const
        { return p_port.get_interface(); }
    virtual sc_port_base& get_port_base() const
        { return p_port; }

private:
    mutable PORT& p_port;
};

} // namespace port_handle
} // namespace internals

class rc_port_handle // (copyable)
{
private:
    typedef internals::port_handle::port_ref      port_ref;
    typedef rc_hash_map<sc_port_base*, port_ref*> portmap;

public:
    rc_port_handle()
        : p_port_base(NULL), p_port_ref(NULL)
        { }

    template<class IF, int N, sc_port_policy POL>
    rc_port_handle(sc_port<IF, N, POL>& port_);

    bool valid() const
        { return (p_port_base != NULL); }

    void reset()
        { p_port_base = NULL; p_port_ref = NULL; }

    sc_interface* get_interface() const
        { return p_port_ref->get_interface(); }

    sc_object* get_object() const
        { return p_port_base; }

    sc_port_base* operator->() const
        { return p_port_base; }

    sc_port_base* operator*() const
        { return p_port_base; }

    operator sc_port_base*() const
        { return p_port_base; }

    operator sc_object*() const
        { return p_port_base; }

    bool operator==(const rc_port_handle& other) const
        { return p_port_base == other.p_port_base; }

    bool operator<(const rc_port_handle& other) const
        { return p_port_base < other.p_port_base; }

private:
    sc_port_base* p_port_base;
    port_ref*     p_port_ref;

private:
    static portmap p_portmap;
};

/* template code */

template<class IF, int N, sc_port_policy POL>
rc_port_handle::rc_port_handle(sc_port<IF, N, POL>& port_)
    : p_port_base(&port_)
{
    typedef sc_port<IF, N, POL> port_type;
    // find the map element for the port (create it if not exists)
    port_ref*& pref = p_portmap[&port_];
    // is port_ not contained in the map?
    if (pref == NULL) {
        // create the port's storage element and write it to the map
        pref = new internals::port_handle::port_ref_t<port_type>(port_);
    }
    // let p_port_ref point to the current port
    p_port_ref = pref;
}

} // namespace ReChannel

#endif // RC_PORT_HANDLE_H_

//
// $Id: rc_port_handle.h,v 1.5 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_port_handle.h,v $
//
