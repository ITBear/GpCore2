#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include "GpReferenceStorage.hpp"
#include "../../Exceptions/GpException.hpp"
#include "../../Memory/GpMemOps.hpp"
#include "../Strings/GpStringLiterals.hpp"

namespace GPlatform {

template <typename	T,
          typename	RefPtrT,
          bool		_IsWeak>
class GpSharedPtrBase
{
public:
    using this_type		= GpSharedPtrBase<T, RefPtrT, _IsWeak>;
    using value_type	= T;

    static constexpr bool SIsWeak	(void) noexcept {return _IsWeak;}
    static constexpr bool SIsConst	(void) noexcept {return std::is_const_v<RefPtrT>;}

    template<typename BASE, typename DERIVED>
    using IsBaseOf = std::is_base_of<typename BASE::value_type, typename DERIVED::value_type>;

    //      Derived ->       Base OK
    //const Derived -> const Base OK
    //      Derived -> const Base OK
    //const Derived ->       Base Error
    template<typename FROM, typename TO>
    using IsConvertible	= typename std::enable_if<IsBaseOf<TO, FROM>::value &&
                                                  !(FROM::SIsConst() && !TO::SIsConst())
                                                 >::type;

public:
    [[nodiscard]] static
    this_type			SNull (void) noexcept
    {
        return this_type(nullptr);
    }

    template<typename... Ts>
    [[nodiscard]] static
    this_type			SNew (Ts&&... aArgs)
    {
        RefPtrT refCounter = MemOps::SNew<GpReferenceStorage<value_type,
                                                             MemOps::NewDeleter>>(std::forward<Ts>(aArgs)...);
        this_type res(refCounter);

        return res;
    }

    template<typename... Ts>
    [[nodiscard]] static
    this_type			SEmplace (void* aPtrToPlace, Ts&&... aArgs)
    {
        RefPtrT refCounter = GpMemOps::SEmplace<GpReferenceStorage<value_type,
                                                GpMemOps::EmplaceDeleter>>(aPtrToPlace, std::forward<Ts>(aArgs)...);
        return this_type(refCounter);
    }

private:
                                    GpSharedPtrBase	(RefPtrT aRefCounter) noexcept;

public:
    template<typename TSP, typename = IsConvertible<TSP, this_type>>
                                    GpSharedPtrBase (const TSP& aSharedPtr) noexcept:
    iRefCounter(aSharedPtr._RefCounter())
    {
        if (iRefCounter)
        {
            iRefCounter->template Accuire<_IsWeak>();
        }
    }

    template<typename TSP, typename = IsConvertible<TSP, this_type>>
                                    GpSharedPtrBase (TSP&& aSharedPtr) noexcept:
    iRefCounter(aSharedPtr._MoveRefCounter())
    {
        static_assert (!(TSP::SIsWeak() && !SIsWeak()) , "Can`t move from Weak to !Weak");
    }

                                    GpSharedPtrBase	(void) noexcept;
                                    GpSharedPtrBase (const this_type& aSharedPtr) noexcept;
                                    GpSharedPtrBase (this_type&& aSharedPtr) noexcept;
                                    ~GpSharedPtrBase(void) noexcept;

public:
    /*template<typename TSP, typename = IsConvertible<this_type, TSP>>
    static this_type				SReinterpretConstruct (TSP& aSharedPtr)
    {
        RefPtrT refCounter = aSharedPtr._RefCounter();

        if (refCounter)
        {
            refCounter->template Accuire<_IsWeak>();
        }

        return this_type(refCounter);
    }*/

    void							Clear			(void) noexcept;
    //[[nodiscard]] this_type			Clone			(void) const;	//Create copy of the T object (copy constructor) and place to new SP

    void							Set				(const this_type& aSharedPtr) noexcept;
    void							Set				(this_type&& aSharedPtr) noexcept;

    template<typename TSP, typename = IsConvertible<TSP, this_type>>
    void							Set				(const TSP& aSharedPtr) noexcept
    {
        if (iRefCounter != aSharedPtr._RefCounter())
        {
            Clear();
            iRefCounter = aSharedPtr._RefCounter();

            if (iRefCounter)
            {
                iRefCounter->template Accuire<_IsWeak>();
            }
        }
    }

    template<typename TSP, typename = IsConvertible<TSP, this_type>>
    void							Set				(TSP&& aSharedPtr) noexcept
    {
        static_assert (!(TSP::SIsWeak() && !SIsWeak()) , "Can`t move from Weak to !Weak");

        if (iRefCounter != aSharedPtr._RefCounter())
        {
            Clear();

            iRefCounter = aSharedPtr._MoveRefCounter();
        } else
        {
            aSharedPtr.Clear();
        }
    }

