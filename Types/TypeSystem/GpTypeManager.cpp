#include "GpTypeManager.hpp"
#include <iostream>

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

GpTypeManager::GpTypeManager (void) noexcept
{
}

GpTypeManager::~GpTypeManager (void) noexcept
{
    iElements.Clear();
}

GpTypeManager&  GpTypeManager::S (void) noexcept
{
    static GpTypeManager m;
    return m;
}

size_t  GpTypeManager::Register (const GpTypeStructInfo& aTypeInfo)
{
    //std::string s = aTypeInfo.UID().ToString();
    //printf("[GpTypeManager::Register]: aTypeInfo.UID() = ");
    //printf(s.data());
    //printf("\n");

    iElements.Register(aTypeInfo.UID(), aTypeInfo);

    return 0;
}

void    GpTypeManager::Unregister (const GpUUID& aTypeUID)
{
    iElements.Unregister(aTypeUID);
}

GpTypeStructInfo::C::Opt::CRef  GpTypeManager::Find (const GpUUID& aTypeUID) const noexcept
{
    return iElements.Find(aTypeUID);
}

bool    GpTypeManager::IsBaseOf (const GpUUID& aBaseTypeUID, const GpUUID& aDerivedTypeUID) const noexcept
{
    if (   aBaseTypeUID.IsZero()
        || aDerivedTypeUID.IsZero())
    {
        return false;
    }

    if (aDerivedTypeUID == aBaseTypeUID)
    {
        return true;
    }

    GpTypeStructInfo::C::Opt::CRef typeInfoOpt = Find(aDerivedTypeUID);

    while (typeInfoOpt.has_value())
    {
        const GpTypeStructInfo& typeInfo = typeInfoOpt.value();

        if (typeInfo.UID() == aBaseTypeUID)
        {
            return true;
        }

        typeInfoOpt = Find(typeInfo.BaseUID());
    }

    return false;
}

void    GpTypeManager::UnregisterGroup (const GpUUID& aGroupID)
{
    iElements.Process([&](ElementsT::container_type& aElements)
    {
        for (auto iter = aElements.begin(); iter != aElements.end(); )
        {
            if (iter->second.get().GroupID() == aGroupID)
            {
                iter = aElements.erase(iter);
            } else
            {
                iter++;
            }
        }
    });
}

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
