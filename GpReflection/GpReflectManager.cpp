#include "GpReflectManager.hpp"

#if defined(GP_USE_REFLECTION)

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
    std::scoped_lock lock(iModelSourcesMutex);
    iModelSources.emplace_back(std::move(aSource));
}

const GpReflectModel&   GpReflectManager::Register (const GpReflectModel& aModel)
{
    const GpUUID&           modelUid = aModel.Uid();
    GpReflectModel::CSP     modelCSP = MakeCSP<GpReflectModel>(aModel);
    const GpReflectModel&   modelRef = modelCSP.Vn();

    iElements.Set(modelUid, std::move(modelCSP));

    return modelRef;
}

const GpReflectModel&   GpReflectManager::Find (const GpUUID& aModelUid)
{
    auto res = iElements.GetOpt(aModelUid);

    if (res.has_value())
    {
        return res.value().get().Vn();
    } else
    {
        return FromSources(aModelUid);
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
        const GpReflectModel& model = Find(derivedModelUid);

        derivedModelUid = model.BaseUid();

        if (derivedModelUid == aBaseModelUid)
        {
            return true;
        }
    }

    return false;
}

const GpReflectModel&   GpReflectManager::FromSources (const GpUUID& aModelUid)
{
    std::scoped_lock lock(iModelSourcesMutex);

    for (GpReflectModelSource::SP& source: iModelSources)
    {
        GpReflectModel::C::Opt::Val rOpt = source.V().Get(aModelUid);

        if (rOpt.has_value())
        {
            return Register(rOpt.value());
        }
    }

    THROW_GP(u8"Reflection model was not found by UID '"_sv + aModelUid + u8"'");
}

}//GPlatform

#endif//GP_USE_REFLECTION
