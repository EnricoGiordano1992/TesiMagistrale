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
#ifndef RC_RECONFIGURABLE_H_
#define RC_RECONFIGURABLE_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_switch.h"
#include "ReChannel/core/rc_resettable.h"
#include "ReChannel/core/rc_process.h"
#include "ReChannel/core/rc_reconfigurable_set.h"
#include "ReChannel/core/rc_portmap.h"
#include "ReChannel/core/rc_switch_connector.h"

#include "ReChannel/util/rc_driver_object.h"
#include "ReChannel/util/rc_mutex_object.h"
#include "ReChannel/util/rc_delta_sync_object.h"
#include "ReChannel/util/rc_hash_map.h"
#include "ReChannel/util/rc_object_handle.h"
#include "ReChannel/util/rc_port_handle.h"
#include "ReChannel/util/rc_export_handle.h"

namespace ReChannel {

class rc_control;
class rc_transaction_counter;

// forward declarations
class rc_reconfigurable;
rc_reconfigurable* rc_get_reconfigurable_context();
rc_reconfigurable* rc_get_reconfigurable_context(sc_object* start_search);

namespace internals {
namespace reconfigurable {

class temporary_object : public sc_object {};

class begin_construction
{
    friend void end_construction(begin_construction& constr);
    friend rc_reconfigurable* ::ReChannel::rc_get_reconfigurable_context();
    friend rc_reconfigurable* ::ReChannel::rc_get_reconfigurable_context(
        sc_object* start_search);

protected:
    explicit begin_construction(rc_reconfigurable& reconf);

    void finish();

    void finish(sc_object& reconf);

    ~begin_construction()
        { finish(); }

private:
    rc_reconfigurable* p_previous;
    bool               p_finished;

private:
    static rc_reconfigurable* s_current;

private:
    //disabled
    begin_construction(const begin_construction& reconf);
    begin_construction& operator=(const begin_construction& reconf);
};

inline void end_construction(begin_construction& constr)
    { constr.finish(); }

} // namespace reconfigurable
} // namespace internals

class rc_reconfigurable
{
    friend class rc_control;

    friend class internals::reconfigurable::begin_construction;

    friend class rc_transaction;

    friend class rc_transaction_counter;

public:
    typedef rc_switch_connector_base switch_conn_type;

    typedef rc_switch::filter_chain  filter_chain;

private:
    typedef std::pair<rc_switch*, rc_object_handle> switch_commobj_pair;

    typedef boost::multi_index_container<
              switch_commobj_pair,
              boost::multi_index::indexed_by<
                boost::multi_index::ordered_unique<
                  BOOST_MULTI_INDEX_MEMBER(
                    switch_commobj_pair, rc_switch*, first)
                >,
                boost::multi_index::ordered_non_unique<
                  BOOST_MULTI_INDEX_MEMBER(
                    switch_commobj_pair, rc_object_handle, second)
                >
              >
            > switch_commobj_map;

    typedef switch_commobj_map::nth_index<0>::type   switch_commobj_index;

    typedef switch_commobj_map::nth_index<1>::type   commobj_switch_index;

    typedef std::pair<
                switch_commobj_index::iterator,
                switch_commobj_index::iterator>      switch_commobj_range;

    typedef std::pair<
                commobj_switch_index::iterator,
                commobj_switch_index::iterator>      commobj_switch_range;

    typedef std::set<rc_interface_filter*>           filter_set;

    typedef std::map<rc_object_handle, filter_chain> commobj_filters_map;

    typedef std::set<rc_resettable*>                 resettable_set;

    typedef std::vector<rc_portmap_base*>            portmap_vector;

public:
    enum state_type { UNLOADED=0, INACTIVE, ACTIVE };

    enum { STATE_COUNT=3 };

    enum action_type { UNLOAD=0, LOAD, ACTIVATE, DEACTIVATE };

    enum { ACTION_COUNT=4 };

protected:

    rc_reconfigurable(sc_object* this_=NULL);

public:

    std::string rc_get_name() const;

    const sc_object* rc_get_object() const;

    sc_object* rc_get_object();

    inline state_type rc_get_state() const
        { return p_state; }

