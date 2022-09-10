#pragma once

#include "../../GpUtils_global.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include "GpReferenceStorage.hpp"
#include "../../Exceptions/GpException.hpp"
#include "../Strings/GpStringLiterals.hpp"

namespace GPlatform {

template <typename  T,
          bool      _IsWeak>
class GpSharedPtrBase
{
public:
    using this_type         = GpSharedPtrBase<T, _IsWeak>;
    using value_type        = T;
    using const_value_type  = const std::remove_const_t<T>;

    CLASS_TAG(GpSharedPtrBase)
    CLASS_TAG_DETECTOR(GpSharedPtrBase)

    static constexpr bool SIsWeak   (void) noexcept {return _IsWeak;}
    static constexpr bool SIsConst  (void) noexcept {return std::is_const_v<value_type>;}

    template<typename BASE, typename DERIVED>
    using IsBaseOf = std::is_base_of<typename BASE::value_type, typename DERIVED::value_type>;

    //      Derived ->       Base (OK)
    //const Derived -> const Base (OK)
    //      Derived -> const Base (OK)
    //const Derived ->       Base (Error)
    template<typename FROM, typename TO>
    using IsCastableDown = typename std::enable_if<   IsBaseOf<TO, FROM>::value
                                                   && !(FROM::SIsConst() && !TO::SIsConst())
                                                  >::type;

    //      Base ->       Derived (OK)
    //const Base -> const Derived (OK)
    //      Base -> const Derived (OK)
    //const Base ->       Derived (Error)
    template<typename FROM, typename TO>
    using IsCastableUp  = typename std::enable_if<   IsBaseOf<FROM, TO>::value
                                                  && !(FROM::SIsConst() && !TO::SIsConst())
                                                 >::type;

    template<typename FROM, typename TO>
    using IsCastable    = typename std::enable_if<   (IsBaseOf<FROM, TO>::value || IsBaseOf<TO, FROM>::value)
                                                  && !(FROM::SIsConst() && !TO::SIsConst())
                                                 >::type;

public:
    [[nodiscard]] static
    this_type           SNull (void) noexcept
    {
        return this_type(nullptr);
    }

    template<typename... Ts>
    [[nodiscard]] static
    this_type           SNew (Ts&&... aArgs)
    {
        GpReferenceCounter* refCounter = MemOps::SNew<GpReferenceStorage<value_type>>(std::forward<Ts>(aArgs)...);
        this_type           res(refCounter);

        return res;
    }

private:
                                    GpSharedPtrBase (GpReferenceCounter* aRefCounter) noexcept;

public:
    template<typename TSP, typename = IsCastableDown<TSP, this_type>>
                                    GpSharedPtrBase (const TSP& aSharedPtr) noexcept:
    iRefCounter(aSharedPtr._RefCounter())
    {
        if (iRefCounter)
        {
            iRefCounter->Acquire<_IsWeak>();
        }
    }

                                    GpSharedPtrBase (void) noexcept;
                                    GpSharedPtrBase (const this_type& aSharedPtr) noexcept;
                                    GpSharedPtrBase (this_type&& aSharedPtr) noexcept;
                                    ~GpSharedPtrBase(void) noexcept;

public:
    void                            Clear           (void) noexcept;
    void                            Set             (const this_type& aSharedPtr) noexcept;
    void                            Set             (this_type&& aSharedPtr) noexcept;

    template<typename TSP, typename = IsCastableDown<TSP, this_type>>
    void                            Set             (const TSP& aSharedPtr) noexcept
    {
        if (iRefCounter != aSharedPtr._RefCounter())
        {
            Clear();
            iRefCounter = aSharedPtr._RefCounter();

            if (iRefCounter)
            {
                iRefCounter->Acquire<_IsWeak>();
            }
        }
    }

    [[nodiscard]] bool              IsNULL          (void) const noexcept;
    [[nodiscard]] bool              IsNotNULL       (void) const noexcept;

