/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "cppunit/thread.h"
#include "map2/map_types.h"
#include <algorithm> // random_shuffle

namespace map2 {

#    define TEST_MAP(X)         void X() { test<MapTypes<key_type, value_type>::X >()    ; }
#    define TEST_MAP_NOLF(X)    void X() { test_nolf<MapTypes<key_type, value_type>::X >()    ; }

    namespace {
        static size_t  c_nMapSize = 1000000         ;  // max map size
        static size_t  c_nInsThreadCount = 4        ;  // insert thread count
        static size_t  c_nDelThreadCount = 4        ;  // delete thread count
        static size_t  c_nMaxLoadFactor = 8         ;  // maximum load factor
        static bool    c_bPrintGCState = true       ;
    }

    namespace {
        struct key_thread
        {
            size_t  nKey    ;
            size_t  nThread ;

            key_thread( size_t key, size_t threadNo )
                : nKey( key )
                , nThread( threadNo )
            {}

            key_thread()
            {}
        };

        //typedef MapTypes<key_thread, size_t>::key_val     key_value_pair    ;
    }

    template <>
    struct cmp<key_thread> {
        int operator ()(key_thread const& k1, key_thread const& k2) const
        {
            if ( k1.nKey < k2.nKey )
                return -1   ;
            if ( k1.nKey > k2.nKey )
                return 1    ;
            if ( k1.nThread < k2.nThread )
                return -1   ;
            if ( k1.nThread > k2.nThread )
                return 1    ;
            return 0        ;
        }
        int operator ()(key_thread const& k1, size_t k2) const
        {
            if ( k1.nKey < k2 )
                return -1   ;
            if ( k1.nKey > k2 )
                return 1    ;
            return 0        ;
        }
        int operator ()(size_t k1, key_thread const& k2) const
        {
            if ( k1 < k2.nKey )
                return -1   ;
            if ( k1 > k2.nKey )
                return 1    ;
            return 0        ;
        }
    };

} // namespace map2

namespace std {
    template <>
    struct less<map2::key_thread>
    {
        bool operator()(map2::key_thread const& k1, map2::key_thread const& k2) const
        {
            if ( k1.nKey <= k2.nKey )
                return k1.nKey < k2.nKey || k1.nThread < k2.nThread ;
            return false    ;
        }
    };
} // namespace std

CDS_BEGIN_STD_HASH_NAMESPACE
template <>
struct hash<map2::key_thread>
{
    typedef size_t              result_type     ;
    typedef map2::key_thread    argument_type   ;

    size_t operator()(map2::key_thread const& k) const
    {
        return CDS_STD_HASH_NAMESPACE::hash<size_t>()( k.nKey ) ;
    }
    size_t operator()(size_t k) const
    {
        return CDS_STD_HASH_NAMESPACE::hash<size_t>()( k ) ;
    }
};
CDS_END_STD_HASH_NAMESPACE

namespace boost {
    inline size_t hash_value( map2::key_thread const& k )
    {
        return CDS_STD_HASH_NAMESPACE::hash<size_t>()( k.nKey ) ;
    }

    template <>
    struct hash<map2::key_thread>
    {
        typedef size_t              result_type     ;
        typedef map2::key_thread    argument_type   ;

        size_t operator()(map2::key_thread const& k) const
        {
            return boost::hash<size_t>()( k.nKey ) ;
        }
        size_t operator()(size_t k) const
        {
            return boost::hash<size_t>()( k ) ;
        }
    };
} // namespace boost

namespace map2 {

    template <typename Map>
    static inline void check_before_clear( Map& s )
    {}

    template <typename GC, typename Key, typename T, typename Traits>
    static inline void check_before_clear( cds::container::EllenBinTreeMap<GC, Key, T, Traits>& s )
    {
        CPPUNIT_CHECK_CURRENT( s.check_consistency() ) ;
    }

    class Map_DelOdd: public CppUnitMini::TestCase
    {
        std::vector<size_t>     m_arrData ;

