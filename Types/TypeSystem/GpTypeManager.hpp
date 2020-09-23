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

    using ElementsT = GpElementsCatalog<GpUUID, GpTypeStructInfo, GpMap>;

public:
                                    GpTypeManager   (void) noexcept;
                                    ~GpTypeManager  (void) noexcept;

    static GpTypeManager&           S               (void) noexcept;

    size_t                          Register        (const GpTypeStructInfo& aTypeInfo);
    size_t                          Register        (GpTypeStructInfo&& aTypeInfo);
    void                            Unregister      (const GpUUID& aTypeUID);
    GpTypeStructInfo::C::Opt::CRef  Find            (const GpUUID& aTypeUID) const noexcept;
    bool                            IsBaseOf        (const GpUUID& aBaseTypeUID, const GpUUID& aDerivedTypeUID) const noexcept;

    void                            UnregisterGroup (const GpUUID& aGroupID);

private:
    ElementsT                       iElements;
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
