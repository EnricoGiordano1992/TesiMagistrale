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
#ifndef RC_TRANSACTION_COUNTER_H_
#define RC_TRANSACTION_COUNTER_H_

#include "ReChannel/core/rc_reconfigurable.h"

namespace ReChannel {

class rc_transaction_counter
    : public sc_object
{
private:
    typedef rc_transaction_counter this_type;

public:

    rc_transaction_counter();

    explicit rc_transaction_counter(std::vector<this_type*>& children);

    inline int count() const
        { return p_count; }

    void modify(int amount);

    void reset();

    ~rc_transaction_counter()
        { this->reset(); }

    inline rc_reconfigurable* get_reconfigurable() const
        { return p_reconf; }

    inline int get_children_count() const
        { return (int)p_children.size(); }

    int get_relay_factor(int index) const;

    void set_relay_factor(int index, int factor);

    void set_relay_factor(rc_transaction_counter& child, int factor);

/* convenience operators */

    inline const this_type& operator++()
        { this->modify(+1); return *this; }

    inline const this_type& operator--()
        { this->modify(-1); return *this; }

    inline const this_type& operator+=(int amount)
        { this->modify(amount); return *this; }

    inline const this_type& operator-=(int amount)
        { this->modify(-amount); return *this; }

/* convenience constructors */

    explicit rc_transaction_counter(this_type& tc1, int factor1);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7, this_type& tc8, int factor8);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7, this_type& tc8, int factor8,
        this_type& tc9, int factor9);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7, this_type& tc8, int factor8,
        this_type& tc9, int factor9, this_type& tc10, int factor10);

private:

    void _rc_relay(int amount);

    void _rc_add_child(rc_transaction_counter& child, int factor=1);

private:

    rc_reconfigurable*                       p_reconf;
    int                                      p_count;
    std::vector<std::pair<this_type*, int> > p_children;

private:
    // disabled
    rc_transaction_counter(const this_type& orig);
    this_type& operator=(const this_type& orig);
};

} // namespace ReChannel

#endif //RC_TRANSACTION_COUNTER_H_

//
// $Id: rc_transaction_counter.h,v 1.11 2007/12/20 20:42:13 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_transaction_counter.h,v $
//
