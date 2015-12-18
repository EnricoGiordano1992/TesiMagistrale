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
#ifndef RC_REPORT_H_
#define RC_REPORT_H_

#include <cstring>
#include <sstream>

#include <systemc.h>

#if !defined(RC_DISABLE_COUT)

#define RC_COUT(msg) std::cout << msg; (void)0

#define RC_COUTL(msg) std::cout << msg << std::endl; (void)0
#else
#define RC_COUT(msg) (void)0
#define RC_COUTL(msg) (void)0
#endif // !defined(RC_DISABLE_COUT)

#define RC_REPORT_MSG_TYPE "/ReChannel"

#define RC_REPORT_INFO(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        SC_REPORT_INFO(msg_type, sstream.str().c_str()); \
    } else (void)0

#define RC_REPORT_WARNING(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        SC_REPORT_WARNING(msg_type, sstream.str().c_str()); \
    } else (void)0

#define RC_REPORT_ERROR(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        SC_REPORT_ERROR(msg_type, sstream.str().c_str()); throw 0; \
    } else (void)0

#define RC_REPORT_FATAL(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        sc_report_handler::force(SC_ABORT); \
        SC_REPORT_FATAL(msg_type, sstream.str().c_str()); throw 0; \
    } else (void)0

#ifdef RC_DEBUG

 #define rc_assert(expr) \
    ((void)((expr) \
      ? 0 : (SC_REPORT_FATAL(RC_ID_ASSERTION_FAILED_, #expr), 0)))
#else
 #define rc_assert(expr) \
    (void)0
#endif

#ifndef RC_DEBUG_LEVEL

 #define RC_DEBUG_LEVEL 3
#endif

#ifdef RC_DEBUG

 #define RC_REPORT_DEBUG(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_, msg)
#else
 #define RC_REPORT_DEBUG(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 1

 #define RC_REPORT_DEBUG_1(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_1_, msg)
#else
 #define RC_REPORT_DEBUG_1(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 2

 #define RC_REPORT_DEBUG_2(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_2_, msg)
#else
 #define RC_REPORT_DEBUG_2(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 3

 #define RC_REPORT_DEBUG_3(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_3_, msg)
#else
 #define RC_REPORT_DEBUG_3(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 4

 #define RC_REPORT_DEBUG_4(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_4_, msg)
#else
 #define RC_REPORT_DEBUG_4(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 5

 #define RC_REPORT_DEBUG_5(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_5_, msg)
#else
 #define RC_REPORT_DEBUG_5(msg) (void)0
#endif

namespace ReChannel {

// predefined message types
extern const char RC_ID_INTERNAL_ERROR_[];
extern const char RC_ID_UNKNOWN_ERROR_[];
extern const char RC_ID_NOT_IMPLEMENTED_[];
extern const char RC_ID_OUT_OF_BOUNDS_[];
extern const char RC_ID_NULL_POINTER_[];
extern const char RC_ID_INVALID_USAGE_[];
extern const char RC_ID_DUPLICATE_ENTRY_[];
extern const char RC_ID_ASSERTION_FAILED_[];

extern const char RC_ID_DEBUG_MSG_[];
extern const char RC_ID_DEBUG_MSG_LVL_1_[];
extern const char RC_ID_DEBUG_MSG_LVL_2_[];
extern const char RC_ID_DEBUG_MSG_LVL_3_[];
extern const char RC_ID_DEBUG_MSG_LVL_4_[];
extern const char RC_ID_DEBUG_MSG_LVL_5_[];

template<bool Assertion>
class rc_static_assertion;

template<>
class rc_static_assertion<true>
{ public: typedef bool fulfilled; };

template<>
class rc_static_assertion<false>
{ public: typedef bool failed; };

} // namespace ReChannel

#define RC_STATIC_ASSERT(name_, assertion_) \
    typedef ::ReChannel::rc_static_assertion< \
            (bool)(assertion_) \
        >::fulfilled RC_STATIC_ASSERT_##name_/*;*/

#define RC_STATIC_ASSERT_T(name_, assertion_) \
    typedef typename ::ReChannel::template rc_static_assertion< \
            (bool)(assertion_) \
        >::fulfilled RC_STATIC_ASSERT_##name_/*;*/

#endif // RC_REPORT_H_

//
// $Id: rc_report.h,v 1.10 2007/11/23 20:10:50 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_report.h,v $
//
