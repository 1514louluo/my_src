/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#include "set/hdr_intrusive_striped_set.h"
#include <cds/intrusive/striped_set/boost_splay_set.h>
#include <cds/intrusive/striped_set.h>

namespace set {
    namespace bi = boost::intrusive ;

    namespace {
        typedef IntrusiveStripedSetHdrTest::base_item< bi::splay_set_base_hook<> > base_item_type ;
        typedef IntrusiveStripedSetHdrTest::member_item< bi::splay_set_member_hook<> > member_item_type ;
    }

    void IntrusiveStripedSetHdrTest::Striped_splay_set_basehook()
    {
        typedef ci::StripedSet<
            bi::splay_set<base_item_type, bi::compare<IntrusiveStripedSetHdrTest::less<base_item_type> > >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::mutex_policy< ci::striped_set::striping<> >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_splay_set_basehook_bucket_threshold()
    {
        typedef ci::StripedSet<
            bi::splay_set<base_item_type, bi::compare<IntrusiveStripedSetHdrTest::less<base_item_type> > >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<64> >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_splay_set_basehook_bucket_threshold_rt()
    {
        typedef ci::StripedSet<
            bi::splay_set<base_item_type, bi::compare<IntrusiveStripedSetHdrTest::less<base_item_type> > >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<0> >
        > set_type ;

        set_type s( 64, ci::striped_set::single_bucket_size_threshold<0>(256) ) ;
        test_with( s ) ;
    }

    void IntrusiveStripedSetHdrTest::Striped_splay_set_memberhook()
    {
        typedef ci::StripedSet<
            bi::splay_set<
                member_item_type
                , bi::member_hook< member_item_type, bi::splay_set_member_hook<>, &member_item_type::hMember>
                , bi::compare<IntrusiveStripedSetHdrTest::less<member_item_type> >
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_splay_set_memberhook_bucket_threshold()
    {
        typedef ci::StripedSet<
            bi::splay_set<
                member_item_type
                , bi::member_hook< member_item_type, bi::splay_set_member_hook<>, &member_item_type::hMember>
                , bi::compare<IntrusiveStripedSetHdrTest::less<member_item_type> >
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<256> >
        > set_type ;

        test<set_type>() ;
    }

    void IntrusiveStripedSetHdrTest::Striped_splay_set_memberhook_bucket_threshold_rt()
    {
        typedef ci::StripedSet<
            bi::splay_set<
                member_item_type
                , bi::member_hook< member_item_type, bi::splay_set_member_hook<>, &member_item_type::hMember>
                , bi::compare<IntrusiveStripedSetHdrTest::less<member_item_type> >
            >
            ,co::hash< IntrusiveStripedSetHdrTest::hash_int >
            ,co::resizing_policy< ci::striped_set::single_bucket_size_threshold<0> >
        > set_type ;

        set_type s( 64, ci::striped_set::single_bucket_size_threshold<0>(256) ) ;
        test_with( s ) ;
    }

} // namespace set


