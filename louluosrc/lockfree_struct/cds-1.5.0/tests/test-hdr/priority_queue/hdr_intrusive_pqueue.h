/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#ifndef CDSHDRTEST_INTRUSIVE_PQUEUE_H
#define CDSHDRTEST_INTRUSIVE_PQUEUE_H

#include "cppunit/cppunit_proxy.h"
#include "size_check.h"
#include <algorithm>

namespace priority_queue {

    namespace intrusive_pqueue {
        static size_t const c_nCapacity = 1024 * 16 ;

        struct disposer {
            static size_t   m_nCallCount ;

            template <typename T>
            void operator()( T * p )
            {
                ++m_nCallCount  ;
            }
        };
        struct another_disposer {
            static size_t   m_nCallCount ;

            template <typename T>
            void operator()( T * p )
            {
                ++m_nCallCount  ;
            }
        };

        template <typename Disp>
        static inline bool check_disposer_value( size_t nExpected )
        {
            return true ;
        }
        template <>
        inline bool check_disposer_value<disposer>( size_t nExpected )
        {
            return disposer::m_nCallCount == nExpected ;
        }

        template <typename PQueue>
        struct constants {
            static size_t const nCapacity = c_nCapacity ;
        };
    } // namespace intrusive_pqueue

    class IntrusivePQueueHdrTest: public CppUnitMini::TestCase
    {
    public:
        static size_t const c_nCapacity = intrusive_pqueue::c_nCapacity ;

        typedef int     key_type    ;
        static key_type const c_nMinValue = -123  ;

        struct compare {
            int operator()( key_type k1, key_type k2 ) const
            {
                return k1 - k2 ;
            }
        };

        template <typename T>
        class data_array
        {
            T *     pFirst  ;
            T *     pLast   ;

        public:
            data_array( size_t nSize )
                : pFirst( new T[nSize] )
                , pLast( pFirst + nSize )
            {
                T i = c_nMinValue   ;
                for ( T * p = pFirst; p != pLast; ++p, ++i )
                    *p = i  ;

                std::random_shuffle( pFirst, pLast )    ;
            }

            ~data_array()
            {
                delete [] pFirst ;
            }

            T * begin() { return pFirst; }
            T * end()   { return pLast ; }
            size_t size() const
            {
                return pLast - pFirst ;
            }
        };

