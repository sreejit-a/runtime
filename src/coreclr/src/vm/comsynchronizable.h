// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.


/*============================================================
**
** Header: COMSynchronizable.h
**
** Purpose: Native methods on System.SynchronizableObject
**          and its subclasses.
**
**
===========================================================*/

#ifndef _COMSYNCHRONIZABLE_H
#define _COMSYNCHRONIZABLE_H

#include "field.h"          // For FieldDesc definition.

//
// Each function that we call through native only gets one argument,
// which is actually a pointer to its stack of arguments.  Our structs
// for accessing these are defined below.
//

struct SharedState;

class ThreadNative
{
friend class ThreadBaseObject;

public:

    enum
    {
        PRIORITY_LOWEST = 0,
        PRIORITY_BELOW_NORMAL = 1,
        PRIORITY_NORMAL = 2,
        PRIORITY_ABOVE_NORMAL = 3,
        PRIORITY_HIGHEST = 4,
    };

    enum
    {
        ThreadStopRequested = 1,
        ThreadSuspendRequested = 2,
        ThreadBackground = 4,
        ThreadUnstarted = 8,
        ThreadStopped = 16,
        ThreadWaitSleepJoin = 32,
        ThreadSuspended = 64,
        ThreadAbortRequested = 128,
        ThreadAborted = 256,
    };

    enum
    {
        ApartmentSTA = 0,
        ApartmentMTA = 1,
        ApartmentUnknown = 2
    };

    static LPVOID F_CALL_CONV FastGetCurrentThread();
    static LPVOID F_CALL_CONV FastGetDomain();

    static void StartInner(ThreadBaseObject* pThisUNSAFE, StackCrawlMark* pStackMark);

    static FCDECL1(void, Abort, ThreadBaseObject* pThis);
    static FCDECL1(void, ResetAbort, ThreadBaseObject* pThis);
    static FCDECL2(void,    Start,             ThreadBaseObject* pThisUNSAFE, StackCrawlMark* pStackMark);
#ifndef FEATURE_CORECLR
    static FCDECL1(void,    Suspend,           ThreadBaseObject* pThisUNSAFE);
    static FCDECL1(void,    Resume,            ThreadBaseObject* pThisUNSAFE);
#endif // FEATURE_CORECLR
    static FCDECL1(INT32,   GetPriority,       ThreadBaseObject* pThisUNSAFE);
    static FCDECL2(void,    SetPriority,       ThreadBaseObject* pThisUNSAFE, INT32 iPriority);
    static FCDECL1(void,    Interrupt,         ThreadBaseObject* pThisUNSAFE);
    static FCDECL1(FC_BOOL_RET, IsAlive,       ThreadBaseObject* pThisUNSAFE);
    static FCDECL2(FC_BOOL_RET, Join,          ThreadBaseObject* pThisUNSAFE, INT32 Timeout);
#undef Sleep
    static FCDECL1(void,    Sleep,             INT32 iTime);
#define Sleep(a) Dont_Use_Sleep(a)
    static FCDECL3(void,    SetStart,          ThreadBaseObject* pThisUNSAFE, Object* pDelegateUNSAFE, INT32 iRequestedStackSize);
    static FCDECL2(void,    SetBackground,     ThreadBaseObject* pThisUNSAFE, CLR_BOOL isBackground);
    static FCDECL1(FC_BOOL_RET, IsBackground,  ThreadBaseObject* pThisUNSAFE);
    static FCDECL1(INT32,   GetThreadState,    ThreadBaseObject* pThisUNSAFE);
    static FCDECL1(INT32,   GetThreadContext,  ThreadBaseObject* pThisUNSAFE);
#ifdef FEATURE_COMINTEROP_APARTMENT_SUPPORT
    static FCDECL1(INT32,   GetApartmentState, ThreadBaseObject* pThis);
    static FCDECL3(INT32,   SetApartmentState, ThreadBaseObject* pThisUNSAFE, INT32 iState, CLR_BOOL fireMDAOnMismatch);
    static FCDECL1(void,    StartupSetApartmentState, ThreadBaseObject* pThis);
#endif // FEATURE_COMINTEROP_APARTMENT_SUPPORT
    static FCDECL0(Object*, GetDomain);
#ifdef FEATURE_REMOTING
    static FCDECL1(Object*, GetContextFromContextID,        LPVOID ContextID);
    static FCDECL6(Object*, InternalCrossContextCallback, ThreadBaseObject* refThis, ContextBaseObject* refContext, LPVOID contextID, INT32 appDomainId, Object* oDelegateUNSAFE, PtrArray* oArgsUNSAFE);
#endif    
#ifdef FEATURE_LEAK_CULTURE_INFO
    static FCDECL4(FC_BOOL_RET, nativeGetSafeCulture, ThreadBaseObject* threadUNSAFE, int appDomainId, CLR_BOOL isUI, OBJECTREF *safeCulture);
#else
    static void QCALLTYPE nativeInitCultureAccessors();
#endif

