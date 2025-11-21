#pragma once

#include <GpCore2/GpUtils/Types/Pointers/GpSharedPtr.hpp>

namespace GPlatform {

TAG_REGISTER(GpWeakPtr)

namespace Concepts::Ptr {

template <typename T>
concept IsWeakPtr = requires()
{
    requires GpHasTag_GpWeakPtr<T>();
};

}// namespace Concepts::Ptr

class _GpWeakPtrAccess;

template<typename T>
class GpWeakPtr
{
public:
    using this_type         = GpWeakPtr<T>;
    using value_type        = T;
    using const_value_type  = const std::remove_const_t<T>;
    using ControlBlockT     = GpSharedPtrControlBlock;

    TAG_SET(GpWeakPtr)

    static constexpr bool SIsConst  (void) noexcept {return std::is_const_v<value_type>;}

public:
                            GpWeakPtr           (ControlBlockT* aControlBlock,
                                                 GpMethodAccessGuard<_GpWeakPtrAccess>) noexcept;
                            GpWeakPtr           (void) noexcept;
                            GpWeakPtr           (const this_type& aWeakPtr) noexcept;
                            GpWeakPtr           (this_type&& aWeakPtr) noexcept;
                            GpWeakPtr           (std::nullptr_t) noexcept;
    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
                            GpWeakPtr           (const TSP& aWeakPtr) noexcept;
    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
                            GpWeakPtr           (const TSP& aSharedPtr) noexcept;

                            ~GpWeakPtr          (void) noexcept;

    void                    Clear               (void) noexcept;

    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
    void                    Set                 (const TSP& aWeakPtr) noexcept;

    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
    void                    Set                 (TSP&& aWeakPtr) noexcept;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
    void                    Set                 (const TSP& aSharedPtr) noexcept;

    bool                    operator!=          (const this_type& aWeakPtr) const noexcept;
    bool                    operator==          (const this_type& aWeakPtr) const noexcept;
    bool                    operator>           (const this_type& aWeakPtr) const noexcept;
    bool                    operator<           (const this_type& aWeakPtr) const noexcept;

    this_type&              operator=           (const this_type& aWeakPtr) noexcept;
    this_type&              operator=           (this_type&& aWeakPtr) noexcept;

    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
    this_type&              operator=           (const TSP& aWeakPtr) noexcept;

    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
    this_type&              operator=           (TSP&& aWeakPtr) noexcept;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
    this_type&              operator=           (const TSP& aSharedPtr) noexcept;

    const ControlBlockT*    ControlBlock        (void) const noexcept;
    ControlBlockT*          ControlBlock        (void) noexcept;
    ControlBlockT*          MoveControlBlock    (void) noexcept;

    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableToBase<GpWeakPtr<T>, TSP>
    TSP                     CastToBase          (void) const;

    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableToDerived<GpWeakPtr<T>, TSP>
    TSP                     CastToDerived       (void) const;

    template<Concepts::Ptr::IsWeakPtr TSP>
        requires Concepts::Ptr::CastableTo<GpWeakPtr<T>, TSP>
    TSP                     CastTo              (void) const;

    GpSharedPtr<T>          Lock                (void) const noexcept;

private:
    ControlBlockT* iControlBlock = nullptr;
};

template <typename T>
GpWeakPtr<T>::GpWeakPtr
(
    ControlBlockT* aControlBlock,
    GpMethodAccessGuard<_GpWeakPtrAccess>
) noexcept:
iControlBlock{aControlBlock}
{
}

template <typename T>
GpWeakPtr<T>::GpWeakPtr (void) noexcept
{
}

template <typename T>
GpWeakPtr<T>::GpWeakPtr (const this_type& aWeakPtr) noexcept:
iControlBlock{const_cast<ControlBlockT*>(aWeakPtr.ControlBlock())}
{
    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireWeak();
    }
}

template <typename T>
GpWeakPtr<T>::GpWeakPtr (this_type&& aWeakPtr) noexcept:
iControlBlock{aWeakPtr.MoveControlBlock()}
{
}

template <typename T>
GpWeakPtr<T>::GpWeakPtr (std::nullptr_t) noexcept:
iControlBlock{nullptr}
{
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
GpWeakPtr<T>::GpWeakPtr (const TSP& aWeakPtr) noexcept
{
    iControlBlock = const_cast<ControlBlockT*>(aWeakPtr.ControlBlock());

    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireWeak();
    }
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
GpWeakPtr<T>::GpWeakPtr (const TSP& aSharedPtr) noexcept
{
    iControlBlock = const_cast<ControlBlockT*>(aSharedPtr.ControlBlock());

    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireWeak();
    }
}

template <typename T>
GpWeakPtr<T>::~GpWeakPtr (void) noexcept
{
    Clear();
}

