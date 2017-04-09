/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "priority_queue/hdr_intrusive_pqueue.h"
#include "priority_queue/hdr_pqueue.h"

namespace priority_queue {

    size_t intrusive_pqueue::disposer::m_nCallCount = 0 ;
    size_t intrusive_pqueue::another_disposer::m_nCallCount = 0 ;

    size_t pqueue::disposer::m_nCallCount = 0 ;

} // namespace priority_queue


CPPUNIT_TEST_SUITE_REGISTRATION_(priority_queue::IntrusivePQueueHdrTest, s_IntrusivePQueueHdrTest)      ;
CPPUNIT_TEST_SUITE_REGISTRATION_(priority_queue::PQueueHdrTest, s_PQueueHdrTest)      ;
