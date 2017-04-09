/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "priority_queue/hdr_pqueue.h"
#include <cds/container/mspriority_queue.h>
#include <cds/details/std/mutex.h>

namespace priority_queue {
    namespace pqueue {
        template <typename T, typename Traits>
        struct constants<cds::container::MSPriorityQueue<T, Traits> > {
            static size_t const nCapacity = c_nCapacity - 1 ;
        };
    }

    namespace {
        typedef cds::opt::v::static_buffer< char, PQueueHdrTest::c_nCapacity > buffer_type ;
    }

    void PQueueHdrTest::MSPQueue_st()
    {
        typedef cds::container::MSPriorityQueue< PQueueHdrTest::value_type,
            cds::container::mspriority_queue::make_traits<
                cds::opt::buffer< buffer_type >
            >::type
        > pqueue    ;

        test_msq_stat<pqueue>() ;
    }

    void PQueueHdrTest::MSPQueue_st_cmp()
    {
        typedef cds::container::MSPriorityQueue< PQueueHdrTest::value_type,
            cds::container::mspriority_queue::make_traits<
                cds::opt::buffer< buffer_type >
                ,cds::opt::compare< PQueueHdrTest::compare >
            >::type
        > pqueue    ;

        test_msq_stat<pqueue>() ;
    }

    void PQueueHdrTest::MSPQueue_st_less()
    {
        typedef cds::container::MSPriorityQueue< PQueueHdrTest::value_type,
            cds::container::mspriority_queue::make_traits<
                cds::opt::buffer< buffer_type >
                ,cds::opt::less< PQueueHdrTest::less >
            >::type
        > pqueue    ;

        test_msq_stat<pqueue>() ;
    }

    void PQueueHdrTest::MSPQueue_st_cmpless()
    {
        typedef cds::container::MSPriorityQueue< PQueueHdrTest::value_type,
            cds::container::mspriority_queue::make_traits<
                cds::opt::buffer< buffer_type >
                ,cds::opt::less< PQueueHdrTest::less >
                ,cds::opt::compare< PQueueHdrTest::compare >
            >::type
        > pqueue    ;

        test_msq_stat<pqueue>() ;
    }

    void PQueueHdrTest::MSPQueue_st_cmp_mtx()
    {
        typedef cds::container::MSPriorityQueue< PQueueHdrTest::value_type,
            cds::container::mspriority_queue::make_traits<
                cds::opt::buffer< buffer_type >
                ,cds::opt::compare< PQueueHdrTest::compare >
                ,cds::opt::lock_type<cds_std::mutex>
            >::type
        > pqueue    ;

        test_msq_stat<pqueue>() ;
    }

} // namespace priority_queue