    inline bool rc_is_state_changing() const
        { return (p_state != p_next_state); }

    inline state_type rc_get_next_state() const
        { return p_next_state; }

    inline bool rc_is_loaded() const
        { return (p_state != UNLOADED); }

    inline bool rc_is_active() const
        { return (p_state == ACTIVE); }

    inline int rc_get_transaction_count() const
        { return p_transaction_count; }

    void bind(rc_switch_connector_base& switch_connector);

    void rc_register_switch(
        rc_switch& switch_obj, const rc_object_handle& bound_obj);

    bool rc_is_registered(rc_switch& switch_obj) const;

    void rc_register_resettable(rc_resettable& resettable);

    bool rc_is_registered(rc_resettable& resettable) const;

    inline void rc_begin_transaction()
        { ++p_transaction_count; }

    inline void rc_end_transaction();

    void rc_possible_deactivation();

    void rc_possible_deactivation_delta();

    inline bool rc_is_deactivation_requested() const
        { return (p_next_state == INACTIVE && p_state == ACTIVE); }

    void rc_set_delay(action_type a, sc_time t);

    inline sc_time rc_get_delay(action_type a) const
        { return p_action_delays[a]; }

    inline void rc_set_default_delay(action_type a, sc_time t)
        { p_action_default_delays[a] = t; }

    inline sc_time rc_get_default_delay(action_type a) const
        { return p_action_default_delays[a]; }

    inline operator const rc_reconfigurable_set&() const
        { return p_self_set; }

    inline const rc_process_control& rc_get_process_control() const
        { return p_pctrl; }

    inline switch_conn_type* rc_get_current_switch_connector() const
        { return p_curr_switch_conn; }

    virtual ~rc_reconfigurable();

protected:

    virtual void rc_on_load() {}

    virtual void rc_on_activate() {}

    virtual void rc_on_deactivate() {}

    virtual void rc_on_unload() {}

    void rc_add_portmap(rc_portmap_base& portmap);

    void rc_clear_portmaps();

    bool rc_is_compatible(const rc_portmap_base& portmap) const
        { return (this->_rc_is_compatible(portmap) > -1); }

    void rc_add_filter(
        const rc_object_handle& commobj_, rc_interface_filter& filter_);

    int rc_get_filter_count(const rc_object_handle& commobj_) const;

    filter_chain rc_get_filter_chain(
        const rc_object_handle& commobj_) const;

    bool rc_has_filter(rc_interface_filter& filter_) const;

    void rc_refresh_notify(const rc_object_handle& commobj_);

    void rc_refresh_notify_all();

private:

    static void get_all_reconfigurables(
        std::vector<rc_reconfigurable*>& copy_dest)
        { copy_dest = s_reconfigurables; }

private:
    inline rc_control* get_control() const
        { return p_control; }

    void set_control(rc_control* control)
        { p_control = control; }

    void start_of_simulation();

    void reconfigure(state_type new_state);

    void change_switch_state(rc_switch::state_type new_state);

    void move(switch_conn_type& target);

    inline bool has_lock() const
        { return p_mutex.has_lock(); }

    inline bool is_locked() const
        { return p_mutex.is_locked(); }

    inline const sc_event& get_lock_release_event() const
        { return p_mutex.get_lock_release_event(); }

    inline bool trylock()
        { return p_mutex.trylock(); }

    inline void lock()
        { p_mutex.lock(); }

    inline bool lock(sc_time timeout)
        { return p_mutex.lock(timeout); }

    bool unlock();

    bool share_lock(sc_process_handle proc);

    bool reset_lock_share();

    inline bool has_lock_share() const
        { return (p_lock_share == sc_get_current_process_handle()); }

    bool lock_switches(bool report_error=false);

    void unlock_switches();

    void reset_transaction_count();

    rc_portmap_base& get_portmap(int index);

    int get_portmap_count() const
        { return p_portmap_vector.size(); }

private:

/* for internal use only */

    void _rc_set_sc_object(sc_object& reconf);

    void _rc_delta_sync_state_change(rc_delta_sync_object& delta_sync);

    void _rc_unload();

