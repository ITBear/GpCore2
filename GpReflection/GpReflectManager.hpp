#pragma once

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpDictionary.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpReflection/GpReflectModelSource.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectManager
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectManager)
    CLASS_DD(GpReflectManager)
    TAG_SET(THREAD_SAFE)

    using ElementsT = GpDictionary<boost::container::flat_map<GpUUID, GpReflectModel::CSP>>;

public:
                                    GpReflectManager    (void) noexcept;
                                    ~GpReflectManager   (void) noexcept;

    static GpReflectManager&        S                   (void) noexcept {return sInstance;}
    static GpReflectManager&        _S_                 (void) noexcept;

    void                            AddModelSource      (GpReflectModelSource::SP aSource);

    void                            Register            (GpReflectModel::CSP aModelCSP);
    [[nodiscard]] bool              TryRegister         (GpReflectModel::CSP aModelCSP);
    GpReflectModel::CSP             Find                (const GpUUID& aModelUid);
    GpReflectModel::C::Opt::CSP     FindOpt             (const GpUUID& aModelUid) noexcept;

    bool                            IsBaseOf            (const GpUUID& aBaseModelUid,
                                                         const GpUUID& aDerivedModelUid);
    bool                            IsBaseOfNoEx        (const GpUUID& aBaseModelUid,
                                                         const GpUUID& aDerivedModelUid) noexcept;
    GpReflectModel::C::Opt::CRef    SelectBaseModel     (const GpReflectModel&  aModelA,
                                                         const GpReflectModel&  aModelB);
    GpUUID::C::Opt::Val             SelectBaseModel     (const GpUUID& aModelUidA,
                                                         const GpUUID& aModelUidB);

    template<typename TO_SP, typename FROM_SP>
    [[nodiscard]] TO_SP             CastSP              (FROM_SP& aFrom);

    template<typename TO_SP, typename FROM_SP> [[nodiscard]] static
    TO_SP                           SCastSP             (FROM_SP& aFrom) {return S().CastSP<TO_SP, FROM_SP>(aFrom);}

    template<typename TO, typename FROM>
    [[nodiscard]] TO&               CastRef             (FROM& aFrom);

    template<typename TO, typename FROM>
    [[nodiscard]] std::optional<std::reference_wrapper<TO>>
                                    CastRefOpt          (FROM& aFrom) noexcept;

    template<typename TO, typename FROM>
    [[nodiscard]] const TO&         CastCRef            (const FROM& aFrom);

    template<typename TO, typename FROM>
    [[nodiscard]] std::optional<std::reference_wrapper<const TO>>
                                    CastCRefOpt         (FROM& aFrom) noexcept;

    template<typename TO, typename FROM> [[nodiscard]] static
    TO&                             SCastRef            (FROM& aFrom) {return S().CastRef<TO, FROM>(aFrom);}

    template<typename TO, typename FROM> [[nodiscard]] static
    const TO&                       SCastCRef           (const FROM& aFrom) {return S().CastCRef<TO, FROM>(aFrom);}

private:
    GpReflectModel::CSP             FromSources         (const GpUUID& aModelUid);
    GpReflectModel::C::Opt::CSP     FromSourcesOpt      (const GpUUID& aModelUid);

private:
    ElementsT                           iElements;

    mutable GpSpinLock                  iModelSourcesSpinLock;
    GpReflectModelSource::C::Vec::SP    iModelSources       GUARDED_BY(iModelSourcesSpinLock);

    static GpReflectManager&            sInstance;
};

template<typename TO_SP, typename FROM_SP>
[[nodiscard]] TO_SP GpReflectManager::CastSP (FROM_SP& aFrom)
{
    using TO_VAL_T = typename TO_SP::value_type;

    GpReflectModel::CSP     fromModelCSP    = aFrom->ReflectModel();
    const GpReflectModel&   fromModel       = fromModelCSP.Vn();
    GpReflectModel::CSP     toModelCSP      = TO_VAL_T::SReflectModel();
    const GpReflectModel&   toModel         = toModelCSP.Vn();

    THROW_COND_GP
    (
        IsBaseOf
        (
            toModel.Uid(),
            fromModel.Uid()
        ),
        [&]()
        {
            return fmt::format
            (
                "Failed to cast object with type UID '{}' to object with type UID '{}'",
                fromModel.Uid(),
                toModel.Uid()
            );
        }
    );

    return aFrom.template CastAs<TO_SP>();
}

