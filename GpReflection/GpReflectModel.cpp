#include <GpCore2/GpReflection/GpReflectModel.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

GpReflectModel::GpReflectModel
(
    const GpUUID&               aUid,
    const GpUUID&               aBaseUid,
    std::string&&               aName,
    PropsT&&                    aProps,
    const GpUUID&               aGroupId,
    GpReflectObjectFactory::SP  aFactory,
    const size_t                aAlign,
    const size_t                aSize
) noexcept:
iUid    (aUid),
iBaseUid(aBaseUid),
iName   (std::move(aName)),
iProps  (std::move(aProps)),
iGroupId(aGroupId),
iFactory(std::move(aFactory)),
iAlign  (aAlign),
iSize   (aSize)
{
    UpdatePropsNameToIdx();
}

GpReflectModel::GpReflectModel
(
    const GpUUID&               aUid,
    const GpUUID&               aBaseUid,
    std::string&&               aName,
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
    UpdatePropsNameToIdx();
}

GpReflectModel::GpReflectModel (const GpReflectModel& aModel):
iUid           (aModel.iUid),
iBaseUid       (aModel.iBaseUid),
iName          (aModel.iName),
iProps         (aModel.iProps),
iPropsNameToIdx(aModel.iPropsNameToIdx),
iGroupId       (aModel.iGroupId),
iFactory       (aModel.iFactory),
iAlign         (aModel.iAlign),
iSize          (aModel.iSize)
{
}

GpReflectModel::GpReflectModel (GpReflectModel&& aModel) noexcept:
iUid           (std::move(aModel.iUid)),
iBaseUid       (std::move(aModel.iBaseUid)),
iName          (std::move(aModel.iName)),
iProps         (std::move(aModel.iProps)),
iPropsNameToIdx(std::move(aModel.iPropsNameToIdx)),
iGroupId       (std::move(aModel.iGroupId)),
iFactory       (std::move(aModel.iFactory)),
iAlign         (std::move(aModel.iAlign)),
iSize          (std::move(aModel.iSize))
{
}

GpReflectModel& GpReflectModel::operator= (const GpReflectModel& aModel)
{
    iUid            = aModel.iUid;
    iBaseUid        = aModel.iBaseUid;
    iName           = aModel.iName;
    iProps          = aModel.iProps;
    iPropsNameToIdx = aModel.iPropsNameToIdx;
    iGroupId        = aModel.iGroupId;
    iFactory        = aModel.iFactory;
    iAlign          = aModel.iAlign;
    iSize           = aModel.iSize;

    return *this;
}

GpReflectModel& GpReflectModel::operator= (GpReflectModel&& aModel) noexcept
{
    iUid            = std::move(aModel.iUid);
    iBaseUid        = std::move(aModel.iBaseUid);
    iName           = std::move(aModel.iName);
    iProps          = std::move(aModel.iProps);
    iPropsNameToIdx = std::move(aModel.iPropsNameToIdx);
    iGroupId        = std::move(aModel.iGroupId);
    iFactory        = std::move(aModel.iFactory);
    iAlign          = std::move(aModel.iAlign);
    iSize           = std::move(aModel.iSize);

    return *this;
}

GpReflectModel::~GpReflectModel (void) noexcept
{
}

const GpReflectProp&    GpReflectModel::Prop (std::string_view aName) const
{
    const auto iter = iPropsNameToIdx.find(aName);

    THROW_COND_GP
    (
        iter != std::end(iPropsNameToIdx),
        [aName]()
        {
            return fmt::format
            (
                "Property was not found by name '{}'",
                aName
            );
        }
    );

    const size_t idx = iter->second;
    return iProps.at(idx);
}

GpReflectProp::C::Opt::CRef GpReflectModel::PropOpt (std::string_view aName) const noexcept
{
    const auto iter = iPropsNameToIdx.find(aName);

    if (iter == std::end(iPropsNameToIdx)) [[unlikely]]
    {
        return std::nullopt;
    }

    const size_t idx = iter->second;
    return iProps[idx];
}

const GpReflectProp&    GpReflectModel::Prop
(
    std::string_view                    aName,
    const GpReflectType::EnumT          aType,
    const GpReflectContainerType::EnumT aContainerType
) const
{
    const GpReflectProp& prop = Prop(aName);

    THROW_COND_GP
    (
        prop.Type() == aType,
        [&]()
        {
            return fmt::format
            (
                "Property '{}'::'{}' have other type",
                Name(),
                aName
            );
        }
    );

    THROW_COND_GP
    (
        prop.Container() == aContainerType,
        [&]()
        {
            return fmt::format
            (
                "Property '{}'::'{}' have other container type",
                Name(),
                aName
            );
        }
    );

    return prop;
}

GpReflectProp::SmallVecCRef GpReflectModel::PropsWithoutBaseProps (void) const
{
    if (iProps.empty())
    {
        return {};
    }

    size_t startPropIdx = 0;

    if (iBaseUid.IsNotZero())
    {
        GpReflectModel::CSP     baseModelCSP    = GpReflectManager::S().Find(iBaseUid);
        const auto&             baseProps       = baseModelCSP.Vn().Props();

        if (!baseProps.empty())
        {
            startPropIdx = baseProps.at(std::size(baseProps) - 1).IdxInProps() + 1;
        }
    }

    GpReflectProp::SmallVecCRef resProps;
    resProps.reserve(std::size(iProps) - startPropIdx);
    const size_t propsCount = std::size(iProps);

    for (size_t id = startPropIdx; id < propsCount; id++)
    {
        resProps.emplace_back(iProps[id]);
    }

    return resProps;
}

void    GpReflectModel::UpdatePropsNameToIdx (void)
{
    size_t idx = 0;
    for (const GpReflectProp& prop: iProps)
    {
        iPropsNameToIdx[std::string(prop.Name())] = idx;
        idx++;
    }
}

}// namespace GPlatform
