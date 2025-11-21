#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>

#if defined(GP_USE_ENUMS)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>

namespace GPlatform {

TAG_REGISTER(GpEnum)

namespace EnumConcepts {

template <typename T>
concept IsEnum = requires()
{
    requires GpHasTag_GpEnum<T>();
};

template <typename T>
concept IsEnumFlags = requires()
{
    requires GpHasTag_GpEnumFlags<T>();
};

}// namespace EnumConcepts

TAG_REGISTER(GpEnumFlags)

class GP_UTILS_API GpEnumFlags
{
public:
    CLASS_DD(GpEnumFlags)
    TAG_SET(GpEnumFlags)

    using value_type    = s_int_32;
    using NamesListT    = std::vector<std::tuple<std::string_view, value_type>>;

protected:
    constexpr                       GpEnumFlags (void) noexcept
                                    {
                                    }

    constexpr                       GpEnumFlags (const GpEnumFlags& aFlags) noexcept:
                                    iRawValue{aFlags.iRawValue}
                                    {
                                    }

    constexpr                       GpEnumFlags (GpEnumFlags&& aFlags) noexcept:
                                    iRawValue{aFlags.iRawValue}
                                    {
                                    }

    constexpr explicit              GpEnumFlags (const value_type aRawValue) noexcept:
                                    iRawValue{aRawValue}
                                    {
                                    }

public:
    virtual                         ~GpEnumFlags(void) noexcept
                                    {
                                        iRawValue = 0;
                                    }

    constexpr void                  Clear               (void) noexcept {iRawValue = 0;}

    constexpr value_type            RawValue            (void) const noexcept {return iRawValue;}
    constexpr void                  SetAllFromRaw       (const value_type aRawValue) noexcept {iRawValue = aRawValue;}
    constexpr void                  CombineFromRaw      (const value_type aRawValue) noexcept {iRawValue |= aRawValue;}
    constexpr void                  Combine             (const value_type aId) noexcept {iRawValue |=  value_type(value_type(1) << aId);}
    constexpr void                  Unset               (const value_type aId) noexcept {iRawValue &= ~value_type(value_type(1) << aId);}
    constexpr size_t                UpCount             (void) const noexcept {return BitOps::PopCount(iRawValue);}
    constexpr bool                  Test                (const value_type aId) const noexcept {return iRawValue & value_type(value_type(1) << aId);}
    constexpr bool                  TestAll             (const GpEnumFlags& aFlags) const noexcept {return (iRawValue & aFlags.iRawValue) == aFlags.iRawValue;}
    constexpr bool                  Empty               (void) const noexcept {return iRawValue == 0;}
    constexpr void                  ApplyMask           (const value_type aMask) noexcept {iRawValue &= aMask;}

    virtual const NamesListT&       Names               (void) const noexcept = 0;
    virtual void                    Combine             (std::string_view aEnumName) = 0;
    virtual void                    Unset               (std::string_view aEnumName) = 0;
    virtual bool                    Test                (std::string_view aEnumName) const = 0;
    virtual std::string_view        ToStringFlag        (const value_type aId) const = 0;
    virtual value_type              FromStringFlag      (std::string_view aEnumName) const = 0;

    std::vector<std::string>        ToStringArray       (void) const;
    std::vector<std::string_view>   ToStringViewArray   (void) const;
    void                            FromStringArray     (const std::vector<std::string>& aArray);
    void                            FromStringViewArray (const std::vector<std::string_view>& aArray);

    std::string                     Echo                (void) const;

protected:
    value_type iRawValue = 0;
};

template<typename E>
class GpEnumFlagsST final: public GpEnumFlags
{
public:
    using this_type         = GpEnumFlagsST<E>;
    using EnumT             = E;
    using EnumTE            = typename E::EnumT;
    using init_mask_type    = std::initializer_list<EnumTE>;

public:
    constexpr               GpEnumFlagsST   (void) noexcept
                            {
                            }

    constexpr               GpEnumFlagsST   (const GpEnumFlagsST& aFlags) noexcept:
                            GpEnumFlags{aFlags}
                            {
                            }

    constexpr               GpEnumFlagsST   (GpEnumFlagsST&& aFlags) noexcept:
                            GpEnumFlags{std::move(aFlags)}
                            {
                            }

    constexpr               GpEnumFlagsST   (const init_mask_type aFlags) noexcept
                            {
                                Combine(aFlags);
                            }

    constexpr explicit      GpEnumFlagsST   (const value_type aRawValue) noexcept:
                            GpEnumFlags{aRawValue}
                            {
                            }

