#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/TypeTraits/GpTypeInfoUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/Config/IncludeExt/fmt.hpp>

#include <typeinfo>

namespace GPlatform {

class GpAny;

namespace AnyConcepts {

template <typename T>
concept IsNotAny = !std::is_same_v<std::remove_cvref_t<T>, GpAny>;

} // namespace AnyConcepts

class GpAnyHolderBase
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpAnyHolderBase)

public:
    struct PayloadT
    {
        const std::type_info&   iTypeInfo;
        const void*             iPtr;
    };

protected:
                        GpAnyHolderBase     (void) noexcept = default;

public:
    virtual             ~GpAnyHolderBase    (void) noexcept = default;

    virtual PayloadT    Payload             (void) const noexcept = 0;
};

template<AnyConcepts::IsNotAny T>
class GpAnyHolder final: public GpAnyHolderBase
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpAnyHolder)

public:
    explicit            GpAnyHolder     (const T& aValue): iValue{aValue} {}
    explicit            GpAnyHolder     (T&& aValue): iValue{std::move(aValue)} {}
    virtual             ~GpAnyHolder    (void) noexcept override final = default;

    virtual PayloadT    Payload         (void) const noexcept override final
    {
        return PayloadT
        {
            .iTypeInfo  = typeid(T),
            .iPtr       = &iValue
        };
    }

private:
    T iValue;
};

class GpAny
{
    CLASS_REMOVE_CTRS_COPY(GpAny)

private:
    template<typename T>
    using AnyHolderT = GpAnyHolder<std::decay_t<T>>;

public:
                        GpAny           (void) noexcept = default;
    inline              GpAny           (GpAny&& aAny) noexcept;

    template<AnyConcepts::IsNotAny T>
    explicit            GpAny           (const T& aValue);

    template<AnyConcepts::IsNotAny T>
    explicit            GpAny           (T&& aValue);
                        ~GpAny          (void) noexcept = default;

    inline void         Clear           (void) noexcept;
    inline bool         Empty           (void) const noexcept;

    inline GpAny&       operator=       (GpAny&& aAny) noexcept;

    template<AnyConcepts::IsNotAny T>
    GpAny&              operator=       (T&& aValue);

    inline const std::type_info&
                        TypeInfo        (void) const noexcept;

    template<AnyConcepts::IsNotAny T>
    bool                IsContatinType  (void) const noexcept;

    template<AnyConcepts::IsNotAny T>
    const T&            Value           (void) const;

    template<AnyConcepts::IsNotAny T>
    T&                  Value           (void);

    template<AnyConcepts::IsNotAny T>
    const T&            ValueNoCheck    (void) const;

    template<AnyConcepts::IsNotAny T>
    T&                  ValueNoCheck    (void);

private:
    std::unique_ptr<GpAnyHolderBase> iPtrHolder;
};

GpAny::GpAny (GpAny&& aAny) noexcept:
iPtrHolder{std::move(aAny.iPtrHolder)}
{
}

template<AnyConcepts::IsNotAny T>
GpAny::GpAny (const T& aValue):
iPtrHolder{std::make_unique<AnyHolderT<T>>(aValue)}
{
}

template<AnyConcepts::IsNotAny T>
GpAny::GpAny (T&& aValue):
iPtrHolder{std::make_unique<AnyHolderT<T>>(std::forward<T>(aValue))}
{
}

void GpAny::Clear (void) noexcept
{
    iPtrHolder.reset();
}

bool GpAny::Empty (void) const noexcept
{
    return !iPtrHolder;
}

GpAny&  GpAny::operator= (GpAny&& aAny) noexcept
{
    iPtrHolder = std::move(aAny.iPtrHolder);

    return *this;
}

template<AnyConcepts::IsNotAny T>
GpAny& GpAny::operator= (T&& aValue)
{
    iPtrHolder = std::make_unique<AnyHolderT<T>>(std::forward<T>(aValue));

    return *this;
}

const std::type_info& GpAny::TypeInfo (void) const noexcept
{
    if (iPtrHolder) [[likely]]
    {
        return iPtrHolder->Payload().iTypeInfo;
    } else
    {
        return typeid(nullptr);
    }
}

template<AnyConcepts::IsNotAny T>
bool GpAny::IsContatinType (void) const noexcept
{
    if (!iPtrHolder) [[unlikely]]
    {
        return false;
    }

    const GpAnyHolderBase::PayloadT&    thisPayload     = iPtrHolder->Payload();
    const std::type_info&               thisTypeInfo    = thisPayload.iTypeInfo;
    const std::type_info&               otherTypeInfo   = typeid(std::decay_t<T>);

    return GpTypeInfoUtils::SIsSame(thisTypeInfo, otherTypeInfo);
}

template<AnyConcepts::IsNotAny T>
const T&    GpAny::Value (void) const
{
    VERIFY
    (
        iPtrHolder != nullptr,
        "Value is null"_sv
    );

    const GpAnyHolderBase::PayloadT&    thisPayload     = iPtrHolder->Payload();
    const std::type_info&               thisTypeInfo    = thisPayload.iTypeInfo;
    const std::type_info&               otherTypeInfo   = typeid(std::decay_t<T>);

    VERIFY
    (
        GpTypeInfoUtils::SIsSame(thisTypeInfo, otherTypeInfo),
        [&]()
        {
            return fmt::format
            (
                "Bad cast: from '{}' to '{}'",
                thisTypeInfo.name(),
                otherTypeInfo.name()
            );
        }
    );

    return *static_cast<const std::decay_t<T>*>(thisPayload.iPtr);
}

template<AnyConcepts::IsNotAny T>
T&  GpAny::Value (void)
{
     return const_cast<T&>(const_cast<const GpAny&>(*this).Value<T>());
}

template<AnyConcepts::IsNotAny T>
const T&    GpAny::ValueNoCheck (void) const
{
    const GpAnyHolderBase::PayloadT& thisPayload = iPtrHolder->Payload();

    return *static_cast<const std::decay_t<T>*>(thisPayload.iPtr);
}

template<AnyConcepts::IsNotAny T>
T&  GpAny::ValueNoCheck (void)
{
     return const_cast<T&>(const_cast<const GpAny&>(*this).ValueNoCheck<T>());
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
