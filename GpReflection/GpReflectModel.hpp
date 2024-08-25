#pragma once

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/Config/GpConfig.hpp>

#include "GpReflectProp.hpp"
#include "GpReflectObjectFactory.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModel
{
public:
    CLASS_DD(GpReflectModel)

    using PropT             = GpReflectProp;
    using PropsT            = GpReflectProp::SmallVecVal;
    using PropsNameToIdxT   = boost::container::small_flat_map<std::string/*name*/, size_t/*index in */, 24, std::less<>>;

public:
                                    GpReflectModel          (void) noexcept = default;
                                    GpReflectModel          (const GpUUID&              aUid,
                                                             const GpUUID&              aBaseUid,
                                                             std::string&&              aName,
                                                             PropsT&&                   aProps,
                                                             const GpUUID&              aGroupId,
                                                             GpReflectObjectFactory::SP aFactory,
                                                             size_t                     aAlign,
                                                             size_t                     aSize) noexcept;
                                    GpReflectModel          (const GpUUID&              aUid,
                                                             const GpUUID&              aBaseUid,
                                                             std::string&&              aName,
                                                             const GpUUID&              aGroupId,
                                                             GpReflectObjectFactory::SP aFactory) noexcept;
                                    GpReflectModel          (const GpReflectModel& aModel);
                                    GpReflectModel          (GpReflectModel&& aModel) noexcept;
                                    ~GpReflectModel         (void) noexcept;

    GpReflectModel&                 operator=               (const GpReflectModel& aModel);
    GpReflectModel&                 operator=               (GpReflectModel&& aModel) noexcept;

    const GpUUID&                   Uid                     (void) const noexcept {return iUid;}
    const GpUUID&                   BaseUid                 (void) const noexcept {return iBaseUid;}
    std::string_view                Name                    (void) const noexcept {return iName;}
    const PropsT&                   Props                   (void) const noexcept {return iProps;}
    const PropsNameToIdxT&          PropsNameToIdx          (void) const noexcept {return iPropsNameToIdx;}
    const GpReflectProp&            Prop                    (const size_t aId) const {return iProps.at(aId);}
    const GpReflectProp&            Prop                    (std::string_view aName) const;
    GpReflectProp::C::Opt::CRef     PropOpt                 (std::string_view aName) const noexcept;
    const GpReflectProp&            Prop                    (std::string_view                       aName,
                                                             const GpReflectType::EnumT             aType,
                                                             const GpReflectContainerType::EnumT    aContainerType) const;
    GpReflectProp::SmallVecCRef     PropsWithoutBaseProps   (void) const;

    const GpUUID&                   GroupId                 (void) const noexcept {return iGroupId;}
    size_t                          Align                   (void) const noexcept {return iAlign;}
    size_t                          Size                    (void) const noexcept {return iSize;}

    const GpReflectObjectFactory&   Factory                 (void) const noexcept {return iFactory.Vn();}
    GpSP<GpReflectObject>           NewInstance             (void) const {return iFactory.Vn().NewInstanceSP(iUid);}
    void                            ConstructInplace        (void* aDataPtr) const {iFactory.Vn().ConstructInplace(aDataPtr);}
    void                            DestructInplace         (void* aDataPtr) const {iFactory.Vn().DestructInplace(aDataPtr);}

private:
    void                            UpdatePropsNameToIdx    (void);

private:
    GpUUID                          iUid;
    GpUUID                          iBaseUid;
    std::string                     iName;
    PropsT                          iProps;
    PropsNameToIdxT                 iPropsNameToIdx;
    GpUUID                          iGroupId;
    GpReflectObjectFactory::SP      iFactory;
    size_t                          iAlign      = 0;
    size_t                          iSize       = 0;
};

}// namespace GPlatform
