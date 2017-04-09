/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf@gmail.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.5.0
*/


#ifndef _CDS_URCU_DETAILS_SH_H
#define _CDS_URCU_DETAILS_SH_H

#include <cds/urcu/details/sh_decl.h>

#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
#include <cds/threading/model.h>

//@cond
namespace cds { namespace urcu { namespace details {

    // Inlines

    // sh_thread_gc
    template <typename RCUtag>
    inline sh_thread_gc<RCUtag>::sh_thread_gc()
    {
        if ( !threading::Manager::isThreadAttached() )
            cds::threading::Manager::attachThread() ;
    }

    template <typename RCUtag>
    inline sh_thread_gc<RCUtag>::~sh_thread_gc()
    {
        cds::threading::Manager::detachThread() ;
    }

    template <typename RCUtag>
    inline typename sh_thread_gc<RCUtag>::thread_record * sh_thread_gc<RCUtag>::get_thread_record()
    {
        return cds::threading::getRCU<RCUtag>() ;
    }

    template <typename RCUtag>
    inline void sh_thread_gc<RCUtag>::access_lock()
    {
        thread_record * pRec = get_thread_record() ;
        assert( pRec != null_ptr<thread_record *>()) ;

        uint32_t tmp = pRec->m_nAccessControl.load( CDS_ATOMIC::memory_order_relaxed ) ;
        if ( (tmp & rcu_class::c_nNestMask) == 0 ) {
            pRec->m_nAccessControl.store(
                sh_singleton<RCUtag>::instance()->global_control_word(CDS_ATOMIC::memory_order_acquire),
                CDS_ATOMIC::memory_order_release
            ) ;
        }
        else {
            pRec->m_nAccessControl.fetch_add( 1, CDS_ATOMIC::memory_order_release ) ;
        }
        CDS_COMPILER_RW_BARRIER ;
    }

    template <typename RCUtag>
    inline void sh_thread_gc<RCUtag>::access_unlock()
    {
        thread_record * pRec = get_thread_record() ;
        assert( pRec != null_ptr<thread_record *>()) ;

        CDS_COMPILER_RW_BARRIER ;
        pRec->m_nAccessControl.fetch_sub( 1, CDS_ATOMIC::memory_order_release )   ;
    }

    template <typename RCUtag>
    inline bool sh_thread_gc<RCUtag>::is_locked()
    {
        thread_record * pRec = get_thread_record() ;
        assert( pRec != null_ptr<thread_record *>()) ;

        return (pRec->m_nAccessControl.load( CDS_ATOMIC::memory_order_relaxed ) & rcu_class::c_nNestMask) != 0 ;
    }


    // sh_singleton
    template <typename RCUtag>
    inline void sh_singleton<RCUtag>::set_signal_handler()
    {
        //TODO: OS-specific code must be moved to cds::OS namespace
        struct sigaction sigact ;
        memset( &sigact, 0, sizeof(sigact)) ;
        sigact.sa_sigaction = signal_handler    ;
        sigact.sa_flags = SA_SIGINFO    ;
        sigemptyset( &sigact.sa_mask )  ;
        //sigaddset( &sigact.sa_mask, m_nSigNo )  ;
        sigaction( m_nSigNo, &sigact, null_ptr<struct sigaction *>() ) ;

        sigaddset( &sigact.sa_mask, m_nSigNo )  ;
        pthread_sigmask( SIG_UNBLOCK, &sigact.sa_mask, NULL )   ;
    }

    template <typename RCUtag>
    inline void sh_singleton<RCUtag>::clear_signal_handler()
    {}

    template <typename RCUtag>
    void sh_singleton<RCUtag>::signal_handler( int signo, siginfo_t * sigInfo, void * context )
    {
        thread_record * pRec = cds::threading::getRCU<RCUtag>() ;
        if ( pRec ) {
            CDS_ATOMIC::atomic_signal_fence( CDS_ATOMIC::memory_order_acquire ) ;
            pRec->m_bNeedMemBar.store( false, CDS_ATOMIC::memory_order_relaxed ) ;
            CDS_ATOMIC::atomic_signal_fence( CDS_ATOMIC::memory_order_release ) ;
        }
    }

    template <typename RCUtag>
    inline void sh_singleton<RCUtag>::raise_signal( cds::OS::ThreadId tid )
    {
        pthread_kill( tid, m_nSigNo ) ;
    }

    template <typename RCUtag>
    template <class Backoff>
    inline void sh_singleton<RCUtag>::force_membar_all_threads( Backoff& bkOff )
    {
        OS::ThreadId const nullThreadId = OS::nullThreadId() ;

        // Send "need membar" signal to all RCU threads
        for ( thread_record * pRec = m_ThreadList.head( CDS_ATOMIC::memory_order_acquire); pRec; pRec = pRec->m_list.m_pNext ) {
            OS::ThreadId tid = pRec->m_list.m_idOwner.load( CDS_ATOMIC::memory_order_acquire) ;
            if ( tid != nullThreadId ) {
                pRec->m_bNeedMemBar.store( true, CDS_ATOMIC::memory_order_release ) ;
                raise_signal( tid )  ;
            }
        }

        // Wait while all RCU threads process the signal
        for ( thread_record * pRec = m_ThreadList.head( CDS_ATOMIC::memory_order_acquire); pRec; pRec = pRec->m_list.m_pNext ) {
            OS::ThreadId tid = pRec->m_list.m_idOwner.load( CDS_ATOMIC::memory_order_acquire) ;
            if ( tid != nullThreadId ) {
                bkOff.reset()   ;
                while ( pRec->m_bNeedMemBar.load( CDS_ATOMIC::memory_order_acquire )) {
                    // Some versions of OSes can lose signals
                    // So, we resend the signal
                    raise_signal( tid ) ;
                    bkOff() ;
                }
            }
        }
    }

    template <typename RCUtag>
    bool sh_singleton<RCUtag>::check_grace_period( thread_record * pRec ) const
    {
        uint32_t const v = pRec->m_nAccessControl.load( CDS_ATOMIC::memory_order_acquire ) ;
        return (v & signal_handling_rcu::c_nNestMask)
            && ((( v ^ m_nGlobalControl.load( CDS_ATOMIC::memory_order_relaxed )) & ~signal_handling_rcu::c_nNestMask )) ;
    }

    template <typename RCUtag>
    template <class Backoff>
    void sh_singleton<RCUtag>::wait_for_quiescent_state( Backoff& bkOff )
    {
        OS::ThreadId const nullThreadId = OS::nullThreadId() ;

        for ( thread_record * pRec = m_ThreadList.head( CDS_ATOMIC::memory_order_acquire); pRec; pRec = pRec->m_list.m_pNext ) {
            while ( pRec->m_list.m_idOwner.load( CDS_ATOMIC::memory_order_acquire) != nullThreadId && check_grace_period( pRec ))
                bkOff() ;
        }
    }

}}} // namespace cds:urcu::details
//@endcond

#endif // #ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
#endif // #ifndef _CDS_URCU_DETAILS_SH_H