    protected:
        typedef key_thread  key_type    ;
        typedef size_t      value_type  ;
        typedef std::pair<key_type const, value_type> pair_type ;

        CDS_ATOMIC::atomic<size_t>      m_nInsThreadCount ;

        // Inserts keys from [0..N)
        template <class Map>
        class InsertThread: public CppUnitMini::TestThread
        {
            Map&     m_Map      ;

            virtual InsertThread *    clone()
            {
                return new InsertThread( *this )    ;
            }

            struct ensure_func
            {
                template <typename Q>
                void operator()( bool bNew, Q const& )
                {}
            };
        public:
            size_t  m_nInsertSuccess    ;
            size_t  m_nInsertFailed     ;

        public:
            InsertThread( CppUnitMini::ThreadPool& pool, Map& rMap )
                : CppUnitMini::TestThread( pool )
                , m_Map( rMap )
            {}
            InsertThread( InsertThread& src )
                : CppUnitMini::TestThread( src )
                , m_Map( src.m_Map )
            {}

            Map_DelOdd&  getTest()
            {
                return reinterpret_cast<Map_DelOdd&>( m_Pool.m_Test )   ;
            }

            virtual void init() { cds::threading::Manager::attachThread()   ; }
            virtual void fini() { cds::threading::Manager::detachThread()   ; }

            virtual void test()
            {
                Map& rMap = m_Map   ;

                m_nInsertSuccess =
                    m_nInsertFailed = 0 ;

                std::vector<size_t>& arrData = getTest().m_arrData ;
                for ( size_t i = 0; i < arrData.size(); ++i ) {
                    if ( rMap.insert( key_type( arrData[i], m_nThreadNo )))
                        ++m_nInsertSuccess ;
                    else
                        ++m_nInsertFailed ;
                }

                ensure_func f ;
                for ( size_t i = arrData.size() - 1; i > 0; --i ) {
                    if ( arrData[i] & 1 ) {
                        rMap.ensure( key_type( arrData[i], m_nThreadNo ), f ) ;
                    }
                }

                getTest().m_nInsThreadCount.fetch_sub( 1, CDS_ATOMIC::memory_order_acquire ) ;
            }
        };

        // Deletes odd keys from [0..N)
        template <class Map>
        class DeleteThread: public CppUnitMini::TestThread
        {
            Map&     m_Map      ;

            virtual DeleteThread *    clone()
            {
                return new DeleteThread( *this )    ;
            }
        public:
            size_t  m_nDeleteSuccess    ;
            size_t  m_nDeleteFailed     ;

            struct key_equal {
                bool operator()( key_type const& k1, key_type const& k2 ) const
                {
                    return k1.nKey == k2.nKey ;
                }
                bool operator()( size_t k1, key_type const& k2 ) const
                {
                    return k1 == k2.nKey ;
                }
                bool operator()( key_type const& k1, size_t k2 ) const
                {
                    return k1.nKey == k2 ;
                }
            };


            struct key_less {
                bool operator()( key_type const& k1, key_type const& k2 ) const
                {
                    return k1.nKey < k2.nKey ;
                }
                bool operator()( size_t k1, key_type const& k2 ) const
                {
                    return k1 < k2.nKey ;
                }
                bool operator()( key_type const& k1, size_t k2 ) const
                {
                    return k1.nKey < k2 ;
                }

                typedef key_equal equal_to ;
            };

        public:
            DeleteThread( CppUnitMini::ThreadPool& pool, Map& rMap )
                : CppUnitMini::TestThread( pool )
                , m_Map( rMap )
            {}
            DeleteThread( DeleteThread& src )
                : CppUnitMini::TestThread( src )
                , m_Map( src.m_Map )
            {}

            Map_DelOdd&  getTest()
            {
                return reinterpret_cast<Map_DelOdd&>( m_Pool.m_Test )   ;
            }

            virtual void init() { cds::threading::Manager::attachThread()   ; }
            virtual void fini() { cds::threading::Manager::detachThread()   ; }