    [[nodiscard]] value_type&       V               (void) {return *P();}
    [[nodiscard]] const_value_type& V               (void) const {return *P();}
    [[nodiscard]] value_type&       Vn              (void) noexcept {return *Pn();}
    [[nodiscard]] const_value_type& Vn              (void) const noexcept {return *Pn();}
    [[nodiscard]] value_type*       P               (void)
    {
        THROW_COND_GP
        (
            iRefCounter != nullptr,
            "Shared pointer is empty"_sv
        );

        T* ptr = Pn();

        THROW_COND_GP
        (
            ptr != nullptr,
            "Shared pointer value is null"_sv
        );

        return ptr;
    }
    [[nodiscard]] const_value_type* P               (void) const {return const_cast<this_type&>(*this).P();}

    [[nodiscard]] value_type*       Pn              (void) noexcept
    {
        return iRefCounter->template ValuePtr<T>();
    }

    [[nodiscard]] const_value_type* Pn              (void) const noexcept {return const_cast<this_type&>(*this).Pn();}

    //----------------- operators -------------------------
    [[nodiscard]] bool              operator!=      (const this_type& aSharedPtr) const noexcept;
    [[nodiscard]] bool              operator==      (const this_type& aSharedPtr) const noexcept;
    [[nodiscard]] bool              operator>       (const this_type& aSharedPtr) const noexcept;
    [[nodiscard]] bool              operator<       (const this_type& aSharedPtr) const noexcept;

    this_type&                      operator=       (const this_type& aSharedPtr) noexcept;
    this_type&                      operator=       (this_type&& aSharedPtr) noexcept;

    template<typename TSP, typename = IsCastableDown<this_type, TSP>>
    this_type&                      operator=       (const TSP& aSharedPtr) noexcept
    {
        Set(aSharedPtr);
        return *this;
    }

    /*template<typename TSP, typename = IsMovable<TSP, this_type>>
    this_type&                      operator=       (TSP&& aSharedPtr) noexcept
    {
        Set(std::move(aSharedPtr));
        return *this;
    }*/

    template<typename TSP, typename = IsCastableDown<this_type, TSP>>
    TSP                             CastDownAs      (void) noexcept
    {
        return TSP::_SConstructFromRefCounter(iRefCounter);
    }

    template<typename TSP, typename = IsCastableUp<this_type, TSP>>
    TSP                             CastUpAs        (void) noexcept
    {
        return TSP::_SConstructFromRefCounter(iRefCounter);
    }

    template<typename TSP, typename = IsCastable<this_type, TSP>>
    TSP                             CastAs      (void) noexcept
    {
        return TSP::_SConstructFromRefCounter(iRefCounter);
    }

    GpReferenceCounter* _RefCounter                 (void) const noexcept {return iRefCounter;}
    GpReferenceCounter* _MoveRefCounter             (void) noexcept {GpReferenceCounter* r = iRefCounter; iRefCounter = nullptr; return r;}

