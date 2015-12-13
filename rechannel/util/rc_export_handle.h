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
#ifndef RC_EXPORT_HANDLE_H_
#define RC_EXPORT_HANDLE_H_

#include <systemc.h>

#include "ReChannel/util/rc_hash_map.h"
#include "ReChannel/util/rc_report.h"

namespace ReChannel {

namespace internals {
namespace export_handle {

class export_ref
{
public:
    virtual sc_interface* get_interface() const = 0;

    virtual sc_export_base& get_export_base() const = 0;

protected:
    virtual ~export_ref() { }
};

template<class IF>
class export_ref_t
    : virtual public export_ref
{
public:
    export_ref_t(sc_export<IF>& export_)
        : p_export(export_)
        { }
    virtual sc_interface* get_interface() const
        { return p_export.get_interface(); }
    virtual sc_export_base& get_export_base() const
        { return p_export; }

private:
    mutable sc_export<IF>& p_export;
};

} // namespace export_handle
} // namespace internals

class rc_export_handle // (copyable)
{
private:
    typedef internals::export_handle::export_ref      export_ref;
    typedef rc_hash_map<sc_export_base*, export_ref*> exportmap;

public:
    rc_export_handle()
        : p_export_base(NULL), p_export_ref(NULL)
        { }

    template<class IF> rc_export_handle(sc_export<IF>& export_);

    bool valid() const
        { return (p_export_base != NULL); }

    void reset()
        { p_export_base = NULL; p_export_ref = NULL; }

    sc_interface* get_interface() const
        { return p_export_ref->get_interface(); }

    sc_object* get_object() const
        { return p_export_base; }

    sc_export_base* operator->() const
        { return p_export_base; }

    sc_export_base* operator*() const
        { return p_export_base; }

    operator sc_export_base*() const
        { return p_export_base; }

    operator sc_object*() const
        { return p_export_base; }

    bool operator==(const rc_export_handle& other) const
        { return p_export_base == other.p_export_base; }

    bool operator<(const rc_export_handle& other) const
        { return p_export_base < other.p_export_base; }

private:
    sc_export_base* p_export_base;
    export_ref*     p_export_ref;

private:
    static exportmap p_exportmap;
};

/* template code */

template<class IF>
rc_export_handle::rc_export_handle(sc_export<IF>& export_)
    : p_export_base(&export_)
{
    // find the map element for the export (create it if not exists)
    export_ref*& expref = p_exportmap[&export_];
    // was export_ not contained in the map?
    if (expref == NULL) {
        // store new export_ref_ in the map
        expref =
            new internals::export_handle::export_ref_t<IF>(export_);
    }
    // let p_export_ref point to the current export
    p_export_ref = expref;
}

} // namespace ReChannel

#endif // RC_EXPORT_HANDLE_H_

//
// $Id: rc_export_handle.h,v 1.7 2007/12/20 20:33:00 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_export_handle.h,v $
//
