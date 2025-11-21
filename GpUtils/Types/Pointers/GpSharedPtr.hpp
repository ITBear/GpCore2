#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Types/Pointers/GpSharedPtrStorage.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>

namespace GPlatform {

TAG_REGISTER(GpSharedPtr)

namespace Concepts::Ptr {

template <typename T>
concept IsSharedPtr = requires()
{
    requires GpHasTag_GpSharedPtr<T>();
};

template<typename BASE, typename DERIVED>
using IsBaseOf = std::is_base_of<std::remove_cvref_t<typename BASE::value_type>, std::remove_cvref_t<typename DERIVED::value_type>>;

//      Derived ->       Base (OK)
//const Derived -> const Base (OK)
//      Derived -> const Base (OK)
//const Derived ->       Base (Error)
template<typename FROM, typename TO>
concept CastableToBase = requires()
{
    requires
       IsBaseOf<TO, FROM>::value
    && !(FROM::SIsConst() && !TO::SIsConst());
};

//      Base ->       Derived (OK)
//const Base -> const Derived (OK)
//      Base -> const Derived (OK)
//const Base ->       Derived (Error)
template<typename FROM, typename TO>
concept CastableToDerived = requires()
{
   requires
      IsBaseOf<FROM, TO>::value
   && !(FROM::SIsConst() && !TO::SIsConst());
};

template<typename FROM, typename TO>
concept CastableTo = requires()
{
    requires
       (IsBaseOf<FROM, TO>::value || IsBaseOf<TO, FROM>::value)
    && !(FROM::SIsConst() && !TO::SIsConst());
};

}// namespace Concepts::Ptr

class _GpSharedPtrAccess;

template <typename T>
class GpSharedPtr
{
public:
    using this_type         = GpSharedPtr<T>;
    using value_type        = T;
    using const_value_type  = const std::remove_const_t<T>;
    using ControlBlockT     = GpSharedPtrControlBlock;

    TAG_SET(GpSharedPtr)

    static constexpr bool SIsConst  (void) noexcept {return std::is_const_v<value_type>;}

public:
                            GpSharedPtr         (void) noexcept;
                            GpSharedPtr         (const this_type& aSharedPtr) noexcept;
                            GpSharedPtr         (this_type&& aSharedPtr) noexcept;
                            GpSharedPtr         (ControlBlockT* aControlBlock,
                                                 GpMethodAccessGuard<_GpSharedPtrAccess>) noexcept;
                            GpSharedPtr         (std::nullptr_t) noexcept;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, this_type>
                            GpSharedPtr         (const TSP& aSharedPtr) noexcept;
    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, this_type>
                            GpSharedPtr         (TSP&& aSharedPtr) noexcept;

                            ~GpSharedPtr        (void) noexcept;

    void                    Clear               (void) noexcept;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, this_type>
    void                    Set                 (const TSP& aSharedPtr) noexcept;
    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, this_type>
    void                    Set                 (TSP&& aSharedPtr) noexcept;

    bool                    IsNULL              (void) const noexcept;
    bool                    IsNotNULL           (void) const noexcept;

    bool                    operator!=          (const this_type& aSharedPtr) const noexcept;
    bool                    operator==          (const this_type& aSharedPtr) const noexcept;
    bool                    operator>           (const this_type& aSharedPtr) const noexcept;
    bool                    operator<           (const this_type& aSharedPtr) const noexcept;

    this_type&              operator=           (const this_type& aSharedPtr) noexcept;
    this_type&              operator=           (this_type&& aSharedPtr) noexcept;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, this_type>
    this_type&              operator=           (const TSP& aSharedPtr) noexcept;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<TSP, this_type>
    this_type&              operator=           (TSP&& aSharedPtr) noexcept;

    const ControlBlockT*    ControlBlock        (void) const noexcept;
    ControlBlockT*          ControlBlock        (void) noexcept;
    ControlBlockT*          MoveControlBlock    (void) noexcept;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToBase<this_type, TSP>
    TSP                     CastToBase          (void) const;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableToDerived<this_type, TSP>
    TSP                     CastToDerived       (void) const;

    template<Concepts::Ptr::IsSharedPtr TSP>
        requires Concepts::Ptr::CastableTo<this_type, TSP>
    TSP                     CastTo              (void) const;

    value_type&             V                   (void);
    const_value_type&       V                   (void) const;
    value_type&             Vn                  (void) noexcept;
    const_value_type&       Vn                  (void) const noexcept;
    value_type*             P                   (void);
    const_value_type*       P                   (void) const;
    value_type*             Pn                  (void) noexcept;
    const_value_type*       Pn                  (void) const noexcept;

    value_type*             operator->          (void);
    const_value_type*       operator->          (void) const;