template<typename TO, typename FROM>
[[nodiscard]] TO&   GpReflectManager::CastRef (FROM& aFrom)
{
    using TO_VAL_T = std::remove_cv_t<TO>;

    GpReflectModel::CSP     fromModelCSP    = aFrom.ReflectModel();
    const GpReflectModel&   fromModel       = fromModelCSP.Vn();
    GpReflectModel::CSP     toModelCSP      = TO_VAL_T::SReflectModel();
    const GpReflectModel&   toModel         = toModelCSP.Vn();

    THROW_COND_GP
    (
        IsBaseOf
        (
            toModel.Uid(),
            fromModel.Uid()
        ),
        [&]()
        {
            return fmt::format
            (
                "Failed to cast object with type UID '{}' to object with type UID '{}'",
                fromModel.Uid(),
                toModel.Uid()
            );
        }
    );

    return static_cast<TO&>(aFrom);
}

template<typename TO, typename FROM>
[[nodiscard]] std::optional<std::reference_wrapper<TO>> GpReflectManager::CastRefOpt (FROM& aFrom) noexcept
{
    using TO_VAL_T = std::remove_cv_t<TO>;


    GpReflectModel::CSP     fromModelCSP    = aFrom.ReflectModel();
    const GpReflectModel&   fromModel       = fromModelCSP.Vn();
    GpReflectModel::CSP     toModelCSP      = TO_VAL_T::SReflectModel();
    const GpReflectModel&   toModel         = toModelCSP.Vn();

    const bool isBase = IsBaseOfNoEx
    (
        toModel.Uid(),
        fromModel.Uid()
    );

    if (!isBase) [[unlikely]]
    {
        return std::nullopt;
    }

    return static_cast<TO&>(aFrom);
}

template<typename TO, typename FROM>
[[nodiscard]] const TO& GpReflectManager::CastCRef (const FROM& aFrom)
{
    using TO_VAL_T = std::remove_cv_t<TO>;

    GpReflectModel::CSP     fromModelCSP    = aFrom.ReflectModel();
    const GpReflectModel&   fromModel       = fromModelCSP.Vn();
    GpReflectModel::CSP     toModelCSP      = TO_VAL_T::SReflectModel();
    const GpReflectModel&   toModel         = toModelCSP.Vn();

    THROW_COND_GP
    (
        IsBaseOf
        (
            toModel.Uid(),
            fromModel.Uid()
        ),
        [&]()
        {
            return fmt::format
            (
                "Failed to cast object with type UID '{}' to object with type UID '{}'",
                fromModel.Uid(),
                toModel.Uid()
            );
        }
    );

    return static_cast<const TO&>(aFrom);
}

template<typename TO, typename FROM>
[[nodiscard]] std::optional<std::reference_wrapper<const TO>>   GpReflectManager::CastCRefOpt (FROM& aFrom) noexcept
{
    using TO_VAL_T = std::remove_cv_t<TO>;

    GpReflectModel::CSP     fromModelCSP    = aFrom.ReflectModel();
    const GpReflectModel&   fromModel       = fromModelCSP.Vn();
    GpReflectModel::CSP     toModelCSP      = TO_VAL_T::SReflectModel();
    const GpReflectModel&   toModel         = toModelCSP.Vn();

    const bool isBase = IsBaseOfNoEx
    (
        toModel.Uid(),
        fromModel.Uid()
    );

    if (!isBase) [[unlikely]]
    {
        return std::nullopt;
    }

    return static_cast<const TO&>(aFrom);
}

}// namespace GPlatform