    constexpr               GpEnumFlagsST   (const EnumT& aEnumT) noexcept
                            {
                                Combine(aEnumT.Value());
                            }

    constexpr explicit      GpEnumFlagsST   (const EnumTE aEnumTE) noexcept
                            {
                                Combine(aEnumTE);
                            }

    virtual                 ~GpEnumFlagsST  (void) noexcept override final
                            {
                            }

    constexpr void          Combine         (const init_mask_type aFlags) noexcept
    {
        for (const EnumTE e: aFlags)
        {
            Combine(e);
        }
    }

    constexpr void          Combine         (const EnumTE aEnum) noexcept
    {
        GpEnumFlags::Combine(GpEnumFlags::value_type(aEnum));
    }

    constexpr void          Combine         (const GpEnumFlagsST& aFlags) noexcept
    {
        GpEnumFlags::CombineFromRaw(aFlags.RawValue());
    }

    constexpr void          Unset           (const EnumTE aEnum) noexcept
    {
        GpEnumFlags::Unset(GpEnumFlags::value_type(aEnum));
    }

    constexpr bool          Test            (const EnumTE aEnum) const noexcept
    {
        return GpEnumFlags::Test(GpEnumFlags::value_type(aEnum));
    }

    virtual const NamesListT&   Names       (void) const noexcept override final
    {
        EnumT e;
        return e.Names();
    }

    virtual void            Combine         (std::string_view aEnumName) override final
    {
        GpEnumFlags::Combine(GpEnumFlags::value_type(E::SFromString(aEnumName)));
    }

    virtual void            Unset           (std::string_view aEnumName) override final
    {
        GpEnumFlags::Unset(GpEnumFlags::value_type(E::SFromString(aEnumName)));
    }

    virtual bool            Test            (std::string_view aEnumName) const override final
    {
        return GpEnumFlags::Test(GpEnumFlags::value_type(E::SFromString(aEnumName)));
    }

    virtual std::string_view    ToStringFlag    (const value_type aId) const override final
    {
        return E::SToString(typename E::EnumT(aId));
    }

    virtual value_type      FromStringFlag  (std::string_view aEnumName) const override final
    {
        return GpEnumFlags::value_type(E::SFromString(aEnumName));
    }

    bool                    operator==      (const this_type& aFlags) const noexcept
    {
        return iRawValue == aFlags.iRawValue;
    }

    this_type&              operator=       (const this_type& aFlags) noexcept
    {
        iRawValue = aFlags.iRawValue;
        return *this;
    }

    this_type&              operator|=      (const typename E::EnumT aFlag) noexcept
    {
        iRawValue |= value_type(value_type(1) << aFlag);
        return *this;
    }

    friend this_type        operator|       (const this_type& aFlagsLeft, const this_type& aFlagsRight) noexcept
    {
        return this_type(aFlagsLeft.iRawValue | aFlagsRight.iRawValue);
    }

    friend this_type        operator|       (const this_type& aFlagsLeft, const typename E::EnumT aFlagRight) noexcept
    {
        return this_type(aFlagsLeft.iRawValue | value_type(value_type(1) << aFlagRight));
    }

    template<typename T>
    friend this_type        operator|       (const typename T::EnumT aFlagLeft, const typename T::EnumT aFlagRight) noexcept;//Implemented in GpEnum

    friend this_type        operator|       (const typename E::EnumT aFlagLeft, const this_type& aFlagsRight) noexcept
    {
        return this_type(value_type(value_type(1) << aFlagLeft) | aFlagsRight.iRawValue);
    }
};

}// namespace GPlatform

//********************** fmt *********************
namespace FMT_NAMESPASE {

template<typename T>
struct formatter<GpEnumFlagsST<T>>
{
    using enum_flags_type = GpEnumFlagsST<T>;

    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(const enum_flags_type& aEnumFlags, FormatContext& aCtx) const
    {
        return ::fmt::format_to(aCtx.out(), "{}", aEnumFlags.ToStringViewArray());
    }
};

}// namespace FMT_NAMESPASE

// ********************** Hash *********************
namespace std {

template<> struct hash<GPlatform::GpEnumFlags>
{
    using argument_type = GPlatform::GpEnumFlags;
    using result_type   = ::std::size_t;

    result_type operator()(argument_type const& aArg) const noexcept
    {
        return result_type(aArg.RawValue());
    }
};

inline string to_string(const ::GPlatform::GpEnumFlags& aEnumFlags)
{
    return ::fmt::format("{}", ::fmt::join(aEnumFlags.ToStringViewArray(), ","));
}

}// namespace std

#endif// #if defined(GP_USE_ENUMS)
