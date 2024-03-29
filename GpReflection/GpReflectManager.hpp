#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpUtils/Types/Containers/GpDictionary.hpp"
#include "GpReflectModelSource.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectManager
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectManager)
    CLASS_DD(GpReflectManager)
    TAG_SET(THREAD_SAFE)

    using ElementsT = GpDictionary<GpUUID, GpReflectModel::CSP>;

public:
                                    GpReflectManager    (void) noexcept;
                                    ~GpReflectManager   (void) noexcept;

    static GpReflectManager&        S                   (void) noexcept {return sInstance;}
    static GpReflectManager&        _S_                 (void) noexcept;

    void                            AddModelSource      (GpReflectModelSource::SP aSource);

    const GpReflectModel&           Register            (const GpReflectModel& aModel);
    const GpReflectModel&           Find                (const GpUUID& aModelUid);
    bool                            IsBaseOf            (const GpUUID& aBaseModelUid,
                                                         const GpUUID& aDerivedModelUid);

    template<typename TO_SP, typename FROM_SP>
    [[nodiscard]] TO_SP             CastSP              (FROM_SP& aFrom);

    template<typename TO_SP, typename FROM_SP> [[nodiscard]] static
    TO_SP                           SCastSP             (FROM_SP& aFrom) {return S().CastSP<TO_SP, FROM_SP>(aFrom);}

    template<typename TO, typename FROM>
    [[nodiscard]] TO&               CastRef             (FROM& aFrom);

    template<typename TO, typename FROM>
    [[nodiscard]] const TO&         CastCRef            (const FROM& aFrom);

    template<typename TO, typename FROM> [[nodiscard]] static
    TO&                             SCastRef            (FROM& aFrom) {return S().CastRef<TO, FROM>(aFrom);}

    template<typename TO, typename FROM> [[nodiscard]] static
    const TO&                       SCastCRef           (const FROM& aFrom) {return S().CastCRef<TO, FROM>(aFrom);}

private:
    const GpReflectModel&           FromSources         (const GpUUID& aModelUid);

private:
    ElementsT                           iElements;

    std::mutex                          iModelSourcesMutex;
    GpReflectModelSource::C::Vec::SP    iModelSources;

    static GpReflectManager&            sInstance;
};

template<typename TO_SP, typename FROM_SP>
[[nodiscard]] TO_SP GpReflectManager::CastSP (FROM_SP& aFrom)
{
    using TO_VAL_T = typename TO_SP::value_type;

    const GpReflectModel& fromModel = aFrom.V().ReflectModel();
    const GpReflectModel& toModel   = TO_VAL_T::SReflectModel();

    THROW_COND_GP
    (
        IsBaseOf
        (
            toModel.Uid(),
            fromModel.Uid()
        ),
        [&]()
        {
            return
                u8"Failed to cast object with type UID "_sv + fromModel.Uid()
                + u8" to object with type UID "_sv + toModel.Uid();
        }
    );

    return aFrom.template CastAs<TO_SP>();
}

template<typename TO, typename FROM>
[[nodiscard]] TO&   GpReflectManager::CastRef (FROM& aFrom)
{
    using TO_VAL_T = std::remove_cv_t<TO>;

    const GpReflectModel& fromModel = aFrom.ReflectModel();
    const GpReflectModel& toModel   = TO_VAL_T::SReflectModel();

    THROW_COND_GP
    (
        IsBaseOf
        (
            toModel.Uid(),
            fromModel.Uid()
        ),
        [&]()
        {
            return
                u8"Failed to cast object with type UID "_sv + fromModel.Uid()
                + u8" to object with type UID "_sv + toModel.Uid();
        }
    );

    return static_cast<TO&>(aFrom);
}

template<typename TO, typename FROM>
[[nodiscard]] const TO& GpReflectManager::CastCRef (const FROM& aFrom)
{
    using TO_VAL_T = std::remove_cv_t<TO>;

    const GpReflectModel& fromModel = aFrom.ReflectModel();
    const GpReflectModel& toModel   = TO_VAL_T::SReflectModel();

    THROW_COND_GP
    (
        IsBaseOf
        (
            toModel.Uid(),
            fromModel.Uid()
        ),
        [&]()
        {
            return
                u8"Failed to cast object with type UID "_sv + fromModel.Uid()
                + u8" to object with type UID "_sv + toModel.Uid();
        }
    );

    return static_cast<const TO&>(aFrom);
}

}//GPlatform

#endif//GP_USE_REFLECTION
