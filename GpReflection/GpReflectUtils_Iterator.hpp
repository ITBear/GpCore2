#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectUtils_Iterator
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectUtils_Iterator)

    using CheckFnT          = std::function<bool(const GpReflectModel&, const GpReflectProp&)>;
    using ApplyFnT          = std::function<void(const GpReflectProp&, void* aPropPtr)>;
    using IteratorStackT    = std::stack<std::tuple<GpReflectModel::CSP, void* /*reflect data ptr*/ >>;

public:
    static void     SIterateAndApplyIf  (GpReflectObject&   aObject,
                                         const CheckFnT&&   aCheckFn,
                                         const ApplyFnT&&   aApplyFn);

private:
    static void     SProcessObject      (const GpReflectProp&   aProp,
                                         void*                  aReflectDataPtr,
                                         IteratorStackT&        aIteratorStack);
    static void     SProcessObjectSP    (const GpReflectProp&   aProp,
                                         void*                  aReflectDataPtr,
                                         IteratorStackT&        aIteratorStack);

};

}// namespace GPlatform
