#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

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
    GpUniqueLock uniqueLock{iModelSourcesSpinLock};

    iModelSources.emplace_back(std::move(aSource));
}

void    GpReflectManager::Register (GpReflectModel::CSP aModelCSP)
{
    const GpUUID& modelUid = aModelCSP.V().Uid();

    auto [model, status] = iElements.FindOrSet(modelUid, std::move(aModelCSP));

    VERIFY
    (
        status == GpContainerUpdateStatus::INSERT_NEW,
        [modelUid, &model]()
        {
            return fmt::format
            (
                "Failed to register the model with UID: {}. Another model with this UID is already registered, model name '{}'",
                modelUid,
                model.V().Name()
            );
        }
    );
}

bool    GpReflectManager::TryRegister (GpReflectModel::CSP aModelCSP)
{
    const GpUUID& modelUid = aModelCSP.V().Uid();

    auto [model, status] = iElements.FindOrSet(modelUid, std::move(aModelCSP));

    return status == GpContainerUpdateStatus::INSERT_NEW;
}

GpReflectModel::CSP GpReflectManager::Find (const GpUUID& aModelUid)
{
    std::optional<GpReflectModel::CSP> modelOpt = iElements.FindOpt(aModelUid);

    if (modelOpt.has_value())
    {
        return modelOpt.value();
    } else
    {
        return FromSources(aModelUid);
    }
}

GpReflectModel::C::Opts::CSP    GpReflectManager::FindOpt (const GpUUID& aModelUid) noexcept
{
    std::optional<GpReflectModel::CSP> modelOpt = iElements.FindOpt(aModelUid);

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
        GpReflectModel::C::Opts::CSP modelOpt = FindOpt(derivedModelUid);

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

GpReflectModel::C::Opts::CRef   GpReflectManager::SelectBaseModel
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

GpUUID::C::Opts::Val    GpReflectManager::SelectBaseModel
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
    GpUniqueLock uniqueLock{iModelSourcesSpinLock};

    for (GpReflectModelSource::SP& source: iModelSources)
    {
        GpReflectModel::C::Opts::CSP modelOpt = source.V().Get(aModelUid);

        if (modelOpt.has_value())
        {
            GpReflectModel::CSP& modelCSP = modelOpt.value();
            Register(modelCSP);
            return modelCSP;
        }
    }

    THROW
    (
        fmt::format
        (
            "Reflection model with UID '{}' was not found",
            aModelUid
        )
    );
}

GpReflectModel::C::Opts::CSP    GpReflectManager::FromSourcesOpt (const GpUUID& aModelUid)
{
    GpUniqueLock uniqueLock{iModelSourcesSpinLock};

    for (GpReflectModelSource::SP& source: iModelSources)
    {
        GpReflectModel::C::Opts::CSP modelOpt = source.V().Get(aModelUid);

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
