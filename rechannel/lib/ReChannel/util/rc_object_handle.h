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
#ifndef RC_OBJECT_HANDLE_H_
#define RC_OBJECT_HANDLE_H_

#include "ReChannel/util/rc_port_handle.h"
#include "ReChannel/util/rc_export_handle.h"

namespace ReChannel {

class rc_object_handle // (copyable)
{
public:
    rc_object_handle();

    rc_object_handle(sc_object& obj_);

    rc_object_handle(const rc_port_handle& port_);

    template<class IF, int N, sc_port_policy POL>
    rc_object_handle(sc_port<IF, N, POL>& port_);

    rc_object_handle(const rc_export_handle& export_);

    template<class IF>
    rc_object_handle(sc_export<IF>& export_);

public:
    rc_object_handle& operator=(sc_object& obj_);

    template<class IF, int N, sc_port_policy POL>
    rc_object_handle& operator=(sc_port<IF, N, POL>& port_);

    rc_object_handle& operator=(const rc_port_handle& port_);

    template<class IF>
    rc_object_handle& operator=(sc_export<IF>& export_);

    rc_object_handle& operator=(const rc_export_handle& export_);

    inline bool valid() const
        { return (p_object != NULL); }

    inline bool is_port() const
        { return p_port.valid(); }

    inline bool is_export() const
        { return p_export.valid(); }

    inline bool is_channel() const
        { return p_channel_if != NULL; }

    inline bool is_comm_object() const
        { return (is_channel() || is_port() || is_export()); }

    inline rc_port_handle get_port() const
        { return p_port; }

    inline rc_export_handle get_export() const
        { return p_export; }

    sc_interface* get_interface() const;

    inline sc_object* get_object() const
        { return p_object; }

    sc_object* operator->() const
        { return p_object; }

    sc_object* operator*() const
        { return p_object; }

    inline operator sc_object*() const
        { return p_object; }

    bool operator==(const rc_object_handle& other) const
        { return p_object == other.p_object; }

    bool operator<(const rc_object_handle& other) const
        { return p_object < other.p_object; }

private:
    sc_object*       p_object;
    sc_interface*    p_channel_if;
    rc_port_handle   p_port;
    rc_export_handle p_export;
};

/* template code */

template<class IF, int N, sc_port_policy POL>
rc_object_handle::rc_object_handle(sc_port<IF, N, POL>& port_)
    : p_object(static_cast<sc_object*>(&port_)), p_channel_if(NULL),
      p_port(port_)
{ }

template<class IF>
rc_object_handle::rc_object_handle(sc_export<IF>& export_)
    : p_object(static_cast<sc_object*>(&export_)), p_channel_if(NULL),
      p_export(export_)
{ }

template<class IF, int N, sc_port_policy POL>
rc_object_handle& rc_object_handle::operator=(sc_port<IF, N, POL>& port_)
{
    p_object = static_cast<sc_object*>(&port_);
    p_channel_if = NULL;
    p_port = port_;
    p_export.reset();
    return *this;
}

template<class IF>
rc_object_handle& rc_object_handle::operator=(sc_export<IF>& export_)
{
    p_object = static_cast<sc_object*>(&export_);
    p_channel_if = NULL;
    p_port.reset();
    p_export = export_;
    return *this;
}

} // namespace ReChannel

#endif // RC_PORT_HANDLE_H_

//
// $Id: rc_object_handle.h,v 1.3 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_object_handle.h,v $
//