    static this_type    _SConstructFromRefCounter   (GpReferenceCounter* aRefCounter)
    {
        if (aRefCounter)
        {
            aRefCounter->template Acquire<_IsWeak>();
        }

        return this_type(aRefCounter);
    }

private:
    GpReferenceCounter* iRefCounter = nullptr;
};

template <typename T, bool _IsWeak>
GpSharedPtrBase<T, _IsWeak>::GpSharedPtrBase (GpReferenceCounter* aRefCounter) noexcept:
iRefCounter(aRefCounter)
{
}

template <typename T, bool _IsWeak>
GpSharedPtrBase<T, _IsWeak>::GpSharedPtrBase (void) noexcept
{
}

template <typename T, bool _IsWeak>
GpSharedPtrBase<T, _IsWeak>::GpSharedPtrBase (const this_type& aSharedPtr) noexcept:
iRefCounter(aSharedPtr._RefCounter())
{
    if (iRefCounter)
    {
        iRefCounter->template Acquire<_IsWeak>();
    }
}

template <typename T, bool _IsWeak>
GpSharedPtrBase<T, _IsWeak>::GpSharedPtrBase (this_type&& aSharedPtr) noexcept:
iRefCounter(aSharedPtr._MoveRefCounter())
{
}

template <typename T, bool _IsWeak>
GpSharedPtrBase<T, _IsWeak>::~GpSharedPtrBase (void) noexcept
{
    Clear();
}

template <typename T, bool _IsWeak>
void    GpSharedPtrBase<T, _IsWeak>::Clear (void) noexcept
{
    if (iRefCounter)
    {
        const size_t refCount = iRefCounter->template Release<_IsWeak>();

        if (refCount == 0)
        {
            MemOps::SDelete(iRefCounter);
        }

        iRefCounter = nullptr;
    }
}

template <typename T, bool _IsWeak>
void    GpSharedPtrBase<T, _IsWeak>::Set (const this_type& aSharedPtr) noexcept
{
    if (iRefCounter != aSharedPtr.iRefCounter)
    {
        Clear();
        iRefCounter = aSharedPtr.iRefCounter;

        if (iRefCounter)
        {
            iRefCounter->template Acquire<_IsWeak>();
        }
    }
}

template <typename T, bool _IsWeak>
void    GpSharedPtrBase<T, _IsWeak>::Set (this_type&& aSharedPtr) noexcept
{
    if (iRefCounter != aSharedPtr.iRefCounter)
    {
        Clear();

        iRefCounter = aSharedPtr._MoveRefCounter();
    } else
    {
        aSharedPtr.Clear();
    }
}

template <typename T, bool _IsWeak>
bool    GpSharedPtrBase<T, _IsWeak>::IsNULL (void) const noexcept
{
    return iRefCounter == nullptr;
}

template <typename T, bool _IsWeak>
bool    GpSharedPtrBase<T, _IsWeak>::IsNotNULL (void) const noexcept
{
    return iRefCounter != nullptr;
}

template <typename T, bool _IsWeak>
bool    GpSharedPtrBase<T, _IsWeak>::operator!= (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter != aSharedPtr.iRefCounter;
}

template <typename T, bool _IsWeak>
bool    GpSharedPtrBase<T, _IsWeak>::operator== (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter == aSharedPtr.iRefCounter;
}

template <typename T, bool _IsWeak>
bool    GpSharedPtrBase<T, _IsWeak>::operator> (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter > aSharedPtr.iRefCounter;
}

template <typename T, bool _IsWeak>
bool    GpSharedPtrBase<T, _IsWeak>::operator< (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter < aSharedPtr.iRefCounter;
}

template <typename T, bool _IsWeak>
typename GpSharedPtrBase<T, _IsWeak>::this_type&    GpSharedPtrBase<T, _IsWeak>::operator= (const this_type& aSharedPtr) noexcept
{
    Set(aSharedPtr);
    return *this;
}

template <typename T, bool _IsWeak>
typename GpSharedPtrBase<T, _IsWeak>::this_type&    GpSharedPtrBase<T, _IsWeak>::operator= (this_type&& aSharedPtr) noexcept
{
    Set(std::move(aSharedPtr));
    return *this;
}

template<typename T>
using GpSP  = GpSharedPtrBase<T, false>;

template<typename T>
using GpCSP = GpSharedPtrBase<const T, false>;

template<typename T>
using GpWP  = GpSharedPtrBase<T, true>;

template<typename T>
using GpCWP = GpSharedPtrBase<const T, true>;

template<typename T, typename... Ts>
[[nodiscard]] typename T::SP    MakeSP (Ts&&... aArgs)
{
    return T::SP::SNew(std::forward<Ts>(aArgs)...);
}

template<typename T, typename... Ts>
[[nodiscard]] typename T::CSP   MakeCSP (Ts&&... aArgs)
{
    return T::CSP::SNew(std::forward<Ts>(aArgs)...);
}

}//namespace GPlatform

//********************** Hash *********************
namespace std {

template <typename T, bool _IsWeak>
struct hash<GPlatform::GpSharedPtrBase<T, _IsWeak>>
{
    size_t operator()(const GPlatform::GpSharedPtrBase<T, _IsWeak>& aSP) const noexcept
    {
        return std::hash<const GPlatform::GpReferenceCounter*>()(aSP._RefCounter());
    }
};

}//std

#endif//#if defined(GP_USE_SHARED_POINTERS)
