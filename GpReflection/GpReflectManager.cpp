#include "GpReflectManager.hpp"
#include "GpReflectObject.hpp"

namespace GPlatform {

GpReflectManager&   GpReflectManager::sInstance = GpReflectManager::_S_();

GpReflectManager::GpReflectManager (void) noexcept
{
}

GpReflectManager::~GpReflectManager (void) noexcept
{
    iElements.Clear();
}

GpReflectManager&   GpReflectManager::_S_ (void) noexcept
{
    static GpReflectManager m;
    return m;
}

void    GpReflectManager::AddModelSource (GpReflectModelSource::SP aSource)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iModelSourcesSpinLock};

    iModelSources.emplace_back(std::move(aSource));
}

void    GpReflectManager::Register (GpReflectModel::CSP aModelCSP)
{
    const GpUUID& modelUid = aModelCSP.V().Uid();

    auto [model, isInserted] = iElements.TrySet(modelUid, std::move(aModelCSP));

    THROW_COND_GP
    (
        isInserted == true,
        [modelUid]()
        {
            return fmt::format
            (
                "Failed to register model with uid: {}",
                modelUid
            );
        }
    );
}

bool    GpReflectManager::TryRegister (GpReflectModel::CSP aModelCSP)
{
    const GpUUID& modelUid = aModelCSP.V().Uid();

    auto [model, isInserted] = iElements.TrySet(modelUid, std::move(aModelCSP));

    return isInserted;
}

GpReflectModel::CSP GpReflectManager::Find (const GpUUID& aModelUid)
{
    std::optional<GpReflectModel::CSP> modelOpt = iElements.GetOpt(aModelUid);

    if (modelOpt.has_value())
    {
        return modelOpt.value();
    } else
    {
        return FromSources(aModelUid);
    }
}

GpReflectModel::C::Opt::CSP GpReflectManager::FindOpt (const GpUUID& aModelUid) noexcept
{
    std::optional<GpReflectModel::CSP> modelOpt = iElements.GetOpt(aModelUid);

    if (modelOpt.has_value())
    {
        return modelOpt;
    } else
    {
        return FromSourcesOpt(aModelUid);
    }
}

bool    GpReflectManager::IsBaseOf
(
    const GpUUID& aBaseModelUid,
    const GpUUID& aDerivedModelUid
)
{
    if (aDerivedModelUid == aBaseModelUid)
    {
        return true;
    }

    const GpUUID    lastModelUid    = GpReflectObject::SReflectModelUid();
    GpUUID          derivedModelUid = aDerivedModelUid;

    while (derivedModelUid != lastModelUid)
    {
        GpReflectModel::CSP modelCSP = Find(derivedModelUid);

        derivedModelUid = modelCSP.Vn().BaseUid();

        if (derivedModelUid == aBaseModelUid)
        {
            return true;
        }
    }

    return false;
}

bool    GpReflectManager::IsBaseOfNoEx
(
    const GpUUID& aBaseModelUid,
    const GpUUID& aDerivedModelUid
) noexcept
{
    if (aDerivedModelUid == aBaseModelUid)
    {
        return true;
    }

    const GpUUID    lastModelUid    = GpReflectObject::SReflectModelUid();
    GpUUID          derivedModelUid = aDerivedModelUid;

    while (derivedModelUid != lastModelUid)
    {
        GpReflectModel::C::Opt::CSP modelOpt = FindOpt(derivedModelUid);

        if (!modelOpt.has_value()) [[unlikely]]
        {
            return false;
        }

        derivedModelUid = modelOpt.value().Vn().BaseUid();

        if (derivedModelUid == aBaseModelUid)
        {
            return true;
        }
    }

    return false;
}

GpReflectModel::C::Opt::CRef    GpReflectManager::SelectBaseModel
(
    const GpReflectModel& aModelA,
    const GpReflectModel& aModelB
)
{
    if (IsBaseOf(aModelA.Uid(), aModelB.Uid()))
    {
        return aModelA;
    } else if (IsBaseOf(aModelB.Uid(), aModelA.Uid()))
    {
        return aModelB;
    } else
    {
        return std::nullopt;
    }
}

GpUUID::C::Opt::Val GpReflectManager::SelectBaseModel
(
    const GpUUID& aModelUidA,
    const GpUUID& aModelUidB
)
{
    if (IsBaseOf(aModelUidA, aModelUidB))
    {
        return aModelUidA;
    } else if (IsBaseOf(aModelUidB, aModelUidA))
    {
        return aModelUidB;
    } else
    {
        return std::nullopt;
    }
}

GpReflectModel::CSP GpReflectManager::FromSources (const GpUUID& aModelUid)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iModelSourcesSpinLock};

    for (GpReflectModelSource::SP& source: iModelSources)
    {
        GpReflectModel::C::Opt::CSP modelOpt = source.V().Get(aModelUid);

        if (modelOpt.has_value())
        {
            GpReflectModel::CSP& modelCSP = modelOpt.value();
            Register(modelCSP);
            return modelCSP;
        }
    }

    THROW_GP
    (
        fmt::format
        (
            "Reflection model was not found by UID '{}'",
            aModelUid
        )
    );
}

GpReflectModel::C::Opt::CSP GpReflectManager::FromSourcesOpt (const GpUUID& aModelUid)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iModelSourcesSpinLock};

    for (GpReflectModelSource::SP& source: iModelSources)
    {
        GpReflectModel::C::Opt::CSP modelOpt = source.V().Get(aModelUid);

        if (modelOpt.has_value())
        {
            GpReflectModel::CSP& modelCSP = modelOpt.value();
            Register(modelCSP);
            return modelCSP;
        }
    }

    return std::nullopt;
}

}// namespace GPlatform
