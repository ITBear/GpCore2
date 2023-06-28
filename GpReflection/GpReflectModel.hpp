#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectProp.hpp"
#include "GpReflectObjectFactory.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModel
{
public:
    CLASS_DD(GpReflectModel)

    using PropT     = GpReflectProp;
    using PropsT    = PropT::C::Vec::Val;

public:
                                GpReflectModel  (void) noexcept = default;
    inline                      GpReflectModel  (const GpUUID&              aUid,
                                                 const GpUUID&              aBaseUid,
                                                 std::u8string&&            aName,
                                                 PropsT&&                   aProps,
                                                 const GpUUID&              aGroupId,
                                                 GpReflectObjectFactory::SP aFactory,
                                                 const size_t               aAlign,
                                                 const size_t               aSize) noexcept;
    inline                      GpReflectModel  (const GpUUID&              aUid,
                                                 const GpUUID&              aBaseUid,
                                                 std::u8string&&            aName,
                                                 const GpUUID&              aGroupId,
                                                 GpReflectObjectFactory::SP aFactory) noexcept;
    inline                      GpReflectModel  (const GpReflectModel& aModel);
    inline                      GpReflectModel  (GpReflectModel&& aModel) noexcept;
                                ~GpReflectModel (void) noexcept;

    const GpUUID&               Uid             (void) const noexcept {return iUid;}
    const GpUUID&               BaseUid         (void) const noexcept {return iBaseUid;}
    std::u8string_view          Name            (void) const noexcept {return iName;}
    const PropsT&               Props           (void) const noexcept {return iProps;}
    const GpReflectProp&        Prop            (const size_t aId) const {return iProps.at(aId);}
    const GpReflectProp&        Prop            (std::u8string_view aName) const;
    GpReflectProp::C::Opt::CRef PropOpt         (std::u8string_view aName) const noexcept;
    const GpReflectProp&        Prop            (std::u8string_view                     aName,
                                                 const GpReflectType::EnumT             aType,
                                                 const GpReflectContainerType::EnumT    aContainerType) const;
    const GpUUID&               GroupId         (void) const noexcept {return iGroupId;}
    size_t                      Align           (void) const noexcept {return iAlign;}
    size_t                      Size            (void) const noexcept {return iSize;}

    GpReflectObjectFactory::SP  Factory         (void) const noexcept {return iFactory;}
    GpSP<GpReflectObject>       NewInstance     (void) const {return iFactory.Vn().NewInstance(iUid);}
    void                        Construct       (void* aDataPtr) const {iFactory.Vn().Construct(aDataPtr);}
    void                        Destruct        (void* aDataPtr) const {iFactory.Vn().Destruct(aDataPtr);}

private:
    GpUUID                      iUid;
    GpUUID                      iBaseUid;
    std::u8string               iName;
    PropsT                      iProps;
    GpUUID                      iGroupId;
    GpReflectObjectFactory::SP  iFactory;
    size_t                      iAlign = 0;
    size_t                      iSize = 0;
};

GpReflectModel::GpReflectModel
(
    const GpUUID&               aUid,
    const GpUUID&               aBaseUid,
    std::u8string&&             aName,
    PropsT&&                    aProps,
    const GpUUID&               aGroupId,
    GpReflectObjectFactory::SP  aFactory,
    const size_t                aAlign,
    const size_t                aSize
) noexcept:
iUid(aUid),
iBaseUid(aBaseUid),
iName(std::move(aName)),
iProps(std::move(aProps)),
iGroupId(aGroupId),
iFactory(std::move(aFactory)),
iAlign(aAlign),
iSize(aSize)
{
}

GpReflectModel::GpReflectModel
(
    const GpUUID&               aUid,
    const GpUUID&               aBaseUid,
    std::u8string&&             aName,
    const GpUUID&               aGroupId,
    GpReflectObjectFactory::SP  aFactory
) noexcept:
iUid    (aUid),
iBaseUid(aBaseUid),
iName   (std::move(aName)),
iGroupId(aGroupId),
iFactory(std::move(aFactory)),
iAlign  (1),
iSize   (0)
{
}

GpReflectModel::GpReflectModel (const GpReflectModel& aModel):
iUid    (aModel.iUid),
iBaseUid(aModel.iBaseUid),
iName   (aModel.iName),
iProps  (aModel.iProps),
iGroupId(aModel.iGroupId),
iFactory(aModel.iFactory),
iAlign  (aModel.iAlign),
iSize   (aModel.iSize)
{
}

GpReflectModel::GpReflectModel (GpReflectModel&& aModel) noexcept:
iUid    (std::move(aModel.iUid)),
iBaseUid(std::move(aModel.iBaseUid)),
iName   (std::move(aModel.iName)),
iProps  (std::move(aModel.iProps)),
iGroupId(std::move(aModel.iGroupId)),
iFactory(std::move(aModel.iFactory)),
iAlign  (std::move(aModel.iAlign)),
iSize   (std::move(aModel.iSize))
{
}

}//GPlatform

#endif//GP_USE_REFLECTION