    template<typename... Ts>
    [[nodiscard]] static
    this_type               SNew (Ts&&... aArgs)
    {
        ControlBlockT* controlBlock = MemOps::SNew<GpSharedPtrStorage<std::remove_const_t<T>>>(std::forward<Ts>(aArgs)...);

        return this_type
        {
            controlBlock,
            GpMethodAccess{GpMethodAccessTypeShell<_GpSharedPtrAccess>{}}
        };
    }

private:
    ControlBlockT* iControlBlock = nullptr;
};

template <typename T>
GpSharedPtr<T>::GpSharedPtr (void) noexcept
{
}

template <typename T>
GpSharedPtr<T>::GpSharedPtr (const this_type& aSharedPtr) noexcept:
iControlBlock{const_cast<ControlBlockT*>(aSharedPtr.ControlBlock())}
{
    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireStrong();
    }
}

template <typename T>
GpSharedPtr<T>::GpSharedPtr (this_type&& aSharedPtr) noexcept:
iControlBlock{aSharedPtr.MoveControlBlock()}
{
}

template <typename T>
GpSharedPtr<T>::GpSharedPtr
(
    ControlBlockT* aControlBlock,
    GpMethodAccessGuard<_GpSharedPtrAccess>
) noexcept:
iControlBlock{aControlBlock}
{
}

