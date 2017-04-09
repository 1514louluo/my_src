/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "set/hdr_set.h"
#include <cds/urcu/signal_threaded.h>
#include <cds/container/lazy_list_rcu.h>
#include <cds/container/split_list_set_rcu.h>

namespace set {

#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
    namespace {
        typedef cds::urcu::gc< cds::urcu::signal_threaded<> > rcu_type ;

        struct RCU_SHT_cmp_traits: public cc::split_list::type_traits
        {
            typedef cc::lazy_list_tag                   ordered_list    ;
            typedef HashSetHdrTest::hash_int            hash            ;
            typedef HashSetHdrTest::simple_item_counter item_counter    ;
            typedef cc::opt::v::relaxed_ordering        memory_model    ;
            enum { dynamic_bucket_table = false };

            struct ordered_list_traits: public cc::lazy_list::type_traits
            {
                typedef HashSetHdrTest::cmp<HashSetHdrTest::item>   compare ;
            } ;
        };

        struct RCU_SHT_less_traits: public cc::split_list::type_traits
        {
            typedef cc::lazy_list_tag                ordered_list    ;
            typedef HashSetHdrTest::hash_int            hash            ;
            typedef HashSetHdrTest::simple_item_counter item_counter    ;
            typedef cc::opt::v::sequential_consistent                      memory_model    ;
            enum { dynamic_bucket_table = false };

            struct ordered_list_traits: public cc::lazy_list::type_traits
            {
                typedef HashSetHdrTest::less<HashSetHdrTest::item>   less ;
            };
        };

        struct RCU_SHT_cmpmix_traits: public cc::split_list::type_traits
        {
            typedef cc::lazy_list_tag                ordered_list    ;
            typedef HashSetHdrTest::hash_int            hash            ;
            typedef HashSetHdrTest::simple_item_counter item_counter    ;

            struct ordered_list_traits: public cc::lazy_list::type_traits
            {
                typedef HashSetHdrTest::cmp<HashSetHdrTest::item>   compare ;
                typedef HashSetHdrTest::less<HashSetHdrTest::item>   less   ;
            };
        };
    }
#endif

    void HashSetHdrTest::Split_Lazy_RCU_SHT_cmp()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        // traits-based version
        typedef cc::SplitListSet< rcu_type, item, RCU_SHT_cmp_traits > set   ;

        test_int< set >()  ;

        // option-based version
        typedef cc::SplitListSet< rcu_type, item,
            cc::split_list::make_traits<
                cc::split_list::ordered_list<cc::lazy_list_tag>
                ,cc::opt::hash< hash_int >
                ,cc::opt::item_counter< simple_item_counter >
                ,cc::opt::memory_model< cc::opt::v::relaxed_ordering >
                ,cc::split_list::dynamic_bucket_table< true >
                ,cc::split_list::ordered_list_traits<
                    cc::lazy_list::make_traits<
                        cc::opt::compare< cmp<item> >
                    >::type
                >
            >::type
        > opt_set   ;
        test_int< opt_set >()  ;
#endif
    }

    void HashSetHdrTest::Split_Lazy_RCU_SHT_less()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        // traits-based version
        typedef cc::SplitListSet< rcu_type, item, RCU_SHT_less_traits > set   ;

        test_int< set >()  ;

        // option-based version
        typedef cc::SplitListSet< rcu_type, item,
            cc::split_list::make_traits<
                cc::split_list::ordered_list<cc::lazy_list_tag>
                ,cc::opt::hash< hash_int >
                ,cc::opt::item_counter< simple_item_counter >
                ,cc::opt::memory_model< cc::opt::v::sequential_consistent >
                ,cc::split_list::dynamic_bucket_table< false >
                ,cc::split_list::ordered_list_traits<
                    cc::lazy_list::make_traits<
                        cc::opt::less< less<item> >
                    >::type
                >
            >::type
        > opt_set   ;
        test_int< opt_set >()  ;
#endif
    }

    void HashSetHdrTest::Split_Lazy_RCU_SHT_cmpmix()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        // traits-based version
        typedef cc::SplitListSet< rcu_type, item, RCU_SHT_cmpmix_traits > set   ;
        test_int< set >()  ;

        // option-based version
        typedef cc::SplitListSet< rcu_type, item,
            cc::split_list::make_traits<
                cc::split_list::ordered_list<cc::lazy_list_tag>
                ,cc::opt::hash< hash_int >
                ,cc::opt::item_counter< simple_item_counter >
                ,cc::split_list::ordered_list_traits<
                    cc::lazy_list::make_traits<
                        cc::opt::less< less<item> >
                        ,cc::opt::compare< cmp<item> >
                    >::type
                >
            >::type
        > opt_set   ;
        test_int< opt_set >()  ;
#endif
    }

} // namespace set
