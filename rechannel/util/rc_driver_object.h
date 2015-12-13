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
#ifndef RC_DRIVER_OBJECT_H_
#define RC_DRIVER_OBJECT_H_

#include <vector>
#include <systemc.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/type_traits/add_const.hpp> 
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_void.hpp>

#include "ReChannel/util/rc_throwable.h"

namespace ReChannel {

namespace internals {
namespace driver_object {

template<class R, bool IS_REF = boost::is_reference<R>::value>
class retval;

template<class R>
class copy_retval;

class nb_driver_access_call;

template<class R>
class driver_access_call;

} // namespace driver_object
} // namespace internals

class rc_driver_object_base
{
    friend class internals::driver_object::nb_driver_access_call;
    template<class R>
    friend class internals::driver_object::driver_access_call;

protected:
    struct write_call;
    typedef boost::function<void (void)>      call_type;
    typedef std::vector<write_call>           write_call_vector;
    typedef std::vector<write_call>::iterator write_call_iterator;

protected:
    rc_driver_object_base();

    virtual ~rc_driver_object_base();

protected:
    virtual void writer_method_proc();

    virtual void writer_thread_proc();

    static void safe_event_notify(sc_event& e);

private:
    static void s_writer_method_proc();
    static sc_process_handle s_create_method_process();

protected:
    sc_process_handle  m_hproc;
    write_call_vector* m_write_call_vector; // write call queue
    sc_event*          m_pending_write_event;
    bool               m_pending_write_event_notified_flag;
    bool*              m_pending_write_event_notified;

protected:

    static sc_process_handle s_hproc;
    static write_call_vector s_write_call_vector; // write call queue
    static sc_event          s_pending_write_event;
    static bool              s_pending_write_event_notified_flag;

protected:
    struct write_call // is used to queue write calls
    {
        inline write_call(call_type call_);
        inline write_call(
            call_type call_, sc_event* write_done_event);
        inline write_call(
            call_type call_, sc_event* write_done_event,
            rc_throwable** throw_dest);

