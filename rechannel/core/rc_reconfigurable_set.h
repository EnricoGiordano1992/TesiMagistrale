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
#ifndef RC_RECONFIGURABLE_SET_H_
#define RC_RECONFIGURABLE_SET_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

class rc_reconfigurable;

class rc_reconfigurable_set
    : private std::set<rc_reconfigurable*>
{
    friend class rc_control;

private:
    typedef std::set<rc_reconfigurable*> base_type;

public:
    typedef base_type::iterator       iterator;
    typedef base_type::const_iterator const_iterator;

public:
    rc_reconfigurable_set() { }

    rc_reconfigurable_set(const rc_reconfigurable_set& reconf_set)
        : base_type(reconf_set)
    { }

    inline rc_reconfigurable_set& operator=(
        const rc_reconfigurable_set& reconf_set)
        { base_type::operator=(reconf_set); return *this; }

    inline int size() const
        { return (int)base_type::size(); }

    inline void insert(rc_reconfigurable& reconf)
        { base_type::insert(&reconf); }

    inline void insert(const rc_reconfigurable_set& reconf_set)
        { base_type::insert(reconf_set.begin(), reconf_set.end()); }

    inline void erase(rc_reconfigurable& reconf)
        { base_type::erase(&reconf); }

    void erase(const rc_reconfigurable_set& reconf_set);

    inline bool contains(const rc_reconfigurable& reconf) const
    {
        return (base_type::find(const_cast<rc_reconfigurable*>(&reconf))
            != base_type::end());
    }

    bool contains(const rc_reconfigurable_set& reconf_set) const;

    inline iterator find(rc_reconfigurable& reconf)
        { return base_type::find(&reconf); }

    inline const_iterator find(const rc_reconfigurable& reconf) const
    {
        return base_type::find(
            const_cast<rc_reconfigurable*>(&reconf));
    }

    inline iterator begin()
        { return base_type::begin(); }

    inline const_iterator begin() const
        { return base_type::begin(); }

    inline iterator end()
        { return base_type::end(); }

    inline const_iterator end() const
        { return base_type::end(); }

    inline rc_reconfigurable_set& operator+=(
        rc_reconfigurable& reconf)
        { this->insert(reconf); return *this; }

    inline rc_reconfigurable_set& operator+=(
        const rc_reconfigurable_set& reconf_set)
        { this->insert(reconf_set); return *this; }

    inline rc_reconfigurable_set operator+(
        rc_reconfigurable& reconf) const
        { return (rc_reconfigurable_set(*this) += reconf); }

    inline rc_reconfigurable_set operator+(
        const rc_reconfigurable_set& reconf_set) const
        { return (rc_reconfigurable_set(*this) += reconf_set); }

    inline rc_reconfigurable_set& operator-=(
        rc_reconfigurable& reconf)
        { this->erase(reconf); return *this; }

    inline rc_reconfigurable_set& operator-=(
        const rc_reconfigurable_set& reconf_set)
        { this->erase(reconf_set); return *this; }

    inline rc_reconfigurable_set operator-(
        rc_reconfigurable& reconf) const
        { return (rc_reconfigurable_set(*this) -= reconf); }

    inline rc_reconfigurable_set operator-(
        const rc_reconfigurable_set& reconf_set) const
        { return (rc_reconfigurable_set(*this) -= reconf_set); }
};

/* global functions */

inline rc_reconfigurable_set operator+(
    rc_reconfigurable& dyn_object1, rc_reconfigurable& dyn_object2)
{
    return ((rc_reconfigurable_set() += dyn_object1) += dyn_object2);
}
inline rc_reconfigurable_set operator+(
    rc_reconfigurable& reconf,
    const rc_reconfigurable_set& reconf_set)
{
    return (rc_reconfigurable_set(reconf_set) += reconf);
}

} // namespace ReChannel

#endif // RC_RECONFIGURABLE_SET_H_

//
// $Id: rc_reconfigurable_set.h,v 1.7 2007/12/20 20:39:40 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reconfigurable_set.h,v $
//