    [[nodiscard]] count_t			Counter			(void) const noexcept;
    [[nodiscard]] bool				IsNULL			(void) const noexcept;
    [[nodiscard]] bool				IsNotNULL		(void) const noexcept;

    [[nodiscard]] value_type&		V				(void);
    [[nodiscard]] value_type&		Vn				(void) noexcept;
    [[nodiscard]] value_type*		P				(void);
    [[nodiscard]] value_type*		Pn				(void) noexcept;

    [[nodiscard]] const value_type&	VC				(void) const;
    [[nodiscard]] const value_type&	VCn				(void) const noexcept;
    [[nodiscard]] const value_type*	PC				(void) const;
    [[nodiscard]] const value_type*	PCn				(void) const noexcept;

    //----------------- operators -------------------------
    [[nodiscard]] bool				operator!=		(const this_type& aSharedPtr) const noexcept;
    [[nodiscard]] bool				operator==		(const this_type& aSharedPtr) const noexcept;
    [[nodiscard]] bool				operator>		(const this_type& aSharedPtr) const noexcept;
    [[nodiscard]] bool				operator<		(const this_type& aSharedPtr) const noexcept;

    this_type&						operator=		(const this_type& aSharedPtr) noexcept;
    this_type&						operator=		(this_type&& aSharedPtr) noexcept;

    template<typename TSP, typename = IsConvertible<TSP, this_type>>
    this_type&						operator=		(const TSP& aSharedPtr) noexcept
    {
        Set(aSharedPtr);
        return *this;
    }

    template<typename TSP, typename = IsConvertible<TSP, this_type>>
    this_type&						operator=		(TSP&& aSharedPtr) noexcept
    {
        Set(std::move(aSharedPtr));
        return *this;
    }

    //----------------- user-defined conversion -------------------------
    /*template<typename TSP, typename = ConvertibleTo<TSP>>
    explicit operator TSP () const noexcept
    {
        RefPtrT r = _RefCounter();
        r->Accuire();
        return TSP(r);
    }

    template<typename TSP, typename = ConvertibleTo<TSP>>
    explicit operator TSP& () noexcept
    {
        return reinterpret_cast<TSP&>(*this);
    }

    template<typename TSP, typename = ConvertibleTo<TSP>>
    explicit operator const TSP& () const noexcept
    {
        return reinterpret_cast<const TSP&>(*this);
    }

    template<typename TSP, typename = ConvertibleTo<TSP>>
    explicit operator TSP&& () noexcept
    {
        return std::move(reinterpret_cast<TSP&&>(*this));
    }*/

    /*template<typename TSP, typename = ConvertibleFromOrTo<TSP>>
    TSP&			CastToR				(void) noexcept
    {
        return reinterpret_cast<TSP&>(*this);
    }

    template<typename TSP, typename = ConvertibleFromOrTo<TSP>>
    const TSP&		CastToRC			(void) const noexcept
    {
        return reinterpret_cast<const TSP&>(*this);
    }

    template<typename TSP, typename = ConvertibleFromOrTo<TSP>>
    TSP&&			CastToM				(void) noexcept
    {
        return std::move(reinterpret_cast<TSP&&>(*this));
    }*/

    /*template<typename TSP, typename = SafeConvertibleTo<TSP>>
    explicit operator TSP& () const noexcept
    {
        return Cast<TSP&>();
    }*/

    //*******************************************************************
    /*template<typename TSP, typename = IsConvertible<TSP, this_type>>
    TSP				Cast				(void) const noexcept
    {
        return std::forward<TSP>(reinterpret_cast<TSP>(const_cast<this_type&>(*this)));
    }

    template<typename TSP, typename = IsConvertible<TSP, this_type>>
    TSP				Cast				(void) noexcept
    {
        return std::forward<TSP>(reinterpret_cast<TSP>(*this));
    }

    template<typename TSP, typename = IsConvertible<this_type, TSP>>
    TSP				RCast				(void) const noexcept
    {
        return std::forward<TSP>(reinterpret_cast<TSP>(const_cast<this_type&>(*this)));
    }

    template<typename TSP, typename = IsConvertible<this_type, TSP>>
    TSP				RCast				(void) noexcept
    {
        return std::forward<TSP>(reinterpret_cast<TSP>(*this));
    }*/

    //***********************************************************

    /*template<typename TSP, typename = ConvertibleFromOrTo<TSP>>
    TSP				CastTo				(void) noexcept
    {
        return std::forward<TSP>(reinterpret_cast<TSP>(*this));
    }*/
/*
    //---------------------------------------------------------------------
    static void		SDeepCopyVec		(const GpVector<this_type>&	aSrc,
                                         GpVector<this_type>&		aDst);
    template<typename KeyT>
    static void		SDeepCopyMap		(const GpMap<KeyT, this_type>&	aSrc,
                                         GpMap<KeyT, this_type>&		aDst)
    {
        aDst.clear();

        for (auto&& iter: aSrc)
        {
            aDst.emplace(iter.first, iter.second.Clone());
        }
    }*/

