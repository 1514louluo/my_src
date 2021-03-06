/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "ordered_list/hdr_michael.h"
#include <cds/container/michael_list_nogc.h>

namespace ordlist {
    namespace {
        struct NOGC_cmp_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::cmp<MichaelListTestHeader::item>   compare ;
        };
    }
    void MichaelListTestHeader::NOGC_cmp()
    {
        // traits-based version
        typedef cc::MichaelList< cds::gc::nogc, item, NOGC_cmp_traits > list   ;
        nogc_test< list >()  ;

        // option-based version

        typedef cc::MichaelList< cds::gc::nogc, item,
            cc::michael_list::make_traits<
                cc::opt::compare< cmp<item> >
            >::type
        > opt_list   ;
        nogc_test< opt_list >()  ;
    }

    namespace {
        struct NOGC_less_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::lt<MichaelListTestHeader::item>   less ;
        };
    }
    void MichaelListTestHeader::NOGC_less()
    {
        // traits-based version
        typedef cc::MichaelList< cds::gc::nogc, item, NOGC_less_traits > list   ;
        nogc_test< list >()  ;

        // option-based version

        typedef cc::MichaelList< cds::gc::nogc, item,
            cc::michael_list::make_traits<
                cc::opt::less< lt<item> >
            >::type
        > opt_list   ;
        nogc_test< opt_list >()  ;
    }

    namespace {
        struct NOGC_cmpmix_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::cmp<MichaelListTestHeader::item>   compare ;
            typedef MichaelListTestHeader::lt<MichaelListTestHeader::item>  less    ;
        };
    }
    void MichaelListTestHeader::NOGC_cmpmix()
    {
        // traits-based version
        typedef cc::MichaelList< cds::gc::nogc, item, NOGC_cmpmix_traits > list   ;
        nogc_test< list >()  ;

        // option-based version

        typedef cc::MichaelList< cds::gc::nogc, item,
            cc::michael_list::make_traits<
                cc::opt::compare< cmp<item> >
                ,cc::opt::less< lt<item> >
            >::type
        > opt_list   ;
        nogc_test< opt_list >()  ;
    }

    namespace {
        struct NOGC_ic_traits: public cc::michael_list::type_traits
        {
            typedef MichaelListTestHeader::lt<MichaelListTestHeader::item>   less ;
            typedef cds::atomicity::item_counter item_counter   ;
        };
    }
    void MichaelListTestHeader::NOGC_ic()
    {
        // traits-based version
        typedef cc::MichaelList< cds::gc::nogc, item, NOGC_ic_traits > list   ;
        nogc_test< list >()  ;

        // option-based version

        typedef cc::MichaelList< cds::gc::nogc, item,
            cc::michael_list::make_traits<
                cc::opt::less< lt<item> >
                ,cc::opt::item_counter< cds::atomicity::item_counter >
            >::type
        > opt_list   ;
        nogc_test< opt_list >()  ;
    }

}   // namespace ordlist

