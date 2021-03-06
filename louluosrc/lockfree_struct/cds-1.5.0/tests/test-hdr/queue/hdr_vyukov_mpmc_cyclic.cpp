/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include <cds/container/vyukov_mpmc_cycle_queue.h>

#include "queue/queue_test_header.h"

namespace queue {
    namespace {
        class VyukovMPMCCyclicQueue_int:
            public cds::container::VyukovMPMCCycleQueue<
                int,
                cds::opt::buffer<cds::opt::v::static_buffer<int, 1024 *32 > >
            >
        {
            typedef cds::container::VyukovMPMCCycleQueue<
                int,
                cds::opt::buffer<cds::opt::v::static_buffer<int, 1024 *32 > >
            > base_class ;
        public:
            VyukovMPMCCyclicQueue_int()
                : base_class(1024 * 32)
            {}
        };

        class VyukovMPMCCyclicQueue_int_ic:
            public cds::container::VyukovMPMCCycleQueue<
                int,
                cds::opt::buffer<cds::opt::v::dynamic_buffer<int> >,
                cds::opt::item_counter< cds::atomicity::item_counter >
            >
        {
            typedef cds::container::VyukovMPMCCycleQueue<
                int,
                cds::opt::buffer<cds::opt::v::dynamic_buffer<int> >,
                cds::opt::item_counter< cds::atomicity::item_counter >
            > base_class ;
        public:
            VyukovMPMCCyclicQueue_int_ic()
                : base_class( 32 * 1024 )
            {}
        };
    }

    void Queue_TestHeader::Vyukov_MPMCCyclicQueue()
    {
        testNoItemCounter< VyukovMPMCCyclicQueue_int >()    ;
    }

    void Queue_TestHeader::Vyukov_MPMCCyclicQueue_Counted()
    {
        testWithItemCounter< VyukovMPMCCyclicQueue_int_ic >() ;
    }
}
