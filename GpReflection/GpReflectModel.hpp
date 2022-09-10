#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectProp.hpp"
#include "GpReflectObjectFactory.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModel
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectModel)

    using PropT     = GpReflectProp;
    using PropsT    = PropT::C::Vec::Val;

public:
                                GpReflectModel  (void) noexcept {}
                                GpReflectModel  (const GpUUID&              aUid,
                                                 const GpUUID&              aBaseUid,
                                                 std::string&&              aName,
                                                 PropsT&&                   aProps,
                                                 const GpUUID&              aGroupId,
                                                 GpReflectObjectFactory::SP aFactory,
                                                 const size_t               aAlign,
                                                 const size_t               aSize) noexcept;
                                GpReflectModel  (const GpUUID&              aUid,
                                                 const GpUUID&              aBaseUid,
                                                 std::string&&              aName,
                                                 const GpUUID&              aGroupId,
                                                 GpReflectObjectFactory::SP aFactory) noexcept;
                                GpReflectModel  (const GpReflectModel& aModel);
                                GpReflectModel  (GpReflectModel&& aModel) noexcept;
                                ~GpReflectModel (void) noexcept;

    const GpUUID&               Uid             (void) const noexcept {return iUid;}
    const GpUUID&               BaseUid         (void) const noexcept {return iBaseUid;}
    std::string_view            Name            (void) const noexcept {return iName;}
    const PropsT&               Props           (void) const noexcept {return iProps;}
    const GpReflectProp&        Prop            (const size_t aId) const {return iProps.at(aId);}
    const GpReflectProp&        Prop            (std::string_view aName) const;
    const GpReflectProp&        Prop            (std::string_view                       aName,
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
    size_t                      CalcSizeOfProps (void) const;

private:
    GpUUID                      iUid;
    GpUUID                      iBaseUid;
    std::string                 iName;
    PropsT                      iProps;
    GpUUID                      iGroupId;
    GpReflectObjectFactory::SP  iFactory;
    size_t                      iAlign = 0;
    size_t                      iSize = 0;
};

}//GPlatform

#endif//GP_USE_REFLECTION
