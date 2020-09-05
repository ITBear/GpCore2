#include "GpTypeStructInfo.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

GpTypeStructInfo::GpTypeStructInfo (const GpUUID&           aUID,
                                    const GpUUID&           aBaseUID,
                                    std::string&&           aName,
                                    PropsT&&                aProps,
                                    const GpUUID&           aGroupID,
                                    GpTypeStructFactory::SP aFactory) noexcept:
iUID(aUID),
iBaseUID(aBaseUID),
iName(std::move(aName)),
iProps(std::move(aProps)),
iGroupID(aGroupID),
iFactory(std::move(aFactory))
{
}

GpTypeStructInfo::GpTypeStructInfo (const GpTypeStructInfo& aTypeInfo):
iUID(aTypeInfo.iUID),
iBaseUID(aTypeInfo.iBaseUID),
iName(aTypeInfo.iName),
iProps(aTypeInfo.iProps),
iGroupID(aTypeInfo.iGroupID),
iFactory(aTypeInfo.iFactory)
{
}

GpTypeStructInfo::GpTypeStructInfo (GpTypeStructInfo&& aTypeInfo) noexcept:
iUID(std::move(aTypeInfo.iUID)),
iBaseUID(std::move(aTypeInfo.iBaseUID)),
iName(std::move(aTypeInfo.iName)),
iProps(std::move(aTypeInfo.iProps)),
iGroupID(std::move(aTypeInfo.iGroupID)),
iFactory(std::move(aTypeInfo.iFactory))
{
}

GpTypeStructInfo::~GpTypeStructInfo (void) noexcept
{
}

GpTypeStructInfo&   GpTypeStructInfo::operator= (const GpTypeStructInfo& aTypeInfo)
{
    iUID        = aTypeInfo.iUID;
    iBaseUID    = aTypeInfo.iBaseUID;
    iName       = aTypeInfo.iName;
    iProps      = aTypeInfo.iProps;
    iFactory    = aTypeInfo.iFactory;
    iGroupID    = aTypeInfo.iGroupID;

    return *this;
}

GpTypeStructInfo&   GpTypeStructInfo::operator= (GpTypeStructInfo&& aTypeInfo) noexcept
{
    iUID        = std::move(aTypeInfo.iUID);
    iBaseUID    = std::move(aTypeInfo.iBaseUID);
    iName       = std::move(aTypeInfo.iName);
    iProps      = std::move(aTypeInfo.iProps);
    iFactory    = std::move(aTypeInfo.iFactory);
    iGroupID    = std::move(aTypeInfo.iGroupID);

    return *this;
}

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
