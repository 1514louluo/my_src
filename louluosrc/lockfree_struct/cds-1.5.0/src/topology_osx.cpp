/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include <cds/os/topology.h>

#if CDS_OS_TYPE == CDS_OS_OSX

#include <sys/types.h>
#include <sys/sysctl.h>

namespace cds { namespace OS { CDS_CXX11_INLINE_NAMESPACE namespace OS_X {

    unsigned int topology::s_nProcessorCount = 0   ;

    void topology::init()
    {
        size_t len = sizeof( s_nProcessorCount ) ;
        if ( sysctlbyname("hw.logicalcpu", &s_nProcessorCount, &len, NULL, 0 ) != 0 )
            s_nProcessorCount = 1   ;
    }

    void topology::fini()
    {}
}}} // namespace cds::OS::Linux

#endif  // #if CDS_OS_TYPE == CDS_OS_OSX
