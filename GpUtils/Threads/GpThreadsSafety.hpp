#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

#include <mutex>

#if defined(GP_LOCK_TRACE)
#   include <GpCore2/GpUtils/SyncPrimitives/GpLockTrace.hpp>
#endif// #if defined(GP_LOCK_TRACE)

#if defined(GP_COMPILER_CLANG) && !defined(GP_OS_BROWSER)
#   define THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x))

#   define CAPABILITY(x) \
     THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#   define SCOPED_CAPABILITY \
     THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#   define GUARDED_BY(x) \
     THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))

#   define PT_GUARDED_BY(x) \
     THREAD_ANNOTATION_ATTRIBUTE__(pt_guarded_by(x))

#   define ACQUIRED_AFTER(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(__VA_ARGS__))

#   define ACQUIRED_BEFORE(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#   define ACQUIRE(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#   define ACQUIRE_SHARED(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#   define TRY_ACQUIRE(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#   define TRY_ACQUIRE_SHARED(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

#   define RELEASE(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#   define RELEASE_SHARED(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#   define RELEASE_GENERIC(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(release_generic_capability(__VA_ARGS__))

#   define REQUIRES(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#   define REQUIRES_SHARED(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

#   define NO_THREAD_SAFETY_ANALYSIS \
     THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)

#   define EXCLUDES(...) \
     THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#   define ASSERT_CAPABILITY(x) \
     THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(x))

#   define ASSERT_SHARED_CAPABILITY(x) \
     THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(x))

#   define RETURN_CAPABILITY(x) \
     THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))
#else
#   define CAPABILITY(x)
#   define SCOPED_CAPABILITY
#   define GUARDED_BY(x)
#   define PT_GUARDED_BY(x)
#   define ACQUIRED_AFTER(...)
#   define ACQUIRED_BEFORE(...)
#   define ACQUIRE(...)
#   define ACQUIRE_SHARED(...)
#   define TRY_ACQUIRE(...)
#   define TRY_ACQUIRE_SHARED(...)
#   define RELEASE(...)
#   define RELEASE_SHARED(...)
#   define RELEASE_GENERIC(...)
#   define REQUIRES(...)
#   define REQUIRES_SHARED(...)
#   define NO_THREAD_SAFETY_ANALYSIS
#   define EXCLUDES(...)
#   define ASSERT_CAPABILITY(x)
#   define ASSERT_SHARED_CAPABILITY(x)
#   define RETURN_CAPABILITY(x)
#endif

namespace GPlatform::ThreadSafety {

enum class LockTraceModeE
{
    TRACE_ENABLED,
    TRACE_DISABLED
};

template<class T, LockTraceModeE LTM = LockTraceModeE::TRACE_ENABLED>
class CAPABILITY("sync_primitive_wrap") SyncPrimitiveWrap
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(SyncPrimitiveWrap)
    CLASS_DD(SyncPrimitiveWrap)

    using internal_type = T;

public:
                        SyncPrimitiveWrap               (void)                                                  {}
                        ~SyncPrimitiveWrap              (void) noexcept                                         {}

    void                lock                            (void) ACQUIRE()                                        {       iSyncPrimitiveInternal.lock();}
    void                unlock                          (void) RELEASE()                                        {       iSyncPrimitiveInternal.unlock();}
    bool                try_lock                        (void) TRY_ACQUIRE(true)                                {return iSyncPrimitiveInternal.try_lock();}

    internal_type&      internal                        (void) const noexcept RETURN_CAPABILITY(iSyncPrimitiveInternal) {return iSyncPrimitiveInternal;}

    static constexpr    LockTraceModeE LockTraceMode    (void) noexcept {return LTM;}

private:
    mutable internal_type   iSyncPrimitiveInternal;
};

template<class T, LockTraceModeE LTM = LockTraceModeE::TRACE_ENABLED>
class CAPABILITY("shared_sync_primitive_wrap") SharedSyncPrimitiveWrap
{
public:
    using internal_type = T;

public:
                        SharedSyncPrimitiveWrap         (void)                                                  {}
                        ~SharedSyncPrimitiveWrap        (void) noexcept                                         {}

    void                lock                            (void) ACQUIRE()                                        {iSyncPrimitiveInternal.lock();}
    void                unlock                          (void) RELEASE()                                        {iSyncPrimitiveInternal.unlock();}
    bool                try_lock                        (void) TRY_ACQUIRE(true)                                {return iSyncPrimitiveInternal.try_lock();}

    void                lock_shared                     (void) ACQUIRE_SHARED()                                 {iSyncPrimitiveInternal.lock_shared();}
    void                unlock_shared                   (void) RELEASE_SHARED()                                 {iSyncPrimitiveInternal.unlock_shared();}
    bool                try_lock_shared                 (void) TRY_ACQUIRE_SHARED(true)                         {return iSyncPrimitiveInternal.try_lock_shared();}

    internal_type&      internal                        (void) const noexcept RETURN_CAPABILITY(iSyncPrimitiveInternal) {return iSyncPrimitiveInternal;}

    static constexpr    LockTraceModeE LockTraceMode    (void) noexcept {return LTM;}

private:
    mutable internal_type   iSyncPrimitiveInternal;
};