template <typename T>
#if defined(TSAN_ENABLED)
__attribute__((no_sanitize("thread"))) // NOTE: TSAN reported a false positive data race
#endif// #if defined(TSAN_ENABLED)
void    GpWeakPtr<T>::Clear (void) noexcept
{
    if (iControlBlock == nullptr) [[unlikely]]
    {
        return;
    }

    iControlBlock->ReleaseWeak();
    iControlBlock = nullptr;
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
void    GpWeakPtr<T>::Set (const TSP& aWeakPtr) noexcept
{
    if (static_cast<const void*>(this) == static_cast<const void*>(&aWeakPtr)) [[unlikely]]
    {
        return;
    }

    Clear();
    iControlBlock = const_cast<ControlBlockT*>(aWeakPtr.ControlBlock());

    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireWeak();
    }
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
void    GpWeakPtr<T>::Set (TSP&& aWeakPtr) noexcept
{
    if (static_cast<const void*>(this) == static_cast<const void*>(&aWeakPtr)) [[unlikely]]
    {
        return;
    }

    Clear();
    iControlBlock = aWeakPtr.MoveControlBlock();
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
void    GpWeakPtr<T>::Set (const TSP& aSharedPtr) noexcept
{
    Clear();
    iControlBlock = const_cast<ControlBlockT*>(aSharedPtr.ControlBlock());

    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireWeak();
    }
}

template <typename T>
bool    GpWeakPtr<T>::operator!= (const this_type& aWeakPtr) const noexcept
{
    return iControlBlock != aWeakPtr.ControlBlock();
}

template <typename T>
bool    GpWeakPtr<T>::operator== (const this_type& aWeakPtr) const noexcept
{
    return iControlBlock == aWeakPtr.ControlBlock();
}

template <typename T>
bool    GpWeakPtr<T>::operator> (const this_type& aWeakPtr) const noexcept
{
    return iControlBlock > aWeakPtr.ControlBlock();
}

template <typename T>
bool    GpWeakPtr<T>::operator< (const this_type& aWeakPtr) const noexcept
{
    return iControlBlock < aWeakPtr.ControlBlock();
}

template <typename T>
typename GpWeakPtr<T>::this_type&   GpWeakPtr<T>::operator= (const this_type& aWeakPtr) noexcept
{
    Set(aWeakPtr);
    return *this;
}

template <typename T>
typename GpWeakPtr<T>::this_type&   GpWeakPtr<T>::operator= (this_type&& aWeakPtr) noexcept
{
    Set(std::move(aWeakPtr));
    return *this;
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
typename GpWeakPtr<T>::this_type&   GpWeakPtr<T>::operator= (const TSP& aWeakPtr) noexcept
{
    Set(aWeakPtr);
    return *this;
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
typename GpWeakPtr<T>::this_type&   GpWeakPtr<T>::operator= (TSP&& aWeakPtr) noexcept
{
    Set(std::move(aWeakPtr));
    return *this;
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpWeakPtr<T>>
GpWeakPtr<T>::this_type&    GpWeakPtr<T>::operator= (const TSP& aSharedPtr) noexcept
{
    Set(aSharedPtr);
    return *this;
}

template <typename T>
const GpWeakPtr<T>::ControlBlockT*  GpWeakPtr<T>::ControlBlock (void) const noexcept
{
    return iControlBlock;
}

template <typename T>
GpWeakPtr<T>::ControlBlockT*    GpWeakPtr<T>::ControlBlock (void) noexcept
{
    return iControlBlock;
}

template <typename T>
GpWeakPtr<T>::ControlBlockT*    GpWeakPtr<T>::MoveControlBlock (void) noexcept
{
    ControlBlockT* tmp = iControlBlock;
    iControlBlock = nullptr;
    return tmp;
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableToBase<GpWeakPtr<T>, TSP>
TSP GpWeakPtr<T>::CastToBase (void) const
{
    return TSP
    {
        const_cast<ControlBlockT*>(this->ControlBlock()),
        GpMethodAccess{GpMethodAccessTypeShell<_GpWeakPtrAccess>{}}
    };
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableToDerived<GpWeakPtr<T>, TSP>
TSP GpWeakPtr<T>::CastToDerived (void) const
{
    return TSP
    {
        const_cast<ControlBlockT*>(this->ControlBlock()),
        GpMethodAccess{GpMethodAccessTypeShell<_GpWeakPtrAccess>{}}
    };
}

template <typename T>
template<Concepts::Ptr::IsWeakPtr TSP>
    requires Concepts::Ptr::CastableTo<GpWeakPtr<T>, TSP>
TSP GpWeakPtr<T>::CastTo (void) const
{
    return TSP
    {
        const_cast<ControlBlockT*>(this->ControlBlock()),
        GpMethodAccess{GpMethodAccessTypeShell<_GpWeakPtrAccess>{}}
    };
}

template <typename T>
GpSharedPtr<T>  GpWeakPtr<T>::Lock (void) const noexcept
{
    if (iControlBlock == nullptr)
    {
        return nullptr;
    }

    if (iControlBlock->TryAcquireStrongIfNotZero() == false)
    {
        return nullptr;
    }

    // Strong ref acquired
    return GpSharedPtr<T>
    (
        const_cast<ControlBlockT*>(this->ControlBlock()),
        GpMethodAccess{GpMethodAccessTypeShell<_GpSharedPtrAccess>{}}
    );
}

// ------------------------------------------------

template<typename T>
using GpWP  = GpWeakPtr<T>;

template<typename T>
using GpCWP = GpWeakPtr<const T>;

}// namespace GPlatform
