#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectObjectBuilder
{
public:
                                    GpReflectObjectBuilder  (void) noexcept;
                                    ~GpReflectObjectBuilder (void) noexcept;

    GpReflectObject::SP             Build                   (void) {return iObject;}

    inline GpReflectObjectBuilder&  ModelUID                (const GpUUID&  aUid);

    template<typename... ArgsT>
    GpReflectObjectBuilder&         SetValuesAtOnce         (ArgsT&&... aArgs);

private:
    template<typename T>
    void                            SetValue                (void*                  aReflectObjPtr,
                                                             const GpReflectProp&   aReflectProp,
                                                             T&&                    aValue);

private:
    GpReflectObject::SP             iObject;
};

GpReflectObjectBuilder& GpReflectObjectBuilder::ModelUID (const GpUUID& aUid)
{
    iObject = GpReflectManager::S().Find(aUid).Vn().NewInstance();

    return *this;
}

template<typename... ArgsT>
GpReflectObjectBuilder& GpReflectObjectBuilder::SetValuesAtOnce (ArgsT&&... aArgs)
{
    GpReflectModel::CSP modelCSP        = iObject->ReflectModel();
    void*               reflectObjPtr   = iObject->ReflectDataPtr();
    const auto&         props           = modelCSP.Vn().Props();

    size_t propId = 0;
    ((SetValue<ArgsT>(reflectObjPtr, props[propId++], std::forward<ArgsT>(aArgs))), ...);

    return *this;
}

template<typename T>
void    GpReflectObjectBuilder::SetValue
(
    void*                   aReflectObjPtr,
    const GpReflectProp&    aReflectProp,
    T&&                     aValue
)
{
    aReflectProp.Value<T>(aReflectObjPtr) = std::forward<T>(aValue);
}

}// namespace GPlatform
