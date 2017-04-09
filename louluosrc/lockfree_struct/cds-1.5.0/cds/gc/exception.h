/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#ifndef __CDS_GC_EXCEPTION_H
#define __CDS_GC_EXCEPTION_H

#include <cds/details/defs.h>

namespace cds { namespace gc {

    /// %Exception "Too few hazard pointers"
    CDS_DECLARE_EXCEPTION( too_few_hazard_pointers, "Too few hazard pointers" ) ;

}} // namespace cds::gc

#endif // #ifndef __CDS_GC_EXCEPTION_H
