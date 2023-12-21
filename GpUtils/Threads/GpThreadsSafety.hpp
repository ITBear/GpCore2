#pragma once

#include "../../Config/GpEnvironmentDetector.hpp"

#include <mutex>
#include <shared_mutex>

#if defined(GP_COMPILER_CLANG)
#   define THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x))
#else
#   define THREAD_ANNOTATION_ATTRIBUTE__(x)
#endif

#define CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#define SCOPED_CAPABILITY \
  THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#define GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))

#define PT_GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(pt_guarded_by(x))

#define ACQUIRED_AFTER(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(__VA_ARGS__))

#define ACQUIRED_BEFORE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#define ACQUIRE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define ACQUIRE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define TRY_ACQUIRE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#define TRY_ACQUIRE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

#define RELEASE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define RELEASE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define RELEASE_GENERIC(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(release_generic_capability(__VA_ARGS__))

#define REQUIRES(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define REQUIRES_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

#define NO_THREAD_SAFETY_ANALYSIS \
  THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)

#define EXCLUDES(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#define ASSERT_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(x))

#define ASSERT_SHARED_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(x))

#define RETURN_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))


namespace GPlatform::ThreadSafety {

template<class T>
class CAPABILITY("mutex_wrap") MutexWrap
{
public:
    using internal_type = T;

public:
                        MutexWrap               (void)                                                  {}
                        ~MutexWrap              (void) noexcept                                         {}

    void                lock                    (void) ACQUIRE()                                        {       iMutexInternal.lock();}
    void                unlock                  (void) RELEASE()                                        {       iMutexInternal.unlock();}
    bool                try_lock                (void) TRY_ACQUIRE(true)                                {return iMutexInternal.try_lock();}

    internal_type&      internal                (void) const noexcept RETURN_CAPABILITY(iMutexInternal) {return iMutexInternal;}

private:
    mutable internal_type   iMutexInternal;
};

template<class T>
class CAPABILITY("shared_mutex_wrap") SharedMutexWrap
{
public:
    using internal_type = T;

public:
                        SharedMutexWrap         (void)                                                  {}
                        ~SharedMutexWrap        (void) noexcept                                         {}

    void                lock                    (void) ACQUIRE()                                        {iMutexInternal.lock();}
    void                unlock                  (void) RELEASE()                                        {iMutexInternal.unlock();}
    bool                try_lock                (void) TRY_ACQUIRE(true)                                {return iMutexInternal.try_lock();}

    void                lock_shared             (void) ACQUIRE_SHARED()                                 {iMutexInternal.lock_shared();}
    void                unlock_shared           (void) RELEASE_SHARED()                                 {iMutexInternal.unlock_shared();}
    bool                try_lock_shared         (void) TRY_ACQUIRE_SHARED(true)                         {return iMutexInternal.try_lock_shared();}

    internal_type&      internal                (void) const noexcept RETURN_CAPABILITY(iMutexInternal) {return iMutexInternal;}

private:
    mutable internal_type   iMutexInternal;
};

template<class MutexWrapT, template<typename...> class LockerT>
class SCOPED_CAPABILITY MutexLockerWrap
{
public:
    using MutexInternalT    = typename MutexWrapT::internal_type;
    using LockerInternalT   = LockerT<MutexInternalT>;

public:
                        MutexLockerWrap         (MutexWrapT& aMutexWrap)    ACQUIRE(aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal())                        {}
                        MutexLockerWrap         (MutexWrapT& aMutexWrap,
                                                 std::adopt_lock_t)         REQUIRES(aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal(), std::adopt_lock)       {}
                        MutexLockerWrap         (MutexWrapT& aMutexWrap,
                                                 std::defer_lock_t)         EXCLUDES(aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal(), std::defer_lock)       {}
                        MutexLockerWrap         (MutexWrapT& aMutexWrap,
                                                 std::try_to_lock_t)        TRY_ACQUIRE(true, aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal(), std::try_to_lock)      {}
                        ~MutexLockerWrap        (void)                      RELEASE()                           {}

    void                lock                    (void)                      ACQUIRE()                           {       iLockerInternal.lock();}
    bool                try_lock                (void)                      TRY_ACQUIRE(true)                   {return iLockerInternal.try_lock();}
    void                unlock                  (void)                      RELEASE()                           {       iLockerInternal.unlock();}
    bool                owns_lock               (void) const noexcept                                           {return iLockerInternal.owns_lock();}
    LockerInternalT&    internal_lock           (void) noexcept             RETURN_CAPABILITY(iLockerInternal)  {return iLockerInternal;}

private:
    LockerInternalT     iLockerInternal;
};

template<class MutexWrapT, template<typename...> class LockerT>
class SCOPED_CAPABILITY SharedMutexLockerWrap
{
public:
    using MutexInternalT    = typename MutexWrapT::internal_type;
    using LockerInternalT   = LockerT<MutexInternalT>;

public:
                        SharedMutexLockerWrap   (MutexWrapT& aMutexWrap)    ACQUIRE_SHARED(aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal())                        {}
                        SharedMutexLockerWrap   (MutexWrapT& aMutexWrap,
                                                 std::adopt_lock_t)         REQUIRES_SHARED(aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal(), std::adopt_lock)       {}
                        SharedMutexLockerWrap   (MutexWrapT& aMutexWrap,
                                                 std::defer_lock_t)         EXCLUDES(aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal(), std::defer_lock)       {}
                        SharedMutexLockerWrap   (MutexWrapT& aMutexWrap,
                                                 std::try_to_lock_t)        TRY_ACQUIRE_SHARED(true, aMutexWrap)
                                                : iLockerInternal(aMutexWrap.internal(), std::try_to_lock)      {}
                        ~SharedMutexLockerWrap  (void)                      RELEASE()                   {}

    void                lock                    (void)                      ACQUIRE_SHARED()                    {       iLockerInternal.lock();}
    bool                try_lock                (void)                      TRY_ACQUIRE_SHARED(true)            {return iLockerInternal.try_lock();}
    void                unlock                  (void)                      RELEASE()                           {       iLockerInternal.unlock();}
    LockerInternalT&    internal_lock           (void) noexcept             RETURN_CAPABILITY(iLockerInternal)  {return iLockerInternal;}

private:
    LockerInternalT     iLockerInternal;
};

}// namespace GPlatform::ThreadSafety
