/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


/*
 * Copyright (c) 2003, 2004
 * Zdenek Nemec
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/* $Id$ */

#ifndef _CPPUNITPROXYINTERFACE_H_
#define _CPPUNITPROXYINTERFACE_H_

/*
 * STLport specific
 */
#if !defined (CPPUNIT_MINI_USE_EXCEPTIONS) && \
    (!defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS))
#  define CPPUNIT_MINI_USE_EXCEPTIONS
#endif

#include "cppunit/cppunit_mini.h"
#include <cds/cxx11_atomic.h>   // for cds::atomicity::empty_item_counter

#endif
