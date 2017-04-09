/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "tree/hdr_intrusive_bintree.h"
#include "tree/hdr_intrusive_ellen_bintree_pool_rcu.h"
#include "tree/hdr_intrusive_ellen_bintree_pool_hp.h"
#include "tree/hdr_intrusive_ellen_bintree_pool_ptb.h"

#include "tree/hdr_ellenbintree_set.h"
#include "tree/hdr_ellenbintree_map.h"

namespace tree {
    namespace ellen_bintree_rcu {
        internal_node_pool_type s_InternalNodePool( 1024 * 8 ) ;
        update_desc_pool_type   s_UpdateDescPool ;
    }
    namespace ellen_bintree_hp {
        internal_node_pool_type s_InternalNodePool( 1024 * 8 ) ;
        update_desc_pool_type   s_UpdateDescPool ;
    }
    namespace ellen_bintree_ptb {
        internal_node_pool_type s_InternalNodePool( 1024 * 8 ) ;
        update_desc_pool_type   s_UpdateDescPool ;
    }
}


CPPUNIT_TEST_SUITE_REGISTRATION_(tree::IntrusiveBinTreeHdrTest, s_IntrusiveBinTreeHdrTest)      ;
CPPUNIT_TEST_SUITE_REGISTRATION_(tree::EllenBinTreeSetHdrTest, s_EllenBinTreeSetHdrTest)        ;
CPPUNIT_TEST_SUITE_REGISTRATION_(tree::EllenBinTreeMapHdrTest, s_EllenBinTreeMapHdrTest)        ;