template <typename T>
GpSharedPtr<T>::GpSharedPtr (std::nullptr_t) noexcept:
iControlBlock{nullptr}
{
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpSharedPtr<T>>
GpSharedPtr<T>::GpSharedPtr (const TSP& aSharedPtr) noexcept:
iControlBlock{const_cast<ControlBlockT*>(aSharedPtr.ControlBlock())}
{
    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireStrong();
    }
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpSharedPtr<T>>
GpSharedPtr<T>::GpSharedPtr (TSP&& aSharedPtr) noexcept:
iControlBlock{aSharedPtr.MoveControlBlock()}
{
}

template <typename T>
GpSharedPtr<T>::~GpSharedPtr (void) noexcept
{
    Clear();
}

template <typename T>
#if defined(TSAN_ENABLED)
__attribute__((no_sanitize("thread"))) // NOTE: TSAN reported a false positive data race
#endif// #if defined(TSAN_ENABLED)
void    GpSharedPtr<T>::Clear (void) noexcept
{
    if (iControlBlock == nullptr) [[unlikely]]
    {
        return;
    }

    iControlBlock->ReleaseStrong<T>();
    iControlBlock = nullptr;
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpSharedPtr<T>>
void    GpSharedPtr<T>::Set (const TSP& aSharedPtr) noexcept
{
    if (static_cast<const void*>(this) == static_cast<const void*>(&aSharedPtr)) [[unlikely]]
    {
        return;
    }

    Clear();
    iControlBlock = const_cast<ControlBlockT*>(aSharedPtr.ControlBlock());

    if (iControlBlock) [[likely]]
    {
        iControlBlock->AcquireStrong();
    }
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpSharedPtr<T>>
void    GpSharedPtr<T>::Set (TSP&& aSharedPtr) noexcept
{
    if (static_cast<const void*>(this) == static_cast<const void*>(&aSharedPtr)) [[unlikely]]
    {
        return;
    }

    Clear();
    iControlBlock = aSharedPtr.MoveControlBlock();
}

template <typename T>
bool    GpSharedPtr<T>::IsNULL (void) const noexcept
{
    return (iControlBlock == nullptr)
        || (iControlBlock->Pointer() == nullptr);
}

template <typename T>
bool    GpSharedPtr<T>::IsNotNULL (void) const noexcept
{
    return !IsNULL();
}

template <typename T>
bool    GpSharedPtr<T>::operator!= (const this_type& aSharedPtr) const noexcept
{
    return iControlBlock != aSharedPtr.ControlBlock();
}

template <typename T>
bool    GpSharedPtr<T>::operator== (const this_type& aSharedPtr) const noexcept
{
    return iControlBlock == aSharedPtr.ControlBlock();
}

template <typename T>
bool    GpSharedPtr<T>::operator> (const this_type& aSharedPtr) const noexcept
{
    return iControlBlock > aSharedPtr.ControlBlock();
}

template <typename T>
bool    GpSharedPtr<T>::operator< (const this_type& aSharedPtr) const noexcept
{
    return iControlBlock < aSharedPtr.ControlBlock();
}

template <typename T>
typename GpSharedPtr<T>::this_type& GpSharedPtr<T>::operator= (const this_type& aSharedPtr) noexcept
{
    Set(aSharedPtr);
    return *this;
}

template <typename T>
typename GpSharedPtr<T>::this_type& GpSharedPtr<T>::operator= (this_type&& aSharedPtr) noexcept
{
    Set(std::move(aSharedPtr));
    return *this;
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpSharedPtr<T>>
typename GpSharedPtr<T>::this_type& GpSharedPtr<T>::operator= (const TSP& aSharedPtr) noexcept
{
    Set(aSharedPtr);
    return *this;
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<TSP, GpSharedPtr<T>>
typename GpSharedPtr<T>::this_type& GpSharedPtr<T>::operator= (TSP&& aSharedPtr) noexcept
{
    Set(std::move(aSharedPtr));
    return *this;
}

template <typename T>
const GpSharedPtr<T>::ControlBlockT*    GpSharedPtr<T>::ControlBlock (void) const noexcept
{
    return iControlBlock;
}

template <typename T>
GpSharedPtr<T>::ControlBlockT*  GpSharedPtr<T>::ControlBlock (void) noexcept
{
    return iControlBlock;
}

template <typename T>
GpSharedPtr<T>::ControlBlockT*  GpSharedPtr<T>::MoveControlBlock (void) noexcept
{
    ControlBlockT* tmp = iControlBlock;
    iControlBlock = nullptr;
    return tmp;
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToBase<GpSharedPtr<T>, TSP>
TSP GpSharedPtr<T>::CastToBase (void) const
{
    return CastTo<TSP>();
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableToDerived<GpSharedPtr<T>, TSP>
TSP GpSharedPtr<T>::CastToDerived (void) const
{
    return CastTo<TSP>();
}

template <typename T>
template<Concepts::Ptr::IsSharedPtr TSP>
    requires Concepts::Ptr::CastableTo<GpSharedPtr<T>, TSP>
TSP GpSharedPtr<T>::CastTo (void) const
{
    ControlBlockT* controlBlock = const_cast<ControlBlockT*>(this->ControlBlock());

    if (controlBlock != nullptr)
    {
        controlBlock->AcquireStrong();
    }

    return TSP
    {
        controlBlock,
        GpMethodAccess{GpMethodAccessTypeShell<_GpSharedPtrAccess>{}}
    };
}

template <typename T>
GpSharedPtr<T>::value_type& GpSharedPtr<T>::V (void)
{
    return *P();
}

template <typename T>
GpSharedPtr<T>::const_value_type&   GpSharedPtr<T>::V (void) const
{
    return *P();
}

template <typename T>
GpSharedPtr<T>::value_type& GpSharedPtr<T>::Vn (void) noexcept
{
    return *Pn();
}

template <typename T>
GpSharedPtr<T>::const_value_type&   GpSharedPtr<T>::Vn (void) const noexcept
{
    return *Pn();
}

template <typename T>
GpSharedPtr<T>::value_type* GpSharedPtr<T>::P (void)
{
    VERIFY
    (
        iControlBlock != nullptr,
        "Shared pointer control block is null"_sv
    );

    value_type* ptr = Pn();

    VERIFY
    (
        ptr != nullptr,
        "Shared pointer value is null"_sv
    );

    return ptr;
}

template <typename T>
GpSharedPtr<T>::const_value_type*   GpSharedPtr<T>::P (void) const
{
    return const_cast<this_type&>(*this).P();
}

template <typename T>
GpSharedPtr<T>::value_type* GpSharedPtr<T>::Pn (void) noexcept
{
    return static_cast<value_type*>(iControlBlock->Pointer());
}

template <typename T>
GpSharedPtr<T>::const_value_type*   GpSharedPtr<T>::Pn (void) const noexcept
{
    return static_cast<const_value_type*>(iControlBlock->Pointer());
}

template <typename T>
GpSharedPtr<T>::value_type* GpSharedPtr<T>::operator-> (void)
{
    return P();
}

template <typename T>
GpSharedPtr<T>::const_value_type*   GpSharedPtr<T>::operator-> (void) const
{
    return P();
}

// ------------------------------------------------

template<typename T>
using GpSP  = GpSharedPtr<T>;

template<typename T>
using GpCSP = GpSharedPtr<const T>;

template<typename T, typename... Ts>
[[nodiscard]] GpSP<T>   MakeSP (Ts&&... aArgs)
{
    return GpSP<T>::SNew(std::forward<Ts>(aArgs)...);
}

template<typename T, typename... Ts>
[[nodiscard]] GpCSP<T>  MakeCSP (Ts&&... aArgs)
{
    return GpCSP<T>::SNew(std::forward<Ts>(aArgs)...);
}

}// namespace GPlatform

//********************** Hash *********************
namespace std {

template <typename T>
struct hash<typename ::GPlatform::GpSharedPtr<T>>
{
    size_t operator()(const ::GPlatform::GpSharedPtr<T>& aSP) const noexcept
    {
        return std::hash<const typename ::GPlatform::GpSharedPtr<T>::ControlBlockT*>()(aSP.ControlBlock());
    }
};

}// namespace std
