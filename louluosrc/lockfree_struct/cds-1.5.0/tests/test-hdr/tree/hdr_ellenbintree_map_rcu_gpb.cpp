/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "tree/hdr_ellenbintree_map.h"
#include <cds/urcu/general_buffered.h>
#include <cds/container/ellen_bintree_map_rcu.h>

#include "tree/hdr_intrusive_ellen_bintree_pool_rcu.h"
#include "unit/print_ellenbintree_stat.h"

namespace tree {
    namespace cc = cds::container ;
    namespace co = cds::opt ;
    namespace {
        typedef cds::urcu::gc< cds::urcu::general_buffered<> > rcu_type ;

        typedef cc::ellen_bintree::map_node<rcu_type, EllenBinTreeMapHdrTest::key_type, EllenBinTreeMapHdrTest::value_type> tree_leaf_node  ;
        typedef cc::ellen_bintree::internal_node< EllenBinTreeMapHdrTest::key_type, tree_leaf_node >                        tree_internal_node ;
        typedef cc::ellen_bintree::update_desc<tree_leaf_node, tree_internal_node>                                          tree_update_desc ;

        struct print_stat {
            template <typename Tree>
            void operator()( Tree const& t)
            {
                std::cout << t.statistics() ;
            }
        };

    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_less()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::less< less >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_cmp()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::compare< compare >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_cmpless()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::compare< compare >
                ,co::less< less >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_less_ic()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::less< less >
                ,co::item_counter< cds::atomicity::item_counter >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_cmp_ic()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::item_counter< cds::atomicity::item_counter >
                ,co::compare< compare >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_less_stat()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::less< less >
                ,co::stat< cc::ellen_bintree::stat >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_cmp_ic_stat()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::item_counter< cds::atomicity::item_counter >
                ,co::stat< cc::ellen_bintree::stat >
                ,co::compare< compare >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_less_pool()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::less< less >
                ,co::node_allocator< cds::memory::pool_allocator< tree_internal_node, ellen_bintree_rcu::internal_node_pool_accessor > >
                ,cc::ellen_bintree::update_desc_allocator< cds::memory::pool_allocator< tree_update_desc, ellen_bintree_rcu::update_desc_pool_accessor > >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

    void EllenBinTreeMapHdrTest::EllenBinTree_rcu_gpb_less_pool_ic_stat()
    {
        typedef cc::EllenBinTreeMap< rcu_type, key_type, value_type,
            cc::ellen_bintree::make_map_traits<
                co::less< less >
                ,co::node_allocator< cds::memory::pool_allocator< tree_internal_node, ellen_bintree_rcu::internal_node_pool_accessor > >
                ,cc::ellen_bintree::update_desc_allocator< cds::memory::pool_allocator< tree_update_desc, ellen_bintree_rcu::update_desc_pool_accessor > >
                ,co::item_counter< cds::atomicity::item_counter >
                ,co::stat< cc::ellen_bintree::stat >
            >::type
        > set_type  ;

        test<set_type, print_stat>() ;
    }

} // namespace tree
