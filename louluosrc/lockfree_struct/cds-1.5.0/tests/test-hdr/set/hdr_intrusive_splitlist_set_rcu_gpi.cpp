/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "set/hdr_intrusive_set.h"
#include <cds/urcu/general_instant.h>
#include <cds/intrusive/michael_list_rcu.h>
#include <cds/intrusive/split_list_rcu.h>

namespace set {
    namespace {
        typedef cds::urcu::gc< cds::urcu::general_instant<> > rcu_type ;
    }

    void IntrusiveHashSetHdrTest::split_dyn_RCU_GPI_base_cmp()
    {
        typedef base_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::base_hook< co::gc<rcu_type> > >
                ,co::compare< cmp<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,ci::split_list::dynamic_bucket_table<true>
                ,co::memory_model<co::v::relaxed_ordering>
            >::type
        > set ;
        static_assert( set::options::dynamic_bucket_table, "Set has static bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_dyn_RCU_GPI_base_less()
    {
        typedef base_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::base_hook< co::gc<rcu_type> > >
                ,co::less< less<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,co::memory_model<co::v::sequential_consistent>
            >::type
        > set ;
        static_assert( set::options::dynamic_bucket_table, "Set has static bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_dyn_RCU_GPI_base_cmpmix()
    {
        typedef base_int_item< ci::split_list::node<ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::base_hook< co::gc<rcu_type> > >
                ,co::less< less<item> >
                ,co::compare< cmp<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,co::item_counter< simple_item_counter >
                ,ci::split_list::dynamic_bucket_table<true>
            >::type
        > set ;
        static_assert( set::options::dynamic_bucket_table, "Set has static bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_dyn_RCU_GPI_member_cmp()
    {
        typedef member_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::member_hook<
                    offsetof( item, hMember ),
                    co::gc<rcu_type>
                > >
                ,co::compare< cmp<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
            >::type
        > set ;
        static_assert( set::options::dynamic_bucket_table, "Set has static bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_dyn_RCU_GPI_member_less()
    {
        typedef member_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::member_hook<
                    offsetof( item, hMember ),
                    co::gc<rcu_type>
                > >
                ,co::less< less<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,co::memory_model<co::v::relaxed_ordering>
            >::type
        > set ;
        static_assert( set::options::dynamic_bucket_table, "Set has static bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_dyn_RCU_GPI_member_cmpmix()
    {
        typedef member_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::member_hook<
                    offsetof( item, hMember ),
                    co::gc<rcu_type>
                > >
                ,co::compare< cmp<item> >
                ,co::less< less<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,co::item_counter< simple_item_counter >
                ,co::memory_model<co::v::sequential_consistent>
            >::type
        > set ;
        static_assert( set::options::dynamic_bucket_table, "Set has static bucket table" )    ;

        test_rcu_int<set>()    ;
    }


    // Static bucket table
    void IntrusiveHashSetHdrTest::split_st_RCU_GPI_base_cmp()
    {
        typedef base_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::base_hook< co::gc<rcu_type> > >
                ,co::compare< cmp<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,ci::split_list::dynamic_bucket_table<false>
                ,co::memory_model<co::v::relaxed_ordering>
            >::type
        > set ;
        static_assert( !set::options::dynamic_bucket_table, "Set has dynamic bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_st_RCU_GPI_base_less()
    {
        typedef base_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::base_hook< co::gc<rcu_type> > >
                ,co::less< less<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,ci::split_list::dynamic_bucket_table<false>
                ,co::memory_model<co::v::sequential_consistent>
            >::type
        > set ;
        static_assert( !set::options::dynamic_bucket_table, "Set has dynamic bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_st_RCU_GPI_base_cmpmix()
    {
        typedef base_int_item< ci::split_list::node<ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::base_hook< co::gc<rcu_type> > >
                ,co::less< less<item> >
                ,co::compare< cmp<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,co::item_counter< simple_item_counter >
                ,ci::split_list::dynamic_bucket_table<false>
            >::type
        > set ;
        static_assert( !set::options::dynamic_bucket_table, "Set has dynamic bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_st_RCU_GPI_member_cmp()
    {
        typedef member_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::member_hook<
                    offsetof( item, hMember ),
                    co::gc<rcu_type>
                > >
                ,co::compare< cmp<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,ci::split_list::dynamic_bucket_table<false>
                ,co::memory_model<co::v::relaxed_ordering>
            >::type
        > set ;
        static_assert( !set::options::dynamic_bucket_table, "Set has dynamic bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_st_RCU_GPI_member_less()
    {
        typedef member_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::member_hook<
                    offsetof( item, hMember ),
                    co::gc<rcu_type>
                > >
                ,co::less< less<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                ci::split_list::dynamic_bucket_table<false>
                ,co::hash< hash_int >
                ,co::memory_model<co::v::sequential_consistent>
            >::type
        > set ;
        static_assert( !set::options::dynamic_bucket_table, "Set has dynamic bucket table" )    ;

        test_rcu_int<set>()    ;
    }

    void IntrusiveHashSetHdrTest::split_st_RCU_GPI_member_cmpmix()
    {
        typedef member_int_item< ci::split_list::node< ci::michael_list::node<rcu_type> > > item ;
        typedef ci::MichaelList< rcu_type
            ,item
            ,ci::michael_list::make_traits<
                ci::opt::hook< ci::michael_list::member_hook<
                    offsetof( item, hMember ),
                    co::gc<rcu_type>
                > >
                ,co::compare< cmp<item> >
                ,co::less< less<item> >
                ,ci::opt::disposer< faked_disposer >
            >::type
        >    ord_list    ;

        typedef ci::SplitListSet< rcu_type, ord_list,
            ci::split_list::make_traits<
                co::hash< hash_int >
                ,co::item_counter< simple_item_counter >
                ,ci::split_list::dynamic_bucket_table<false>
            >::type
        > set ;
        static_assert( !set::options::dynamic_bucket_table, "Set has dynamic bucket table" )    ;

        test_rcu_int<set>()    ;
    }


} // namespace set