            virtual void test()
            {
                Map& rMap = m_Map   ;

                m_nDeleteSuccess =
                    m_nDeleteFailed = 0 ;

                std::vector<size_t>& arrData = getTest().m_arrData ;
                if ( m_nThreadNo & 1 ) {
                    for ( size_t k = 0; k < c_nInsThreadCount; ++k ) {
                        for ( size_t i = 0; i < arrData.size(); ++i ) {
                            if ( arrData[i] & 1 ) {
                                if ( rMap.erase_with( arrData[i], key_less() ))
                                    ++m_nDeleteSuccess  ;
                                else
                                    ++m_nDeleteFailed   ;
                            }
                        }
                        if ( getTest().m_nInsThreadCount.load( CDS_ATOMIC::memory_order_acquire ) == 0 )
                            break;
                    }
                }
                else {
                    for ( size_t k = 0; k < c_nInsThreadCount; ++k ) {
                        for ( size_t i = arrData.size() - 1; i > 0; --i ) {
                            if ( arrData[i] & 1 ) {
                                if ( rMap.erase_with( arrData[i], key_less() ))
                                    ++m_nDeleteSuccess  ;
                                else
                                    ++m_nDeleteFailed   ;
                            }
                        }
                        if ( getTest().m_nInsThreadCount.load( CDS_ATOMIC::memory_order_acquire ) == 0 )
                            break;
                    }
                }
            }
        };

    protected:
        template <class Map>
        void do_test( size_t nLoadFactor )
        {
            Map  testMap( c_nMapSize, nLoadFactor ) ;
            do_test_with( testMap ) ;
        }

        template <class Map>
        void do_test_with( Map& testMap )
        {
            typedef InsertThread<Map> insert_thread ;
            typedef DeleteThread<Map> delete_thread ;

            m_nInsThreadCount.store( c_nInsThreadCount, CDS_ATOMIC::memory_order_release ) ;
            cds::OS::Timer    timer    ;

            CppUnitMini::ThreadPool pool( *this )   ;
            pool.add( new insert_thread( pool, testMap ), c_nInsThreadCount ) ;
            pool.add( new delete_thread( pool, testMap ), c_nDelThreadCount ) ;
            pool.run() ;
            CPPUNIT_MSG( "   Duration=" << pool.avgDuration() ) ;

            size_t nInsertSuccess = 0   ;
            size_t nInsertFailed = 0    ;
            size_t nDeleteSuccess = 0   ;
            size_t nDeleteFailed = 0    ;
            for ( CppUnitMini::ThreadPool::iterator it = pool.begin(); it != pool.end(); ++it ) {
                insert_thread * pThread = dynamic_cast<insert_thread *>( *it )   ;
                if ( pThread ) {
                    nInsertSuccess += pThread->m_nInsertSuccess ;
                    nInsertFailed += pThread->m_nInsertFailed   ;
                }
                else {
                    delete_thread * p = static_cast<delete_thread *>( *it ) ;
                    nDeleteSuccess += p->m_nDeleteSuccess   ;
                    nDeleteFailed += p->m_nDeleteFailed ;
                }
            }

            CPPUNIT_MSG( "  Totals (success/failed): \n\t"
                << "      Insert=" << nInsertSuccess << '/' << nInsertFailed << "\n\t"
                << "      Delete=" << nDeleteSuccess << '/' << nDeleteFailed << "\n\t"
                ) ;

            // All even keys must be in the map
            {
                size_t nErrorCount = 0 ;
                CPPUNIT_MSG( "  Check even keys..." ) ;
                for ( size_t n = 0; n < c_nMapSize; n +=2 ) {
                    for ( size_t i = 0; i < c_nInsThreadCount; ++i ) {
                        if ( !testMap.find( key_type(n, i) ) ) {
                            if ( ++nErrorCount < 10 ) {
                                CPPUNIT_MSG( "key " << n << "-" << i << " is not found!") ;
                            }
                        }
                    }
                }
                CPPUNIT_CHECK_EX( nErrorCount == 0, "Totals: " << nErrorCount << " keys is not found") ;
                //if ( nErrorCount >= 10 ) {
                //    CPPUNIT_MSG( "Totals: " << nErrorCount << " keys is not found") ;
                //}
            }

            check_before_clear( testMap )    ;

            CPPUNIT_CHECK( nInsertSuccess == c_nMapSize * c_nInsThreadCount ) ;
            CPPUNIT_CHECK( nInsertFailed == 0 ) ;

            CPPUNIT_MSG( "  Clear map (single-threaded)..." ) ;
            timer.reset()   ;
            testMap.clear() ;
            CPPUNIT_MSG( "   Duration=" << timer.duration() ) ;
            CPPUNIT_CHECK_EX( testMap.empty(), ((long long) testMap.size()) ) ;

            additional_check( testMap ) ;
            print_stat( testMap )       ;

            additional_cleanup( testMap ) ;
        }

