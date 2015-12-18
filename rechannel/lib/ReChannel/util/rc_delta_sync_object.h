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
#ifndef RC_DELTA_SYNC_OBJECT_H_
#define RC_DELTA_SYNC_OBJECT_H_

#include <systemc.h>
#include <boost/function.hpp>

namespace ReChannel {

class rc_delta_sync_object
    : protected sc_prim_channel
{
public:
    typedef boost::function<void (rc_delta_sync_object&)>
        delta_sync_callback_type;

public:
    rc_delta_sync_object();

    rc_delta_sync_object(
        delta_sync_callback_type callback, bool enabled=false);

    virtual void request_update();

    inline bool is_enabled() const
        { return p_enabled; }

    inline void set_enabled(bool b)
        { p_enabled = b; }

    void set_callback(delta_sync_callback_type callback);

protected:
    virtual void update();

private:
    bool                     p_enabled;
    delta_sync_callback_type p_callback;

private:
    // disabled
    rc_delta_sync_object(const rc_delta_sync_object& orig);
    rc_delta_sync_object& operator=(const rc_delta_sync_object& orig);
};

} // namespace ReChannel

#endif // RC_DELTA_SYNC_OBJECT_H_

//
// $Id: rc_delta_sync_object.h,v 1.5 2007/10/24 23:55:23 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_delta_sync_object.h,v $
//
