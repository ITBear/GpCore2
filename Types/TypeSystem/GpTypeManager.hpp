#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructInfo.hpp"
#include "../Containers/GpElementsCatalog.hpp"

namespace GPlatform {

class GPCORE_API GpTypeManager
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTypeManager)
    CLASS_DECLARE_DEFAULTS(GpTypeManager)
    CLASS_TAG(THREAD_SAFE)

    using ElementsT = GpElementsCatalog<GpUUID, std::reference_wrapper<const GpTypeStructInfo>>;

public:
                                    GpTypeManager   (void) noexcept;
                                    ~GpTypeManager  (void) noexcept;

    static GpTypeManager&           S               (void) noexcept;

    size_t                          Register        (const GpTypeStructInfo& aTypeInfo);
    void                            Unregister      (const GpUUID& aTypeUID);
    GpTypeStructInfo::C::Opt::CRef  Find            (const GpUUID& aTypeUID) const noexcept;
    bool                            IsBaseOf        (const GpUUID& aBaseTypeUID, const GpUUID& aDerivedTypeUID) const noexcept;

    void                            UnregisterGroup (const GpUUID& aGroupID);

    template<typename TO_SP, typename FROM_SP>
    [[nodiscard]] TO_SP             CastSP          (FROM_SP& aFrom) const;

    template<typename TO, typename FROM>
    [[nodiscard]] TO&               Cast            (FROM& aFrom) const;

private:
    ElementsT                       iElements;
};

template<typename TO_SP, typename FROM_SP>
[[nodiscard]] TO_SP GpTypeManager::CastSP (FROM_SP& aFrom) const
{
    using TO_VAL_T = typename TO_SP::value_type;

    const GpUUID fromUID    = aFrom.VC().TypeUID();
    const GpUUID toUID      = TO_VAL_T::STypeUID();

    THROW_GPE_COND
    (
        IsBaseOf(toUID, fromUID),
        [&](){return "Failed to cast from UID "_sv + fromUID.ToString() + " to UID " + toUID.ToString();}
    );

    return aFrom.template CastAs<TO_SP>();
}

template<typename TO, typename FROM>
[[nodiscard]] TO&   GpTypeManager::Cast (FROM& aFrom) const
{
    using TO_VAL_T = std::remove_cv_t<TO>;

    const GpUUID fromUID    = aFrom.TypeUID();
    const GpUUID toUID      = TO_VAL_T::STypeUID();

    THROW_GPE_COND
    (
        IsBaseOf(toUID, fromUID),
        [&](){return "Failed to cast from UID "_sv + fromUID.ToString() + " to UID " + toUID.ToString();}
    );

    return static_cast<TO&>(aFrom);
}

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
