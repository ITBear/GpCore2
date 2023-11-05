#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include <typeinfo>

#include "../../Macro/GpMacroClass.hpp"
#include "../../Exceptions/GpException.hpp"
#include "../../TypeTraits/GpTypeInfoUtils.hpp"
#include "../Strings/GpStringOps.hpp"

#include <tuple>

namespace GPlatform {

class GpAnyHolderBase
{
public:
    explicit                        GpAnyHolderBase     (void) noexcept = default;
    virtual                         ~GpAnyHolderBase    (void) noexcept = default;

    virtual std::tuple<const std::type_info&, const void*>
                                    TypeInfoAndPtr      (void) const noexcept = 0;
    virtual GpAnyHolderBase*        Copy                (void) const = 0;
    virtual GpAnyHolderBase*        Move                (void) const = 0;
};

template<typename T>
class GpAnyHolder final: public GpAnyHolderBase
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpAnyHolder)

public:
                                    GpAnyHolder     (const T& aValue): GpAnyHolderBase(), iValue(aValue) {}
                                    GpAnyHolder     (T& aValue): GpAnyHolderBase(), iValue(aValue) {}
                                    GpAnyHolder     (T&& aValue): GpAnyHolderBase(), iValue(std::move(aValue)) {}
    template<typename... Ts>
                                    GpAnyHolder     (Ts&&... aArgs): GpAnyHolderBase(), iValue(std::forward<Ts>(aArgs)...) {}
    virtual                         ~GpAnyHolder    (void) noexcept override final = default;

    virtual std::tuple<const std::type_info&, const void*>
                                    TypeInfoAndPtr  (void) const noexcept override final {return {typeid(T), &iValue};}
    virtual GpAnyHolderBase*        Copy            (void) const override final {return new GpAnyHolder<T>(iValue);}
    virtual GpAnyHolderBase*        Move            (void) const override final {return new GpAnyHolder<T>(std::move(iValue));}

private:
    T                               iValue;
};

class GpAny
{
public:
                                    GpAny           (void) noexcept = default;
    inline                          GpAny           (const GpAny& aAny);
    inline                          GpAny           (GpAny&& aAny) noexcept;

    template<typename T>
                                    GpAny           (const T& aValue);
    template<typename T>
                                    GpAny           (T& aValue);
    template<typename T>
                                    GpAny           (T&& aValue);
    template<typename T,
             typename... Ts>
                                    GpAny           (Ts&&... aArgs);

    inline                          ~GpAny          (void) noexcept;

    inline void                     Clear           (void) noexcept;
    inline bool                     Empty           (void) const noexcept;

    template<typename T>
    GpAny&                          operator=       (const T& aValue);

    template<typename T>
    GpAny&                          operator=       (T& aValue);

    template<typename T>
    GpAny&                          operator=       (T&& aValue);

    inline GpAny&                   operator=       (const GpAny& aAny);
    inline GpAny&                   operator=       (GpAny&& aAny);

    inline const std::type_info&    TypeInfo        (void) const noexcept;

    template<typename T>
    const T&                        Value           (void) const;

    template<typename T>
    T&                              Value           (void);

    template<typename T>
    const T&                        ValueNoCheck    (void) const;

    template<typename T>
    T&                              ValueNoCheck    (void);

private:
    GpAnyHolderBase*                iPtrHolder = nullptr;
};

GpAny::GpAny (const GpAny& aAny):
iPtrHolder
(
     (aAny.iPtrHolder != nullptr) ?
      aAny.iPtrHolder->Copy()
    : nullptr
)
{
}

GpAny::GpAny (GpAny&& aAny) noexcept:
iPtrHolder(std::move(aAny.iPtrHolder))
{
    aAny.iPtrHolder = nullptr;
}

template<typename T>
GpAny::GpAny (const T& aValue):
iPtrHolder(new GpAnyHolder<T>(aValue))
{
}

template<typename T>
GpAny::GpAny (T& aValue):
iPtrHolder(new GpAnyHolder<T>(aValue))
{
}

template<typename T>
GpAny::GpAny (T&& aValue):
iPtrHolder(new GpAnyHolder<T>(std::move(aValue)))
{
}

template<typename T,
         typename... Ts>
GpAny::GpAny (Ts&&... aArgs):
iPtrHolder(new GpAnyHolder<T>(std::forward<Ts>(aArgs)...))
{
}

GpAny::~GpAny (void) noexcept
{
    Clear();
}

void    GpAny::Clear (void) noexcept
{
    if (iPtrHolder) [[likely]]
    {
        delete iPtrHolder;
        iPtrHolder = nullptr;
    }
}

bool    GpAny::Empty (void) const noexcept
{
    return iPtrHolder == nullptr;
}

template<typename T>
GpAny&  GpAny::operator= (const T& aValue)
{
    Clear();

    iPtrHolder = new GpAnyHolder<T>(aValue);

    return *this;
}

GpAny&  GpAny::operator= (const GpAny& aAny)
{
    Clear();

    iPtrHolder = (aAny.iPtrHolder != nullptr) ?
      aAny.iPtrHolder->Copy()
    : nullptr;

    return *this;
}

GpAny&  GpAny::operator= (GpAny&& aAny)
{
    Clear();

    iPtrHolder = aAny.iPtrHolder;
    aAny.iPtrHolder = nullptr;

    return *this;
}

template<typename T>
GpAny&  GpAny::operator= (T& aValue)
{
    Clear();

    iPtrHolder = new GpAnyHolder<T>(aValue);

    return *this;
}

template<typename T>
GpAny&  GpAny::operator= (T&& aValue)
{
    Clear();

    iPtrHolder = new GpAnyHolder<T>(std::move(aValue));

    return *this;
}

const std::type_info&   GpAny::TypeInfo (void) const noexcept
{
    if (iPtrHolder) [[likely]]
    {
        return std::get<0>(iPtrHolder->TypeInfoAndPtr());
    } else
    {
        return typeid(nullptr);
    }
}

template<typename T>
const T&    GpAny::Value (void) const
{
    THROW_COND_GP
    (
        iPtrHolder != nullptr,
        u8"Value is null"_sv
    );

    const auto              fromTypeInfoAndPtr  = iPtrHolder->TypeInfoAndPtr();
    const std::type_info&   toTypeInfo          = typeid(T);

    const std::type_info&   fromTypeInfo        = std::get<0>(fromTypeInfoAndPtr);
    const void*             valuePtr            = std::get<1>(fromTypeInfoAndPtr);

    THROW_COND_GP
    (
        GpTypeInfoUtils::SIsSame(fromTypeInfo, toTypeInfo),
        [&](){return u8"Bad cast: from '"_sv + fromTypeInfo.name() + u8"' to '"_sv + toTypeInfo.name() + u8"'"_sv;}
    );

    return *static_cast<const T*>(valuePtr);
}

template<typename T>
T&  GpAny::Value (void)
{
    return const_cast<T&>(const_cast<const GpAny&>(*this).Value<T>());
}

template<typename T>
const T&    GpAny::ValueNoCheck (void) const
{
    THROW_COND_GP
    (
        iPtrHolder != nullptr,
        u8"Value is null"_sv
    );

    return *static_cast<const T*>(std::get<1>(iPtrHolder->TypeInfoAndPtr()));
}

template<typename T>
T&  GpAny::ValueNoCheck (void)
{
    return const_cast<T&>(const_cast<const GpAny&>(*this).ValueNoCheck<T>());
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
