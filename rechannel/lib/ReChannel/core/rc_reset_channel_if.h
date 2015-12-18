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
#ifndef RC_RESET_CHANNEL_IF_H_
#define RC_RESET_CHANNEL_IF_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_process.h"

namespace ReChannel {

namespace internals {
class process_support;
} // namespace internals

class rc_reset_channel_if
    : virtual public sc_interface
{
    friend class internals::process_support;

protected:
    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const = 0;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const = 0;

    virtual bool get_current_level() const = 0;

    virtual const sc_signal<bool>* get_underlying_reset_signal() const = 0;
};

namespace internals {

class reset_signal
    : public sc_signal<bool>,
      virtual public rc_reset_channel_if
{
private:
    typedef sc_signal<bool> base_type;
    typedef std::set<rc_process_control*> pctrl_set;

public:
    reset_signal(const char* name_=sc_gen_unique_name("signal"))
        : base_type(name_)
    {}

    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    operator const bool&() const
        { return this->read(); }

    reset_signal& operator= (const bool& value)
        { this->write(value); return *this; }

    reset_signal& operator= (const reset_signal& buffer_)
        { this->write(buffer_); return *this; }

protected:
    virtual bool get_current_level() const
        { return this->read(); }

    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return this; }

    virtual void update();

private:
    mutable pctrl_set p_pctrl_set[2];
};

class reset_buffer
    : public sc_buffer<bool>,
      virtual public rc_reset_channel_if
{
private:
    typedef sc_buffer<bool> base_type;
    typedef std::set<rc_process_control*> pctrl_set;

public:
    reset_buffer(const char* name_=sc_gen_unique_name("buffer"))
        : base_type(name_)
    {}

    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    operator const bool&() const
        { return this->read(); }

    reset_buffer& operator= (const bool& value)
        { this->write(value); return *this; }

    reset_buffer& operator= (const reset_buffer& buffer_)
        { this->write(buffer_); return *this; }

protected:
    virtual bool get_current_level() const
        { return this->read(); }

    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return this; }

    virtual void update();

private:
    mutable pctrl_set p_pctrl_set[2];
};

} // namespace internals

} // namespace ReChannel

#endif // RC_RESET_CHANNEL_IF_H_

//
// $Id: rc_reset_channel_if.h,v 1.5 2007/10/09 21:47:01 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reset_channel_if.h,v $
//
