#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_ENUMS)

#include "GpEnum.hpp"

namespace GPlatform {

class GpEnumFlags
{
public:
    CLASS_DECLARE_DEFAULTS(GpEnumFlags)
    CLASS_TAG(GpEnumFlags)
    CLASS_TAG_DETECTOR(GpEnumFlags)

    using value_type = GpEnum::value_type;

protected:
                        GpEnumFlags (void) noexcept
                        {
                        }

                        GpEnumFlags (const GpEnumFlags& aFlags) noexcept:
                        iValue(aFlags.iValue)
                        {
                        }

                        GpEnumFlags (GpEnumFlags&& aFlags) noexcept:
                        iValue(aFlags.iValue)
                        {
                        }

                        GpEnumFlags (const value_type aValue) noexcept:
                        iValue(aValue)
                        {
                        }

public:
    virtual             ~GpEnumFlags(void) noexcept
                        {
                            iValue = 0;
                        }

    void                Clear       (void) noexcept {iValue = 0;}

    value_type          Value       (void) const noexcept {return iValue;}

    void                Set         (const value_type aId) noexcept {iValue |=  value_type(value_type(1) << aId);}
    void                Clear       (const value_type aId) noexcept {iValue &= ~value_type(value_type(1) << aId);}
    bool                Test        (const value_type aId) const noexcept {return iValue & value_type(value_type(1) << aId);}

    virtual void        Set         (std::string_view aEnumName) = 0;
    virtual void        Clear       (std::string_view aEnumName) = 0;
    virtual bool        Test        (std::string_view aEnumName) const = 0;

protected:
    value_type          iValue  = 0;
};


template<typename E> class GpEnumFlagsST: public GpEnumFlags
{
public:
    static_assert(std::is_base_of<GpEnum, E>::value, "E must be inherited from GpEnum");

    using this_type         = GpEnumFlagsST<E>;
    using EnumT             = E;
    using EnumTE            = typename E::EnumT;
    using init_mask_type    = std::initializer_list<EnumTE>;

public:
                        GpEnumFlagsST   (void) noexcept
                        {
                        }

                        GpEnumFlagsST   (const GpEnumFlagsST& aFlags) noexcept:
                        GpEnumFlags(aFlags)
                        {
                        }

                        GpEnumFlagsST   (GpEnumFlagsST&& aFlags) noexcept:
                        GpEnumFlags(std::move(aFlags))
                        {
                        }

                        GpEnumFlagsST   (const init_mask_type aFlags) noexcept
                        {
                            Set(aFlags);
                        }

                        GpEnumFlagsST   (const value_type aFlagsRaw) noexcept:
                        GpEnumFlags(aFlagsRaw)
                        {
                        }

    virtual             ~GpEnumFlagsST  (void) noexcept override final
                        {
                        }

    void                Set             (const init_mask_type aFlags) noexcept
    {
        for (const EnumTE e: aFlags)
        {
            Set(e);
        }
    }

    void                Set             (const EnumTE aEnum) noexcept
    {
        GpEnumFlags::Set(GpEnumFlags::value_type(aEnum));
    }

    void                Clear           (const EnumTE aEnum) noexcept
    {
        GpEnumFlags::Clear(GpEnumFlags::value_type(aEnum));
    }

    bool                Test            (const EnumTE aEnum) const noexcept
    {
        return GpEnumFlags::Test(GpEnumFlags::value_type(aEnum));
    }

    virtual void        Set             (std::string_view aEnumName) override final
    {
        GpEnumFlags::Set(GpEnumFlags::value_type(E::SFromString(aEnumName)));
    }

    virtual void        Clear           (std::string_view aEnumName) override final
    {
        GpEnumFlags::Clear(GpEnumFlags::value_type(E::SFromString(aEnumName)));
    }

    virtual bool        Test            (std::string_view aEnumName) const override final
    {
        return GpEnumFlags::Test(GpEnumFlags::value_type(E::SFromString(aEnumName)));
    }

    this_type&          operator=       (const this_type& aFlags) noexcept
    {
        iValue = aFlags.iValue;
        return *this;
    }

    friend this_type    operator|       (const this_type& aFlagsLeft, const this_type& aFlagsRight) noexcept
    {
        return this_type(aFlagsLeft.iValue | aFlagsRight.iValue);
    }

    friend this_type    operator|       (const this_type& aFlagsLeft, const typename E::EnumT aFlagRight) noexcept
    {
        return this_type(aFlagsLeft.iValue | value_type(value_type(1) << aFlagRight));
    }

    friend this_type    operator|       (const typename E::EnumT aFlagLeft, const typename E::EnumT aFlagRight) noexcept
    {
        return this_type(value_type(value_type(1) << aFlagLeft) | value_type(value_type(1) << aFlagRight));
    }

    friend this_type    operator|       (const typename E::EnumT aFlagLeft, const this_type& aFlagsRight) noexcept
    {
        return this_type(value_type(value_type(1) << aFlagLeft) | aFlagsRight.iValue);
    }
};

}//GPlatform

// ********************** Hash *********************
namespace std{

template<> struct hash<GPlatform::GpEnumFlags>
{
    using argument_type = GPlatform::GpEnumFlags;
    using result_type   = std::size_t;

    result_type operator()(argument_type const& aArg) const noexcept
    {
        return result_type(aArg.Value());
    }
};

}//std

#endif//#if defined(GP_USE_ENUMS)