template<template<typename...> class LockerT, class SyncPrimitiveWrapT>
class SCOPED_CAPABILITY SyncPrimitiveLockerWrap
{
public:
    using SyncPrimitiveInternalT    = typename SyncPrimitiveWrapT::internal_type;
    using LockerInternalT           = LockerT<SyncPrimitiveInternalT>;

public:
                        SyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap) ACQUIRE(aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal())
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLock(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        SyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap, std::adopt_lock_t) REQUIRES(aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal(), std::adopt_lock)
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLock(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        SyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap, std::defer_lock_t) EXCLUDES(aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal(), std::defer_lock)
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLock(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        SyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap, std::try_to_lock_t) TRY_ACQUIRE(true, aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal(), std::try_to_lock)
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLock(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        ~SyncPrimitiveLockerWrap (void) RELEASE()
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnUnlock(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

    void                lock (void) ACQUIRE()
    {
        iLockerInternal.lock();

#if defined(GP_LOCK_TRACE)
        if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
        {
            GpLockTrace::S().OnLock(iLockerInternal.mutex(), this);
        }
#endif// #if defined(GP_LOCK_TRACE)
    }

    bool                try_lock (void) TRY_ACQUIRE(true)
    {
        const bool isLocked = iLockerInternal.try_lock();

#if defined(GP_LOCK_TRACE)
        if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
        {
            if (isLocked) [[likely]]
            {
                GpLockTrace::S().OnLock(iLockerInternal.mutex(), this);
            }
        }
#endif// #if defined(GP_LOCK_TRACE)

        return isLocked;
    }

    void                unlock (void) RELEASE()
    {
#if defined(GP_LOCK_TRACE)
        if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
        {
            if (iLockerInternal.owns_lock())
            {
                GpLockTrace::S().OnUnlock(iLockerInternal.mutex(), this);
            }
        }
#endif// #if defined(GP_LOCK_TRACE)

        iLockerInternal.unlock();
    }

    bool                owns_lock (void) const noexcept
    {
        return iLockerInternal.owns_lock();
    }

    LockerInternalT&    internal_lock (void) noexcept RETURN_CAPABILITY(iLockerInternal)
    {
        return iLockerInternal;
    }

private:
    LockerInternalT     iLockerInternal;
};

template<template<typename...> class LockerT, class SyncPrimitiveWrapT>
class SCOPED_CAPABILITY SharedSyncPrimitiveLockerWrap
{
public:
    using SyncPrimitiveInternalT    = typename SyncPrimitiveWrapT::internal_type;
    using LockerInternalT           = LockerT<SyncPrimitiveInternalT>;

public:
                        SharedSyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap) ACQUIRE_SHARED(aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal())
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLockShared(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        SharedSyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap, std::adopt_lock_t) REQUIRES_SHARED(aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal(), std::adopt_lock)
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLockShared(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        SharedSyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap, std::defer_lock_t) EXCLUDES(aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal(), std::defer_lock)
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLockShared(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        SharedSyncPrimitiveLockerWrap (SyncPrimitiveWrapT& aSyncPrimitiveWrap, std::try_to_lock_t) TRY_ACQUIRE_SHARED(true, aSyncPrimitiveWrap):
                            iLockerInternal(aSyncPrimitiveWrap.internal(), std::try_to_lock)
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnLockShared(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

                        ~SharedSyncPrimitiveLockerWrap (void) RELEASE()
                        {
#if defined(GP_LOCK_TRACE)
                            if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
                            {
                                if (iLockerInternal.owns_lock())
                                {
                                    GpLockTrace::S().OnUnlock(iLockerInternal.mutex(), this);
                                }
                            }
#endif// #if defined(GP_LOCK_TRACE)
                        }

    void                lock (void) ACQUIRE_SHARED()
    {
        iLockerInternal.lock();

#if defined(GP_LOCK_TRACE)
        if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
        {
            GpLockTrace::S().OnLockShared(iLockerInternal.mutex(), this);
        }
#endif// #if defined(GP_LOCK_TRACE)
    }

    bool                try_lock (void) TRY_ACQUIRE_SHARED(true)
    {
        const bool isLocked = iLockerInternal.try_lock();

#if defined(GP_LOCK_TRACE)
        if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
        {
            if (isLocked) [[likely]]
            {
                GpLockTrace::S().OnLock(iLockerInternal.mutex(), this);
            }
        }
#endif// #if defined(GP_LOCK_TRACE)

        return isLocked;
    }   

    void                unlock (void) RELEASE()
    {
#if defined(GP_LOCK_TRACE)
        if constexpr (SyncPrimitiveWrapT::LockTraceMode() == LockTraceModeE::TRACE_ENABLED)
        {
            if (iLockerInternal.owns_lock())
            {
                GpLockTrace::S().OnUnlock(iLockerInternal.mutex(), this);
            }
        }
#endif// #if defined(GP_LOCK_TRACE)

        iLockerInternal.unlock();
    }

    bool                owns_lock (void) const noexcept
    {
        return iLockerInternal.owns_lock();
    }

    //LockerInternalT&  internal_lock (void) noexcept RETURN_CAPABILITY(iLockerInternal)
    //{
    //  return iLockerInternal;
    //}

private:
    LockerInternalT     iLockerInternal;
};

}// namespace GPlatform::ThreadSafety
