/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "set/hdr_intrusive_striped_set.h"
#include <cds/intrusive/striped_set/boost_slist.h>
#include <cds/intrusive/striped_set.h>

#include <cds/details/std/type_traits.h>    // std::is_same

namespace set {
    namespace bi = boost::intrusive ;

    namespace {
        typedef IntrusiveStripedSetHdrTest::base_item< bi::slist_base_hook<> > base_item_type ;
        typedef IntrusiveStripedSetHdrTest::member_item< bi::slist_member_hook<> > member_item_type ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_basehook_cmp()
    {
        typedef ci::StripedSet<
            bi::slist<base_item_type>
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<base_item_type> >
            ,co::mutex_policy< ci::striped_set::striping<> >
        > set_type ;

        static_assert( (std::is_same<
            IntrusiveStripedSetHdrTest::cmp<base_item_type>
            ,set_type::bucket_type::key_comparator
        >::value), "Key compare function selection error" ) ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_basehook_less()
    {
        typedef ci::StripedSet<
            bi::slist<base_item_type>
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<base_item_type> >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_basehook_cmpmix()
    {
        typedef ci::StripedSet<
            bi::slist<base_item_type>
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<base_item_type> >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<base_item_type> >
        > set_type ;

        static_assert( (std::is_same<
            IntrusiveStripedSetHdrTest::cmp<base_item_type>
            ,set_type::bucket_type::key_comparator
        >::value), "Key compare function selection error" ) ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_basehook_bucket_threshold()
    {
        typedef ci::StripedSet<
            bi::slist<base_item_type>
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<base_item_type> >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<base_item_type> >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<8> >
        > set_type ;

        static_assert( (std::is_same<
            IntrusiveStripedSetHdrTest::cmp<base_item_type>
            ,set_type::bucket_type::key_comparator
        >::value), "Key compare function selection error" ) ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_basehook_bucket_threshold_rt()
    {
        typedef ci::StripedSet<
            bi::slist<base_item_type>
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<base_item_type> >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<base_item_type> >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<0> >
        > set_type ;

        static_assert( (std::is_same<
            IntrusiveStripedSetHdrTest::cmp<base_item_type>
            ,set_type::bucket_type::key_comparator
        >::value), "Key compare function selection error" ) ;

        set_type s( 128, ci::striped_set::single_bucket_size_threshold<0>(4) ) ;
        test_with( s ) ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_memberhook_cmp()
    {
        typedef ci::StripedSet<
            bi::slist<
                member_item_type
                , bi::member_hook< member_item_type, bi::slist_member_hook<>, &member_item_type::hMember>
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<member_item_type> >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_memberhook_less()
    {
        typedef ci::StripedSet<
            bi::slist<
                member_item_type
                , bi::member_hook< member_item_type, bi::slist_member_hook<>, &member_item_type::hMember>
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<member_item_type> >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_memberhook_cmpmix()
    {
        typedef ci::StripedSet<
            bi::slist<
                member_item_type
                , bi::member_hook< member_item_type, bi::slist_member_hook<>, &member_item_type::hMember>
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<member_item_type> >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<member_item_type> >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_memberhook_bucket_threshold()
    {
        typedef ci::StripedSet<
            bi::slist<
                member_item_type
                , bi::member_hook< member_item_type, bi::slist_member_hook<>, &member_item_type::hMember>
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<member_item_type> >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<member_item_type> >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<8> >
        > set_type ;

        static_assert( (std::is_same<
            IntrusiveStripedSetHdrTest::cmp<member_item_type>
            ,set_type::bucket_type::key_comparator
        >::value), "Key compare function selection error" ) ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_slist_memberhook_bucket_threshold_rt()
    {
        typedef ci::StripedSet<
            bi::slist<
            member_item_type
            , bi::member_hook< member_item_type, bi::slist_member_hook<>, &member_item_type::hMember>
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::less< IntrusiveStripedSetHdrTest::less<member_item_type> >
            ,co::compare< IntrusiveStripedSetHdrTest::cmp<member_item_type> >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<0> >
        > set_type ;

        static_assert( (std::is_same<
            IntrusiveStripedSetHdrTest::cmp<member_item_type>
            ,set_type::bucket_type::key_comparator
        >::value), "Key compare function selection error" ) ;

        set_type s( 128, ci::striped_set::single_bucket_size_threshold<0>(4) ) ;
        test_with( s ) ;
    }

} // namespace set


