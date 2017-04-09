/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "ellen_bintree_update_desc_pool.h"

namespace ellen_bintree_pool {
    update_desc_pool_type s_UpdateDescPool ;

    bounded_update_desc_pool_type s_BoundedUpdateDescPool ;

    cds::atomicity::event_counter   internal_node_counter::m_nAlloc ;
    cds::atomicity::event_counter   internal_node_counter::m_nFree  ;
}
