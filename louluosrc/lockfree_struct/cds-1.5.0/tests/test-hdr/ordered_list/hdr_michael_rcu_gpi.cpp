/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "ordered_list/hdr_michael.h"
#include <cds/urcu/general_instant.h>
#include <cds/container/michael_list_rcu.h>

namespace ordlist {
    namespace {
        typedef cds::urcu::gc< cds::urcu::general_instant<> >    rcu_type ;

        struct RCU_GPI_cmp_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::cmp<MichaelListTestHeader::item>   compare ;
        };
    }

    void MichaelListTestHeader::RCU_GPI_cmp()
    {
        // traits-based version
        typedef cc::MichaelList< rcu_type, item, RCU_GPI_cmp_traits > list   ;
        test< list >()  ;

        // option-based version

        typedef cc::MichaelList< rcu_type, item,
            cc::michael_list::make_traits<
                cc::opt::compare< cmp<item> >
            >::type
        > opt_list   ;
        test< opt_list >()  ;
    }

    namespace {
        struct RCU_GPI_less_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::lt<MichaelListTestHeader::item>   less ;
        };
    }
    void MichaelListTestHeader::RCU_GPI_less()
    {
        // traits-based version
        typedef cc::MichaelList< rcu_type, item, RCU_GPI_less_traits > list   ;
        test< list >()  ;

        // option-based version

        typedef cc::MichaelList< rcu_type, item,
            cc::michael_list::make_traits<
                cc::opt::less< lt<item> >
            >::type
        > opt_list   ;
        test< opt_list >()  ;
    }

    namespace {
        struct RCU_GPI_cmpmix_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::cmp<MichaelListTestHeader::item>   compare ;
            typedef MichaelListTestHeader::lt<MichaelListTestHeader::item>  less    ;
        };
    }
    void MichaelListTestHeader::RCU_GPI_cmpmix()
    {
        // traits-based version
        typedef cc::MichaelList< rcu_type, item, RCU_GPI_cmpmix_traits > list   ;
        test< list >()  ;

        // option-based version

        typedef cc::MichaelList< rcu_type, item,
            cc::michael_list::make_traits<
                cc::opt::compare< cmp<item> >
                ,cc::opt::less< lt<item> >
            >::type
        > opt_list   ;
        test< opt_list >()  ;
    }

    namespace {
        struct RCU_GPI_ic_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::lt<MichaelListTestHeader::item>   less ;
            typedef cds::atomicity::item_counter item_counter   ;
        };
    }
    void MichaelListTestHeader::RCU_GPI_ic()
    {
        // traits-based version
        typedef cc::MichaelList< rcu_type, item, RCU_GPI_ic_traits > list   ;
        test< list >()  ;

        // option-based version

        typedef cc::MichaelList< rcu_type, item,
            cc::michael_list::make_traits<
                cc::opt::less< lt<item> >
                ,cc::opt::item_counter< cds::atomicity::item_counter >
            >::type
        > opt_list   ;
        test< opt_list >()  ;
    }

}   // namespace ordlist

