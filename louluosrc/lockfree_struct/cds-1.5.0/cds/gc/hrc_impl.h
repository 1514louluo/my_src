/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#ifndef __CDS_GC_HRC_IMPL_H
#define __CDS_GC_HRC_IMPL_H

#include <cds/threading/model.h>
#include <cds/details/static_functor.h>

//@cond
namespace cds { namespace gc {

    inline HRC::thread_gc::thread_gc(
        bool    bPersistent
        )
        : m_bPersistent( bPersistent )
    {
        if ( !cds::threading::Manager::isThreadAttached() )
            cds::threading::Manager::attachThread() ;
    }

    inline HRC::thread_gc::~thread_gc()
    {
        if ( !m_bPersistent )
            cds::threading::Manager::detachThread() ;
    }

    inline HRC::Guard::Guard()
        : Guard::base_class( cds::threading::getGC<HRC>() )
    {}

    template <size_t COUNT>
    inline HRC::GuardArray<COUNT>::GuardArray()
        : GuardArray::base_class( threading::getGC<HRC>() )
    {}

    template <typename T>
    inline  void HRC::retire( T * p, void (* pFunc)(T *) )
    {
        cds::threading::getGC<HRC>().retireNode( p, reinterpret_cast<details::free_retired_ptr_func>(pFunc) )   ;
    }

    template <class Disposer, typename T>
    inline void HRC::retire( T * p )
    {
        cds::threading::getGC<HRC>().retireNode( p, reinterpret_cast<details::free_retired_ptr_func>( cds::details::static_functor<Disposer, T>::call ))    ;
    }

    inline void HRC::scan()
    {
        cds::threading::getGC<HRC>().scan()  ;
    }


}} // namespace cds::gc
//@endcond

#endif // #ifndef __CDS_GC_HRC_IMPL_H