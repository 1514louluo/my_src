/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#ifndef __CDS_COMPILER_CXX11_ATOMIC_PATCHES_H
#define __CDS_COMPILER_CXX11_ATOMIC_PATCHES_H

#if CDS_COMPILER == CDS_COMPILER_GCC
#   include <cds/compiler/gcc/cxx11_atomic_patches.h>
#elif CDS_COMPILER == CDS_COMPILER_INTEL && CDS_OS_INTERFACE == CDS_OSI_WINDOWS
#   include <cds/compiler/icl/cxx11_atomic_patches_win.h>
#endif


#endif // #ifndef __CDS_COMPILER_CXX11_ATOMIC_PATCHES_H