    RefPtrT				_RefCounter			(void) const noexcept {return iRefCounter;}
    RefPtrT				_MoveRefCounter		(void) noexcept {RefPtrT r = iRefCounter; iRefCounter = nullptr; return r;}

private:
    RefPtrT				iRefCounter = nullptr;
};

template <typename T, typename RefPtrT, bool _IsWeak>
GpSharedPtrBase<T, RefPtrT, _IsWeak>::GpSharedPtrBase (RefPtrT aRefCounter) noexcept:
iRefCounter(aRefCounter)
{
}

template <typename T, typename RefPtrT, bool _IsWeak>
GpSharedPtrBase<T, RefPtrT, _IsWeak>::GpSharedPtrBase (void) noexcept
{
}

template <typename T, typename RefPtrT, bool _IsWeak>
GpSharedPtrBase<T, RefPtrT, _IsWeak>::GpSharedPtrBase (const this_type& aSharedPtr) noexcept:
iRefCounter(aSharedPtr._RefCounter())
{
    if (iRefCounter)
    {
        iRefCounter->template Accuire<_IsWeak>();
    }
}

template <typename T, typename RefPtrT, bool _IsWeak>
GpSharedPtrBase<T, RefPtrT, _IsWeak>::GpSharedPtrBase (this_type&& aSharedPtr) noexcept:
iRefCounter(aSharedPtr._MoveRefCounter())
{
}

template <typename T, typename RefPtrT, bool _IsWeak>
GpSharedPtrBase<T, RefPtrT, _IsWeak>::~GpSharedPtrBase (void) noexcept
{
    Clear();
}

template <typename T, typename RefPtrT, bool _IsWeak>
void	GpSharedPtrBase<T, RefPtrT, _IsWeak>::Clear (void) noexcept
{
    if (iRefCounter)
    {
        const_cast<GpReferenceCounter*>(iRefCounter)->Release<_IsWeak>();
        iRefCounter = nullptr;
    }
}

/*template <typename T, typename RefPtrT, bool _IsWeak>
typename GpSharedPtrBase<T, RefPtrT, _IsWeak>::this_type	GpSharedPtrBase<T, RefPtrT, _IsWeak>::Clone (void) const
{
    if (IsNotNULL())
    {
        if constexpr (std::is_base_of<GpReflectable, T>())
        {
            auto		c = VCn().DeepCopy();
            this_type	t(c._RefCounter());
            c._ClearRefCounter();
            return t;
        } else
        {
            return SNew(VC());
        }
    } else
    {
        return SNull();
    }
}*/

template <typename T, typename RefPtrT, bool _IsWeak>
void	GpSharedPtrBase<T, RefPtrT, _IsWeak>::Set (const this_type& aSharedPtr) noexcept
{
    if (iRefCounter != aSharedPtr.iRefCounter)
    {
        Clear();
        iRefCounter = aSharedPtr.iRefCounter;

        if (iRefCounter)
        {
            iRefCounter->template Accuire<_IsWeak>();
        }
    }
}

template <typename T, typename RefPtrT, bool _IsWeak>
void	GpSharedPtrBase<T, RefPtrT, _IsWeak>::Set (this_type&& aSharedPtr) noexcept
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

template <typename T, typename RefPtrT, bool _IsWeak>
count_t	GpSharedPtrBase<T, RefPtrT, _IsWeak>::Counter (void) const noexcept
{
    if (iRefCounter != nullptr)
    {
        return iRefCounter->Counter();
    } else
    {
        return 0_cnt;
    }
}

template <typename T, typename RefPtrT, bool _IsWeak>
bool	GpSharedPtrBase<T, RefPtrT, _IsWeak>::IsNULL (void) const noexcept
{
    return iRefCounter == nullptr;
}

template <typename T, typename RefPtrT, bool _IsWeak>
bool	GpSharedPtrBase<T, RefPtrT, _IsWeak>::IsNotNULL (void) const noexcept
{
    return iRefCounter != nullptr;
}

template <typename T, typename RefPtrT, bool _IsWeak>
T&	GpSharedPtrBase<T, RefPtrT, _IsWeak>::V (void)
{
    if (iRefCounter != nullptr)
    {
        if constexpr(_IsWeak)
        {
            if (iRefCounter->Counter() == 0)
            {
                THROW_GPE("Weak shared pointer counter is 0"_sv);
            }
        }

        return iRefCounter->template Value<T>();
    } else
    {
        THROW_GPE("Shared pointer is empty"_sv);
    }
}

template <typename T, typename RefPtrT, bool _IsWeak>
T&	GpSharedPtrBase<T, RefPtrT, _IsWeak>::Vn (void) noexcept
{
    return iRefCounter->template Value<T>();
}

template <typename T, typename RefPtrT, bool _IsWeak>
T*	GpSharedPtrBase<T, RefPtrT, _IsWeak>::P (void)
{
    if (iRefCounter != nullptr)
    {
        if constexpr(_IsWeak)
        {
            if (iRefCounter->Counter() == 0)
            {
                THROW_GPE("Weak shared pointer counter is 0"_sv);
            }
        }

        return iRefCounter->template Ptr<T>();
    } else
    {
        THROW_GPE("Shared pointer is empty"_sv);
    }
}

template <typename T, typename RefPtrT, bool _IsWeak>
T*	GpSharedPtrBase<T, RefPtrT, _IsWeak>::Pn (void) noexcept
{
    return iRefCounter->template Ptr<T>();
}

template <typename T, typename RefPtrT, bool _IsWeak>
const T&	GpSharedPtrBase<T, RefPtrT, _IsWeak>::VC (void) const
{
    if (iRefCounter != nullptr)
    {
        if constexpr(_IsWeak)
        {
            if (iRefCounter->Counter() == 0)
            {
                THROW_GPE("Weak shared pointer counter is 0"_sv);
            }
        }

        return iRefCounter->template Value<T>();
    } else
    {
        THROW_GPE("Shared pointer is empty"_sv);
    }
}

template <typename T, typename RefPtrT, bool _IsWeak>
const T&	GpSharedPtrBase<T, RefPtrT, _IsWeak>::VCn (void) const noexcept
{
    return iRefCounter->template Value<T>();
}

template <typename T, typename RefPtrT, bool _IsWeak>
const T*	GpSharedPtrBase<T, RefPtrT, _IsWeak>::PC (void) const
{
    if (iRefCounter != nullptr)
    {
        if constexpr(_IsWeak)
        {
            if (iRefCounter->Counter() == 0)
            {
                THROW_GPE("Weak shared pointer counter is 0"_sv);
            }
        }

        return iRefCounter->template Ptr<T>();
    } else
    {
        THROW_GPE("Shared pointer is empty"_sv);
    }
}

template <typename T, typename RefPtrT, bool _IsWeak>
const T*	GpSharedPtrBase<T, RefPtrT, _IsWeak>::PCn (void) const noexcept
{
    return iRefCounter->template Ptr<T>();
}

template <typename T, typename RefPtrT, bool _IsWeak>
bool	GpSharedPtrBase<T, RefPtrT, _IsWeak>::operator!= (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter != aSharedPtr.iRefCounter;
}

template <typename T, typename RefPtrT, bool _IsWeak>
bool	GpSharedPtrBase<T, RefPtrT, _IsWeak>::operator== (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter == aSharedPtr.iRefCounter;
}

template <typename T, typename RefPtrT, bool _IsWeak>
bool	GpSharedPtrBase<T, RefPtrT, _IsWeak>::operator> (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter > aSharedPtr.iRefCounter;
}

template <typename T, typename RefPtrT, bool _IsWeak>
bool	GpSharedPtrBase<T, RefPtrT, _IsWeak>::operator< (const this_type& aSharedPtr) const noexcept
{
    return iRefCounter < aSharedPtr.iRefCounter;
}

template <typename T, typename RefPtrT, bool _IsWeak>
typename GpSharedPtrBase<T, RefPtrT, _IsWeak>::this_type&	GpSharedPtrBase<T, RefPtrT, _IsWeak>::operator= (const this_type& aSharedPtr) noexcept
{
    Set(aSharedPtr);
    return *this;
}

template <typename T, typename RefPtrT, bool _IsWeak>
typename GpSharedPtrBase<T, RefPtrT, _IsWeak>::this_type&	GpSharedPtrBase<T, RefPtrT, _IsWeak>::operator= (this_type&& aSharedPtr) noexcept
{
    Set(std::move(aSharedPtr));
    return *this;
}

template<typename T>
using GpSP	= GpSharedPtrBase<T, GpReferenceCounter*, false>;

template<typename T>
using GpCSP	= GpSharedPtrBase<T, const GpReferenceCounter*, false>;

template<typename T>
using GpWP	= GpSharedPtrBase<T, GpReferenceCounter*, true>;

template<typename T>
using GpCWP	= GpSharedPtrBase<T, const GpReferenceCounter*, true>;

}//namespace GPlatform

#endif//#if defined(GP_USE_SHARED_POINTERS)