    void _rc_load();

    void _rc_activate();

    void _rc_deactivate();

    void _rc_reset();

    int _rc_is_compatible(const rc_portmap_base& portmap) const;

    rc_portmap_base* _rc_get_compatible_portmap(
        const rc_switch_connector_base& switch_conn) const;

private:
    static std::vector<rc_reconfigurable*> s_reconfigurables;

private:
    mutable sc_object*    p_sc_object;
    mutable bool          p_is_no_sc_object;

    state_type            p_state;
    state_type            p_next_state;
    int                   p_transaction_count;

    rc_process_control    p_pctrl;
    rc_delta_sync_object  p_delta_sync;

    switch_commobj_map    p_switch_commobj_map;
    switch_commobj_index& p_switch_commobj_index;
    commobj_switch_index& p_commobj_switch_index;

    filter_set            p_filter_set;
    commobj_filters_map   p_commobj_filters_map;

    rc_control*           p_control;
    rc_reconfigurable_set p_self_set;
    rc_mutex_object       p_mutex;
    sc_process_handle     p_lock_share;
    sc_time               p_action_default_delays[ACTION_COUNT];
    sc_time               p_action_delays[ACTION_COUNT];

    portmap_vector        p_portmap_vector;
    switch_conn_type*     p_curr_switch_conn;

    resettable_set        p_resettable_set;
};

class rc_transaction
{
public:
    inline explicit rc_transaction(rc_reconfigurable* reconf);
    inline void begin();
    inline void end();
    inline bool has_ended() const
        { return p_has_ended; }
    ~rc_transaction();
private:
    // disabled
    rc_transaction(const rc_transaction& orig);
    rc_transaction& operator=(const rc_transaction& orig);
private:
    rc_reconfigurable* reconf;
    bool p_has_ended;
};

/* inline code */

inline void rc_reconfigurable::rc_end_transaction()
{
    if (p_transaction_count > 1) {
        --p_transaction_count;
    } else {
        this->reset_transaction_count();
    }
}

inline rc_transaction::rc_transaction(rc_reconfigurable* reconf)
    : reconf(reconf), p_has_ended(false)
{
    if (reconf != NULL) {
        reconf->rc_begin_transaction();
    }
    // p_has_ended must initially be set to false (see RC_TRANSACTION)
}

inline void rc_transaction::begin()
{
    if (p_has_ended && reconf != NULL) {
        reconf->rc_begin_transaction();
    }
    // this must always be set, to allow RC_TRANSACTION to work properly
    p_has_ended = false;
}

inline void rc_transaction::end()
{
    if (!p_has_ended && reconf != NULL) {
        reconf->rc_end_transaction();
        reconf->rc_possible_deactivation();
    }
    // this must always be set, to allow RC_TRANSACTION to work properly
    p_has_ended = true;
}

/* global functions */

extern rc_reconfigurable* rc_get_reconfigurable_context();

extern rc_reconfigurable* rc_get_reconfigurable_context(
    sc_object* start_search);

extern const rc_reconfigurable* rc_register_resettable(
    rc_resettable& resettable, sc_object* search_start);

} // namespace ReChannel

#define RC_LOAD       rc_reconfigurable::LOAD

#define RC_ACTIVATE   rc_reconfigurable::ACTIVATE

#define RC_DEACTIVATE rc_reconfigurable::DEACTIVATE

#define RC_UNLOAD     rc_reconfigurable::UNLOAD

#define RC_ACTIVE   rc_reconfigurable::ACTIVE

#define RC_INACTIVE rc_reconfigurable::INACTIVE

#define RC_UNLOADED rc_reconfigurable::UNLOADED

#define RC_ON_LOAD() \
        virtual void rc_on_load()

#define RC_ON_ACTIVATE() \
        virtual void rc_on_activate()

#define RC_ON_DEACTIVATE() \
        virtual void rc_on_deactivate()

#define RC_ON_UNLOAD() \
        virtual void rc_on_unload()

#endif //RC_RECONFIGURABLE_H_

//
// $Id: rc_reconfigurable.h,v 1.22 2008/01/01 13:34:00 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reconfigurable.h,v $
//