    protected:
        template <class PQueue>
        void test_bounded_with( PQueue& pq )
        {
            data_array<key_type> arr( pq.capacity() ) ;
            key_type * pFirst = arr.begin()   ;
            key_type * pLast  = pFirst + pq.capacity() ;

            CPPUNIT_ASSERT( pq.empty() ) ;
            CPPUNIT_ASSERT( pq.size() == 0 ) ;
            CPPUNIT_ASSERT( pq.capacity() == intrusive_pqueue::constants<PQueue>::nCapacity ) ;

            size_t nSize = 0 ;

            // Push test
            for ( key_type * p = pFirst; p < pLast; ++p ) {
                CPPUNIT_ASSERT( pq.push( *p ))  ;
                CPPUNIT_ASSERT( !pq.empty() ) ;
                CPPUNIT_ASSERT( pq.size() == ++nSize ) ;
            }

            CPPUNIT_ASSERT( pq.full() ) ;
            CPPUNIT_ASSERT( pq.size() == pq.capacity() ) ;

            // The queue is full
            key_type k = c_nMinValue + key_type(c_nCapacity)    ;
            CPPUNIT_ASSERT( !pq.push( k )) ;
            CPPUNIT_ASSERT( pq.full() ) ;
            CPPUNIT_ASSERT( pq.size() == pq.capacity() ) ;

            // Pop test
            key_type nPrev = c_nMinValue + key_type(pq.capacity()) - 1 ;
            key_type * p = pq.pop() ;
            CPPUNIT_ASSERT( p != NULL ) ;
            CPPUNIT_CHECK_EX( *p == nPrev, "Expected=" << nPrev << ", current=" << *p ) ;

            CPPUNIT_ASSERT( pq.size() == pq.capacity() - 1 ) ;
            CPPUNIT_ASSERT( !pq.full() ) ;
            CPPUNIT_ASSERT( !pq.empty() ) ;

            nSize = pq.size()  ;
            while ( pq.size() > 1 ) {
                p = pq.pop() ;
                CPPUNIT_ASSERT( p != NULL ) ;
                CPPUNIT_CHECK_EX( *p == nPrev - 1, "Expected=" << nPrev - 1 << ", current=" << *p ) ;
                nPrev = *p  ;
                --nSize     ;
                CPPUNIT_ASSERT( pq.size() == nSize ) ;
            }

            CPPUNIT_ASSERT( !pq.full() ) ;
            CPPUNIT_ASSERT( !pq.empty() ) ;
            CPPUNIT_ASSERT( pq.size() == 1 ) ;

            p = pq.pop() ;
            CPPUNIT_ASSERT( p != NULL ) ;
            CPPUNIT_CHECK_EX( *p == c_nMinValue, "Expected=" << c_nMinValue << ", current=" << *p ) ;

            CPPUNIT_ASSERT( !pq.full() ) ;
            CPPUNIT_ASSERT( pq.empty() ) ;
            CPPUNIT_ASSERT( pq.size() == 0 ) ;

            // Clear test
            for ( key_type * p = pFirst; p < pLast; ++p ) {
                CPPUNIT_ASSERT( pq.push( *p ))  ;
            }
            CPPUNIT_ASSERT( !pq.empty() ) ;
            CPPUNIT_ASSERT( pq.full() ) ;
            CPPUNIT_ASSERT( pq.size() == pq.capacity() ) ;

            intrusive_pqueue::disposer::m_nCallCount = 0;
            intrusive_pqueue::another_disposer::m_nCallCount = 0;
            pq.clear()  ;
            CPPUNIT_ASSERT( pq.empty() ) ;
            CPPUNIT_ASSERT( !pq.full() ) ;
            CPPUNIT_ASSERT( pq.size() == 0 ) ;
            CPPUNIT_ASSERT( intrusive_pqueue::another_disposer::m_nCallCount == 0 ) ;
            CPPUNIT_ASSERT( intrusive_pqueue::check_disposer_value<typename PQueue::disposer>( pq.capacity() ) ) ;

            // clear_with test
            for ( key_type * p = pFirst; p < pLast; ++p ) {
                CPPUNIT_ASSERT( pq.push( *p ))  ;
            }
            CPPUNIT_ASSERT( !pq.empty() ) ;
            CPPUNIT_ASSERT( pq.full() ) ;
            CPPUNIT_ASSERT( pq.size() == pq.capacity() ) ;

            intrusive_pqueue::disposer::m_nCallCount = 0;
            intrusive_pqueue::another_disposer::m_nCallCount = 0;
            pq.clear_with( intrusive_pqueue::another_disposer() )  ;
            CPPUNIT_ASSERT( pq.empty() ) ;
            CPPUNIT_ASSERT( !pq.full() ) ;
            CPPUNIT_ASSERT( pq.size() == 0 ) ;
            CPPUNIT_ASSERT( intrusive_pqueue::another_disposer::m_nCallCount == pq.capacity() ) ;
            CPPUNIT_ASSERT( intrusive_pqueue::check_disposer_value<typename PQueue::disposer>( 0 ) ) ;
        }

        template <class PQueue>
        void test_msq_stat()
        {
            PQueue pq( 0 );   // argument should be ignored for static buffer
            test_bounded_with( pq )     ;
        }
        template <class PQueue>
        void test_msq_dyn()
        {
            PQueue pq( c_nCapacity )    ;
            test_bounded_with( pq )     ;
        }

    public:
        void MSPQueue_st()          ;
        void MSPQueue_st_cmp()      ;
        void MSPQueue_st_less()     ;
        void MSPQueue_st_cmpless()  ;
        void MSPQueue_st_cmp_mtx()  ;
        void MSPQueue_st_less_disp();
        void MSPQueue_dyn()         ;
        void MSPQueue_dyn_cmp()     ;
        void MSPQueue_dyn_less()    ;
        void MSPQueue_dyn_cmpless() ;
        void MSPQueue_dyn_cmp_mtx() ;
        void MSPQueue_dyn_less_disp() ;

        CPPUNIT_TEST_SUITE(IntrusivePQueueHdrTest)
            CPPUNIT_TEST(MSPQueue_st)
            CPPUNIT_TEST(MSPQueue_st_cmp)
            CPPUNIT_TEST(MSPQueue_st_less)
            CPPUNIT_TEST(MSPQueue_st_cmpless)
            CPPUNIT_TEST(MSPQueue_st_cmp_mtx)
            CPPUNIT_TEST(MSPQueue_st_less_disp)
            CPPUNIT_TEST(MSPQueue_dyn)
            CPPUNIT_TEST(MSPQueue_dyn_cmp)
            CPPUNIT_TEST(MSPQueue_dyn_less)
            CPPUNIT_TEST(MSPQueue_dyn_cmpless)
            CPPUNIT_TEST(MSPQueue_dyn_cmp_mtx)
            CPPUNIT_TEST(MSPQueue_dyn_less_disp)
        CPPUNIT_TEST_SUITE_END()
    } ;

} // namespace priority_queue

#endif // #ifndef CDSHDRTEST_INTRUSIVE_PQUEUE_H
