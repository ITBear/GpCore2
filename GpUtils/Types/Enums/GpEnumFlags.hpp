#pragma once

#include "../../GpUtils_global.hpp"

#if defined(GP_USE_ENUMS)

#include "../../Macro/GpMacroClass.hpp"
#include "../Containers/GpContainersT.hpp"

namespace GPlatform {

TAG_REGISTER(GpEnum)

namespace EnumConcepts {

template <typename T>
concept IsEnum = requires()
{
    requires GpHasTag_GpEnum<T>();
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
                                    iValue(aFlags.iValue)
                                    {
                                    }

    constexpr                       GpEnumFlags (GpEnumFlags&& aFlags) noexcept:
                                    iValue(aFlags.iValue)
                                    {
                                    }

    constexpr explicit              GpEnumFlags (const value_type aValue) noexcept:
                                    iValue(aValue)
                                    {
                                    }

public:
    virtual                         ~GpEnumFlags(void) noexcept
                                    {
                                        iValue = 0;
                                    }

    constexpr void                  Clear               (void) noexcept {iValue = 0;}

    constexpr value_type            Value               (void) const noexcept {return iValue;}
    constexpr void                  SetAllFromRaw       (const value_type aValue) noexcept {iValue = aValue;}
    constexpr void                  CombineFromRaw      (const value_type aValue) noexcept {iValue |= aValue;}
    constexpr void                  Combine             (const value_type aId) noexcept {iValue |=  value_type(value_type(1) << aId);}
    constexpr void                  Clear               (const value_type aId) noexcept {iValue &= ~value_type(value_type(1) << aId);}
    constexpr bool                  Test                (const value_type aId) const noexcept {return iValue & value_type(value_type(1) << aId);}
    constexpr bool                  Empty               (void) const noexcept {return iValue == 0;}

    virtual const NamesListT&       Names               (void) const noexcept = 0;
    virtual void                    Combine             (std::string_view aEnumName) = 0;
    virtual void                    Clear               (std::string_view aEnumName) = 0;
    virtual bool                    Test                (std::string_view aEnumName) const = 0;
    virtual std::string_view        ToStringFlag        (const value_type aId) const = 0;
    virtual value_type              FromStringFlag      (std::string_view aEnumName) const = 0;

    std::vector<std::string>        ToStringArray       (void) const;
    std::vector<std::string_view>   ToStringViewArray   (void) const;
    void                            FromStringArray     (const std::vector<std::string>& aArray);
    void                            FromStringViewArray (const std::vector<std::string_view>& aArray);

    std::string                     Echo                (void) const;

protected:
    value_type                      iValue  = 0;
};

template<typename E> class GpEnumFlagsST final: public GpEnumFlags
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
                            GpEnumFlags(aFlags)
                            {
                            }

    constexpr               GpEnumFlagsST   (GpEnumFlagsST&& aFlags) noexcept:
                            GpEnumFlags(std::move(aFlags))
                            {
                            }

    constexpr               GpEnumFlagsST   (const init_mask_type aFlags) noexcept
                            {
                                Combine(aFlags);
                            }

    constexpr explicit      GpEnumFlagsST   (const value_type aFlagsRaw) noexcept:
                            GpEnumFlags(aFlagsRaw)
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

    constexpr void          Clear           (const EnumTE aEnum) noexcept
    {
        GpEnumFlags::Clear(GpEnumFlags::value_type(aEnum));
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

    virtual void            Clear           (std::string_view aEnumName) override final
    {
        GpEnumFlags::Clear(GpEnumFlags::value_type(E::SFromString(aEnumName)));
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

    this_type&              operator=       (const this_type& aFlags) noexcept
    {
        iValue = aFlags.iValue;
        return *this;
    }

    this_type&              operator|=      (const typename E::EnumT aFlag) noexcept
    {
        iValue |= value_type(value_type(1) << aFlag);
        return *this;
    }

    friend this_type        operator|       (const this_type& aFlagsLeft, const this_type& aFlagsRight) noexcept
    {
        return this_type(aFlagsLeft.iValue | aFlagsRight.iValue);
    }

    friend this_type        operator|       (const this_type& aFlagsLeft, const typename E::EnumT aFlagRight) noexcept
    {
        return this_type(aFlagsLeft.iValue | value_type(value_type(1) << aFlagRight));
    }

    template<typename T>
    friend this_type        operator|       (const typename T::EnumT aFlagLeft, const typename T::EnumT aFlagRight) noexcept;//Implemented in GpEnum

    friend this_type        operator|       (const typename E::EnumT aFlagLeft, const this_type& aFlagsRight) noexcept
    {
        return this_type(value_type(value_type(1) << aFlagLeft) | aFlagsRight.iValue);
    }
};

}// namespace GPlatform

// ********************** Hash *********************
namespace std {

template<> struct hash<GPlatform::GpEnumFlags>
{
    using argument_type = GPlatform::GpEnumFlags;
    using result_type   = ::std::size_t;

    result_type operator()(argument_type const& aArg) const noexcept
    {
        return result_type(aArg.Value());
    }
};

}// namespace std

#endif// #if defined(GP_USE_ENUMS)