        call_type m_call;
        sc_event* m_write_done_event; // only exists until being notified!
        rc_throwable** m_throw_dest; // destination to forward a throw
    };

private:
    // disabled
    rc_driver_object_base(const rc_driver_object_base& dobj);
    rc_driver_object_base& operator=(const rc_driver_object_base& dobj);
};

template<class IF, bool IS_THREAD> class rc_driver_object_b;

template<class IF>
class rc_driver_object_b<IF, false>
    : public rc_driver_object_base
{
protected:
    // typedefs
    class driver_access;
    typedef rc_driver_object_b<IF, false>  this_type;
    typedef rc_driver_object_base          base_type;

    typedef base_type::write_call          write_call;
    typedef base_type::call_type           call_type;
    typedef base_type::write_call_vector   write_call_vector;
    typedef base_type::write_call_iterator write_call_iterator;

public:
    rc_driver_object_b(IF& channel);

    IF& get_channel()
        { return *p_channel;    }
    void set_channel(IF& channel)
        { p_channel = &channel; }

    bool has_written() const
        { return m_write_call_vector->empty(); }

    void cancel()
    {
#       if !defined(RC_USE_SHARED_METHOD_DRIVER)
            m_write_call_vector->clear();
#       endif
    }

private:
    IF* p_channel;

public:
    driver_access access;

private:
    // disabled
    rc_driver_object_b(const this_type& dobj);
    this_type& operator=(const this_type& dobj);
};

template<class IF>
class rc_driver_object_b<IF, true>
    : public rc_driver_object_base
{
protected:
    // typedefs
    typedef rc_driver_object_b<IF, true> this_type;
    typedef rc_driver_object_base        base_type;

    typedef base_type::write_call          write_call;
    typedef base_type::call_type           call_type;
    typedef base_type::write_call_vector   write_call_vector;
    typedef base_type::write_call_iterator write_call_iterator;

public:
    class driver_access;

public:
    rc_driver_object_b(IF& channel);

    IF& get_channel()
        { return *p_channel;    }
    void set_channel(IF& channel)
        { p_channel = &channel; }

    bool has_written() const
        { return m_write_call_vector->empty(); }
    void cancel()
        { m_write_call_vector->clear(); }

private:
    IF* p_channel;

public:
    driver_access access;

private:
    // disabled
    rc_driver_object_b(const this_type& dobj);
    this_type& operator=(const this_type& dobj);
};

template<class IF>
class rc_driver_object
    : public rc_driver_object_b<IF, true>
{
public:
    // useful typedefs
    typedef rc_driver_object<IF>               this_type;
    typedef rc_driver_object_b<IF, true>       base_type;
    typedef typename base_type::driver_access  driver_access;
public:
    rc_driver_object(IF& channel)
        : base_type(channel)
    { }
};

template<class IF>
class rc_nb_driver_object
    : public rc_driver_object_b<IF, false>
{
public:
    // useful typedefs
    typedef rc_nb_driver_object<IF>           this_type;
    typedef rc_driver_object_b<IF, false>     base_type;
    typedef typename base_type::driver_access driver_access;
public:
    rc_nb_driver_object(IF& channel)
        : base_type(channel)
    { }
};

/* inline code */

inline rc_driver_object_base::write_call::write_call(
    rc_driver_object_base::call_type call_)
    : m_call(call_), m_write_done_event(NULL), m_throw_dest(NULL)
{ }

inline rc_driver_object_base::write_call::write_call(
    rc_driver_object_base::call_type call_,
    sc_event* write_done_event)
    : m_call(call_), m_write_done_event(write_done_event),
      m_throw_dest(NULL)
{ }

inline rc_driver_object_base::write_call::write_call(
    rc_driver_object_base::call_type call_,
    sc_event* write_done_event, rc_throwable** throw_dest)
    : m_call(call_), m_write_done_event(write_done_event),
      m_throw_dest(throw_dest)
{ }

/* template code */

template<class IF>
rc_driver_object_b<IF, false>::rc_driver_object_b(IF& channel)
    : base_type(), p_channel(&channel), access(*this)
{
#if !defined(RC_USE_SHARED_METHOD_DRIVER)
    // create a driver method process
    std::auto_ptr<write_call_vector> wc_vector(new write_call_vector());
    std::auto_ptr<sc_event> pw_event(new sc_event());
    m_write_call_vector = wc_vector.get();
    m_pending_write_event = pw_event.get();
    m_pending_write_event_notified = &m_pending_write_event_notified_flag;
    sc_spawn_options opt;
    opt.set_sensitivity(m_pending_write_event);
    opt.dont_initialize();
    opt.spawn_method();
    m_hproc = sc_spawn(
        sc_bind(&this_type::writer_method_proc, this),
        sc_gen_unique_name("_rc_nb_driver_object_proc"),
        &opt);
    wc_vector.release(); // (note: vector will be deleted by destructor)
    pw_event.release();  // (note: event will be deleted by destructor)
#endif // !defined(RC_USE_SHARED_METHOD_DRIVER)
}

template<class IF>
rc_driver_object_b<IF, true>::rc_driver_object_b(IF& channel)
    : base_type(), p_channel(&channel), access(*this)
{
    // create a driver thread process
    std::auto_ptr<write_call_vector> wc_vector(new write_call_vector());
    std::auto_ptr<sc_event> pw_event(new sc_event());
    m_write_call_vector = wc_vector.get();
    m_pending_write_event = pw_event.get();
    m_pending_write_event_notified = &m_pending_write_event_notified_flag;
    sc_spawn_options opt;
    opt.set_sensitivity(m_pending_write_event);
    opt.dont_initialize();
    m_hproc = sc_spawn(
        sc_bind(&this_type::writer_thread_proc, this),
        sc_gen_unique_name("_rc_driver_object_proc"), &opt);
    wc_vector.release(); // (note: vector will be deleted by destructor)
    pw_event.release();  // (note: event will be deleted by destructor)
}

/* template parameter dependent implementation variants */

// non-blocking driver access
template<class IF>
class rc_driver_object_b<IF, false>::driver_access
{
    friend class rc_driver_object_b<IF, false>;

protected:
    typedef rc_driver_object_b<IF, false> driver_object_type;
    typedef typename internals::driver_object::nb_driver_access_call
        nb_driver_access_call;

public:
    /* non-blocking call without return value and no ref. parameters */
    template<class IF_>
    inline void call(void (IF_::*method)()) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel));
    }

    template<class IF_>
    inline void call(void (IF_::*method)() const) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel));
    }

    template<class A1, class A1_, class IF_>
    inline void call(void (IF_::*method)(A1_), A1 a1) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel, a1));
    }

    template<class A1, class A1_, class IF_>
    inline void call(void (IF_::*method)(A1_) const, A1 a1) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel, a1));
    }

    template<class A1, class A2, class A1_, class A2_, class IF_>
    inline void call(void (IF_::*method)(A1_, A2_), A1 a1, A2 a2) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel, a1, a2));
    }

    template<class A1, class A2, class A1_, class A2_, class IF_>
    inline void call(void (IF_::*method)(A1_, A2_) const,
        A1 a1, A2 a2) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel, a1, a2));
    }

    template<class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_), A1 a1, A2 a2, A3 a3) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel, a1, a2, a3));
    }

    template<class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_) const,
        A1 a1, A2 a2, A3 a3) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel, a1, a2, a3));
    }

    template<class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_),
        A1 a1, A2 a2, A3 a3, A4 a4) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4));
    }

    template<class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_) const,
        A1 a1, A2 a2, A3 a3, A4 a4) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4));
    }

    template<class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5));
    }

    template<class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7));
    }

    template<class A1, class A2, class A3, class A4, class A5,
        class A6, class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7, a8));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7, a8));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline void call(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline void call(
        void (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline void call(
        void (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline void call(
        void (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    {
        nb_driver_access_call()(
            dobj, boost::bind(method, dobj.p_channel,
            a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }

private:
    driver_access(driver_object_type& dobj_)
        : dobj(dobj_)
    { }

private:
    driver_object_type& dobj;

private:
    //disabled
    driver_access(const driver_access& access);
    driver_access& operator=(const driver_access& access);
};

// blocking driver access
template<class IF>
class rc_driver_object_b<IF, true>::driver_access
{
    friend class rc_driver_object_b<IF, true>;

protected:
    typedef rc_driver_object_b<IF, true> driver_object_type;

public:
    /* blocking call */

    template<class R, class IF_>
    inline R call(R (IF_::*method)()) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(method, dobj.p_channel));
    }

    template<class R, class IF_>
    inline R call(R (IF_::*method)() const) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(method, dobj.p_channel));
    }

    template<class R, class A1, class A1_, class IF_>
    inline R call(R (IF_::*method)(A1_), A1 a1) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(method, dobj.p_channel, a1));
    }

    template<class R, class A1, class A1_, class IF_>
    inline R call(R (IF_::*method)(A1_) const, A1 a1) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(method, dobj.p_channel, a1));
    }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R call(R (IF_::*method)(A1_, A2_), A1 a1, A2 a2) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2));
    }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R call(R (IF_::*method)(A1_, A2_) const, A1 a1, A2 a2) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2));
    }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_), A1 a1, A2 a2, A3 a3) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3));
    }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_) const, A1 a1, A2 a2, A3 a3) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3));
    }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_),
        A1 a1, A2 a2, A3 a3, A4 a4) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4));
    }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_) const,
        A1 a1, A2 a2, A3 a3, A4 a4) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4));
    }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5));
    }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5, a6));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5, a6));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5, a6, a7));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5, a6, a7));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5, a6, a7, a8));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel, a1, a2, a3, a4, a5, a6, a7, a8));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R call(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
        const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel,
                a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R call(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
        const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel,
                a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R call(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel,
                a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R call(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    {
        return typename internals::driver_object::driver_access_call<R>()(
            dobj, boost::bind(
                method, dobj.p_channel,
                a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }

private:
    driver_access(driver_object_type& dobj_)
        : dobj(dobj_)
    { }

private:
    driver_object_type& dobj;

private:
    //disabled
    driver_access(const driver_access& access);
    driver_access& operator=(const driver_access& access);
};

namespace internals {
namespace driver_object {

template<class R>
class copy_retval
{
public:
    typedef typename boost::add_const<R>::type src_type;
    typedef retval<R, boost::is_reference<R>::value> dest_type;

    void operator()(src_type src, dest_type* dest) const
        { (*dest) = src; }
};

// R is not a reference
template<class R>
class retval<R, false>
{
    typedef retval<R, false> this_type;
    typedef typename boost::add_const<R>::type const_value_type;

public:
    inline retval() {}
    inline operator R() const { return value; }
    inline this_type& operator=(const_value_type new_value)
    {
        value = new_value; /*copy*/
        return *this;
    }
private:
    R value;
};

// R is a reference
template<class R>
class retval<R, true>
{
    typedef retval<R, true> this_type;
    typedef typename boost::add_const<R>::type const_value_type;
    typedef typename boost::remove_reference<R>::type* value_ptr_type;
public:
    inline retval() : value_ptr(NULL) {}
    inline operator R() const { return *value_ptr; }
    inline this_type& operator=(const_value_type new_value)
    {
        value_ptr = boost::addressof(new_value);
        return *this;
    }
private:
     value_ptr_type value_ptr;
};

class nb_driver_access_call
{
private:
    typedef ::ReChannel::rc_driver_object_base
        driver_object_type;
    typedef driver_object_type::call_type  call_type;
    typedef driver_object_type::write_call write_call;

public:
    inline nb_driver_access_call() {}

    // TODO: should it be inlined?
    void operator()(driver_object_type& dobj, call_type call_obj);
};

// R is not void
template<class R>
class driver_access_call
{
private:
    typedef typename ::ReChannel::rc_driver_object_base
        driver_object_type;
    typedef typename driver_object_type::write_call
        write_call;
    typedef typename internals::driver_object::retval<R>
        retval;
    typedef typename internals::driver_object::copy_retval<R>
        copy_retval;

public:
    inline driver_access_call() {}

    // TODO: should it be inlined?
    template<class BIND_OBJ>
    R operator()(driver_object_type& dobj, BIND_OBJ call_obj) const;
};

template<class R>
template<class BIND_OBJ>
R driver_access_call<R>::operator()(
    driver_object_type& dobj, BIND_OBJ call_obj) const
{
    // the returned value will be written to this variable
    retval ret;
    // create the event that will indicate the execution of the call
    sc_event write_done_event;
    // initialise the thrown_obj to NULL
    rc_throwable* thrown_obj = NULL;
    // create the write call and append it to the write call vector
    // copy retval has to be used to copy the returned value to variable ret
    dobj.m_write_call_vector->push_back(
        write_call(
            boost::bind(
                copy_retval(), call_obj, &ret),
            &write_done_event, &thrown_obj));
    // signal the driver object that a new write call is pending
    //   by notifying the pending write event
    if (!*dobj.m_pending_write_event_notified) {
        *dobj.m_pending_write_event_notified = true;
        rc_driver_object_base::safe_event_notify(
            *dobj.m_pending_write_event);
    }
    // wait for the call to be executed
    ::sc_core::wait(write_done_event);
    // if an event of type rc_throwable occured rethrow it in this context
    if (thrown_obj != NULL) {
        ::ReChannel::rc_throw(thrown_obj);
    }
    // execution was successful
    // return ret
    return (R)ret;
}

// R is void
template<>
class driver_access_call<void>
{
private:
    typedef ::ReChannel::rc_driver_object_base
        driver_object_type;
    typedef driver_object_type::call_type  call_type;
    typedef driver_object_type::write_call write_call;

public:
    inline driver_access_call() {}

    // TODO: should it be inlined?
    void operator()(driver_object_type& dobj, call_type call_obj) const;
};

} // namespace driver_object
} // namespace internals

} // namespace ReChannel

#endif // RC_DRIVER_OBJECT_H_

//
// $Id: rc_driver_object.h,v 1.15 2007/12/20 23:07:10 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_driver_object.h,v $
//