        template <class Map>
        void test()
        {
            CPPUNIT_MSG( "Insert thread count=" << c_nInsThreadCount
                << " delete thread count=" << c_nDelThreadCount
                << " set size=" << c_nMapSize
                );

            for ( size_t nLoadFactor = 1; nLoadFactor <= c_nMaxLoadFactor; nLoadFactor *= 2 ) {
                CPPUNIT_MSG( "Load factor=" << nLoadFactor )   ;
                do_test<Map>( nLoadFactor )     ;
                if ( c_bPrintGCState )
                    print_gc_state()            ;
            }
        }

        template <class Map>
        void test_nolf()
        {
            CPPUNIT_MSG( "Insert thread count=" << c_nInsThreadCount
                << " delete thread count=" << c_nDelThreadCount
                << " set size=" << c_nMapSize
                );

            Map s ;
            do_test_with( s )     ;
            if ( c_bPrintGCState )
                print_gc_state()            ;
        }

        void setUpParams( const CppUnitMini::TestCfg& cfg ) {
            c_nMapSize = cfg.getULong("MapSize", 1000000 ) ;
            c_nInsThreadCount = cfg.getULong("InsThreadCount", 4 )  ;
            c_nDelThreadCount = cfg.getULong("DelThreadCount", 4 )  ;
            c_nMaxLoadFactor = cfg.getULong("MaxLoadFactor", 8 )    ;
            c_bPrintGCState = cfg.getBool("PrintGCStateFlag", true );

            if ( c_nInsThreadCount == 0 )
                c_nInsThreadCount = cds::OS::topology::processor_count() ;
            if ( c_nDelThreadCount == 0 )
                c_nDelThreadCount = cds::OS::topology::processor_count() ;

            m_arrData.resize( c_nMapSize ) ;
            for ( size_t i = 0; i < c_nMapSize; ++i )
                m_arrData[i] = i ;
            std::random_shuffle( m_arrData.begin(), m_arrData.end() )   ;
        }

#   include "map2/map_defs.h"
        CDSUNIT_DECLARE_MichaelMap
        CDSUNIT_DECLARE_SplitList
        //CDSUNIT_DECLARE_StripedMap
        //CDSUNIT_DECLARE_RefinableMap
        CDSUNIT_DECLARE_CuckooMap
        CDSUNIT_DECLARE_SkipListMap
        CDSUNIT_DECLARE_EllenBinTreeMap
        //CDSUNIT_DECLARE_StdMap

        CPPUNIT_TEST_SUITE( Map_DelOdd )
            CDSUNIT_TEST_MichaelMap
            CDSUNIT_TEST_SplitList
            CDSUNIT_TEST_SkipListMap
            CDSUNIT_TEST_EllenBinTreeMap
            //CDSUNIT_TEST_StripedMap
            //CDSUNIT_TEST_RefinableMap
            CDSUNIT_TEST_CuckooMap
            //CDSUNIT_TEST_StdMap
        CPPUNIT_TEST_SUITE_END()
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( Map_DelOdd );
} // namespace map2
