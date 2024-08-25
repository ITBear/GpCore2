#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/TypeTraits/GpTypeInfoUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#include <typeinfo>
#include <tuple>

namespace GPlatform {

class GpAny;

namespace AnyConcepts {

template <typename T>
concept IsNotAny = requires()
{
    requires !std::is_same_v<std::remove_cvref_t<T>, GpAny>;
};

}// namespace AnyConcepts

class GpAnyHolderBase
{
public:
                                    GpAnyHolderBase     (void) noexcept = default;
    virtual                         ~GpAnyHolderBase    (void) noexcept = default;

    virtual std::tuple<const std::type_info&, const void*>
                                    TypeInfoAndPtr      (void) const noexcept = 0;
    virtual GpAnyHolderBase*        Copy                (void) const = 0;
    virtual GpAnyHolderBase*        Move                (void) const = 0;
};

template<AnyConcepts::IsNotAny T>
class GpAnyHolder final: public GpAnyHolderBase
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpAnyHolder)

public:
                                    GpAnyHolder     (const T& aValue): GpAnyHolderBase(), iValue(aValue) {}
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
    struct C
    {
        struct Opt
        {
            using Val = std::optional<GpAny>;
        };
    };

public:
                                    GpAny           (void) noexcept = default;
    inline                          GpAny           (const GpAny& aAny);
    inline                          GpAny           (GpAny&& aAny) noexcept;

    template<AnyConcepts::IsNotAny T>
                                    GpAny           (const T& aValue);
    template<AnyConcepts::IsNotAny T>
                                    GpAny           (T& aValue);
    template<AnyConcepts::IsNotAny T>
                                    GpAny           (T&& aValue);
    template<AnyConcepts::IsNotAny T,
             typename... Ts>
                                    GpAny           (Ts&&... aArgs):
                                    iPtrHolder(new GpAnyHolder<T>(std::forward<Ts>(aArgs)...))
                                    {
                                    }

    inline                          ~GpAny          (void) noexcept;

    inline void                     Clear           (void) noexcept;
    inline bool                     Empty           (void) const noexcept;

    template<AnyConcepts::IsNotAny T>
    GpAny&                          operator=       (const T& aValue);

    template<AnyConcepts::IsNotAny T>
    GpAny&                          operator=       (T& aValue);

    template<AnyConcepts::IsNotAny T>
    GpAny&                          operator=       (T&& aValue);

    inline GpAny&                   operator=       (const GpAny& aAny);
    inline GpAny&                   operator=       (GpAny&& aAny);

    inline const std::type_info&    TypeInfo        (void) const noexcept;

    template<AnyConcepts::IsNotAny T>
    bool                            IsContatinType  (void) const noexcept;

    template<AnyConcepts::IsNotAny T>
    const T&                        Value           (void) const;

    template<AnyConcepts::IsNotAny T>
    T&                              Value           (void);

    template<AnyConcepts::IsNotAny T>
    const T&                        ValueNoCheck    (void) const;

    template<AnyConcepts::IsNotAny T>
    T&                              ValueNoCheck    (void);

private:
    GpAnyHolderBase*                iPtrHolder  = nullptr;
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

template<AnyConcepts::IsNotAny T>
GpAny::GpAny (const T& aValue):
iPtrHolder(new GpAnyHolder<T>(aValue))
{
}

template<AnyConcepts::IsNotAny T>
GpAny::GpAny (T& aValue):
iPtrHolder(new GpAnyHolder<T>(aValue))
{
}

template<AnyConcepts::IsNotAny T>
GpAny::GpAny (T&& aValue):
iPtrHolder(new GpAnyHolder<T>(std::move(aValue)))
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

template<AnyConcepts::IsNotAny T>
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

template<AnyConcepts::IsNotAny T>
GpAny&  GpAny::operator= (T& aValue)
{
    Clear();

    iPtrHolder = new GpAnyHolder<T>(aValue);

    return *this;
}

template<AnyConcepts::IsNotAny T>
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

template<AnyConcepts::IsNotAny T>
bool    GpAny::IsContatinType (void) const noexcept
{
    if (iPtrHolder == nullptr) [[unlikely]]
    {
        return false;
    }

    const auto              fromTypeInfoAndPtr  = iPtrHolder->TypeInfoAndPtr();
    const std::type_info&   toTypeInfo          = typeid(T);

    return GpTypeInfoUtils::SIsSame(std::get<0>(fromTypeInfoAndPtr), toTypeInfo);
}

template<AnyConcepts::IsNotAny T>
const T&    GpAny::Value (void) const
{
    THROW_COND_GP
    (
        iPtrHolder != nullptr,
        "Value is null"_sv
    );

    const auto              fromTypeInfoAndPtr  = iPtrHolder->TypeInfoAndPtr();
    const std::type_info&   toTypeInfo          = typeid(T);

    const std::type_info&   fromTypeInfo        = std::get<0>(fromTypeInfoAndPtr);
    const void*             valuePtr            = std::get<1>(fromTypeInfoAndPtr);

    THROW_COND_GP
    (
        GpTypeInfoUtils::SIsSame(fromTypeInfo, toTypeInfo),
        [&](){return "Bad cast: from '"_sv + fromTypeInfo.name() + "' to '"_sv + toTypeInfo.name() + "'"_sv;}
    );

    return *static_cast<const T*>(valuePtr);
}

template<AnyConcepts::IsNotAny T>
T&  GpAny::Value (void)
{
    return const_cast<T&>(const_cast<const GpAny&>(*this).Value<T>());
}

template<AnyConcepts::IsNotAny T>
const T&    GpAny::ValueNoCheck (void) const
{
    THROW_COND_GP
    (
        iPtrHolder != nullptr,
        "Value is null"_sv
    );

    return *static_cast<const T*>(std::get<1>(iPtrHolder->TypeInfoAndPtr()));
}

template<AnyConcepts::IsNotAny T>
T&  GpAny::ValueNoCheck (void)
{
    return const_cast<T&>(const_cast<const GpAny&>(*this).ValueNoCheck<T>());
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
