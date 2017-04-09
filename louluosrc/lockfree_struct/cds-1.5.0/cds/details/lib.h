/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#ifndef __CDS_LIB_H
#define __CDS_LIB_H
//@cond

#include <cds/details/defs.h>

#ifndef CDS_BUILD_LIB

#ifdef _DEBUG
#   define  CDS_LIB_DEBUG_SUFFIX "_debug"
#else
#   define  CDS_LIB_DEBUG_SUFFIX ""
#endif

#if CDS_COMPILER == CDS_COMPILER_MSVC
#   pragma comment( lib, "libcds-" CDS_PROCESSOR__NICK "-" CDS_COMPILER__NICK CDS_LIB_DEBUG_SUFFIX )
#elif CDS_COMPILER == CDS_COMPILER_INTEL
#   pragma comment( lib, "libcds-" CDS_PROCESSOR__NICK "-" CDS_COMPILER__NICK CDS_LIB_DEBUG_SUFFIX )
#endif

#undef CDS_LIB_DEBUG_SUFFIX

#endif // #ifndef CDS_BUILD_LIB

//@endcond
#endif // #ifndef __CDS_LIB_H