    static
    void QCALLTYPE InformThreadNameChange(QCall::ThreadHandle thread, LPCWSTR name, INT32 len);

    static
    UINT64 QCALLTYPE GetProcessDefaultStackSize();

    static FCDECL1(INT32,   GetManagedThreadId, ThreadBaseObject* th);
#ifndef FEATURE_CORECLR
    static FCDECL0(void,    BeginCriticalRegion);
    static FCDECL0(void,    EndCriticalRegion);
    static FCDECL0(void,    BeginThreadAffinity);
    static FCDECL0(void,    EndThreadAffinity);
#endif // !FEATURE_CORECLR
    static FCDECL1(void,    SpinWait,                       int iterations);
    static BOOL QCALLTYPE YieldThread();
    static FCDECL0(Object*, GetCurrentThread);
    static FCDECL1(void,    Finalize,                       ThreadBaseObject* pThis);
#ifdef FEATURE_COMINTEROP
    static FCDECL1(void,    DisableComObjectEagerCleanup,   ThreadBaseObject* pThis);
#endif //FEATURE_COMINTEROP
#ifdef FEATURE_LEAK_CULTURE_INFO
    static FCDECL1(FC_BOOL_RET,SetThreadUILocale,              StringObject* localeNameUNSAFE);
#endif // FEATURE_LEAK_CULTURE_INFO
    static FCDECL1(FC_BOOL_RET,IsThreadpoolThread,             ThreadBaseObject* thread);
#ifdef FEATURE_COMPRESSEDSTACK    
    static FCDECL2(void*,    SetAppDomainStack, ThreadBaseObject* pThis, SafeHandle* hcsUNSAFE);
    static FCDECL2(void,    RestoreAppDomainStack, ThreadBaseObject* pThis, void* appDomainStack);
#endif // #ifdef FEATURE_COMPRESSEDSTACK

    static FCDECL0(void, FCMemoryBarrier);
    static FCDECL1(void, SetIsThreadStaticsArray, Object* pObject);

    static FCDECL2(void,    SetAbortReason, ThreadBaseObject* pThisUNSAFE, Object* pObject);
#ifndef FEATURE_CORECLR	
    static FCDECL1(Object*, GetAbortReason, ThreadBaseObject* pThisUNSAFE);
#endif
    static FCDECL1(void,    ClearAbortReason, ThreadBaseObject* pThisUNSAFE);

private:

    struct KickOffThread_Args {
        Thread *pThread;
        SharedState *share;
        ULONG retVal;
    };

    static void KickOffThread_Worker(LPVOID /* KickOffThread_Args* */);
    static ULONG __stdcall KickOffThread(void *pass);
    static BOOL DoJoin(THREADBASEREF DyingThread, INT32 timeout);
};


#endif // _COMSYNCHRONIZABLE_H

