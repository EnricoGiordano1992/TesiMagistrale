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
#ifndef RC_RESETTABLE_H_
#define RC_RESETTABLE_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

class rc_reconfigurable;

class rc_resettable
{
    friend class rc_reconfigurable;

protected:
    virtual void rc_on_reset() = 0;

    virtual void rc_on_init_resettable() = 0;

    virtual ~rc_resettable() {}
};

} // namespace ReChannel

#define RC_ON_RESET() \
    virtual void rc_on_reset()

#define RC_ON_INIT_RESETTABLE() \
    virtual void rc_on_init_resettable()

#endif // RC_RESETTABLE_H_

//
// $Id: rc_resettable.h,v 1.10 2007/10/14 05:29:39 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_resettable.h,v $
//
