#include <GpCore2/GpReflection/GpReflectUtils_Iterator.hpp>

namespace GPlatform {

void    GpReflectUtils_Iterator::SIterateAndApplyIf
(
    GpReflectObject&    aObject,
    const CheckFnT&&    aCheckFn,
    const ApplyFnT&&    aApplyFn
)
{
    // Init iteratorStack
    std::stack<std::tuple<GpReflectModel::CSP, void*>> iteratorStack;
    iteratorStack.emplace
    (
        aObject.ReflectModel(),
        aObject.ReflectDataPtr()
    );

    // Iterate over aObject
    while (!iteratorStack.empty())
    {
        auto [modelCSP, reflectDataPtr] = iteratorStack.top();
        iteratorStack.pop();

        const GpReflectModel&               model   = modelCSP.Vn();
        const GpReflectProp::SmallVecVal&   props   = model.Props();

        // Iterate over props
        for (const GpReflectProp& prop: props)
        {
            // Check
            if (aCheckFn(model, prop))
            {
                // Apply
                aApplyFn(prop, reflectDataPtr);
            }

            // Check if prop is not null object
            const GpReflectType::EnumT propType = prop.Type();

            if (propType == GpReflectType::OBJECT)
            {
                SProcessObject(prop, reflectDataPtr, iteratorStack);
            } else if (propType == GpReflectType::OBJECT_SP)
            {
                SProcessObjectSP(prop, reflectDataPtr, iteratorStack);
            }
        }
    }
}

void    GpReflectUtils_Iterator::SProcessObject
(
    const GpReflectProp&    aProp,
    void*                   aReflectDataPtr,
    IteratorStackT&         aIteratorStack
)
{
    const GpReflectContainerType::EnumT propContainerType = aProp.Container();

    if (propContainerType == GpReflectContainerType::NO)
    {
        GpReflectObject& object = aProp.Value_Object(aReflectDataPtr);
        aIteratorStack.emplace
        (
            object.ReflectModel(),
            object.ReflectDataPtr()
        );
    } else if (propContainerType == GpReflectContainerType::VECTOR_WRAP)
    {
        GpVectorReflectObjWrapBase& vecWrap = aProp.VecWrap_Object(aReflectDataPtr);

        const size_t    size    = vecWrap.size();
        const size_t    stride  = vecWrap.stride();
        u_int8_t*       vecData = reinterpret_cast<u_int8_t*>(vecWrap.data());

        for (size_t id = 0; id < size; id++)
        {
            GpReflectObject& object = *reinterpret_cast<GpReflectObject*>(vecData);
            vecData += stride;

            aIteratorStack.emplace
            (
                object.ReflectModel(),
                object.ReflectDataPtr()
            );
        }
    } else
    {
        THROW_GP("Unsupported container type");
    }
}

void    GpReflectUtils_Iterator::SProcessObjectSP
(
    const GpReflectProp&    aProp,
    void*                   aReflectDataPtr,
    IteratorStackT&         aIteratorStack
)
{
    const GpReflectContainerType::EnumT propContainerType = aProp.Container();

    if (propContainerType == GpReflectContainerType::NO)
    {
        GpReflectObject::SP& objectSP = aProp.Value_ObjectSP(aReflectDataPtr);

        if (objectSP.IsNotNULL())
        {
            GpReflectObject& object = objectSP.Vn();

            aIteratorStack.emplace
            (
                object.ReflectModel(),
                object.ReflectDataPtr()
            );
        }
    } else if (propContainerType == GpReflectContainerType::VECTOR)
    {
        std::vector<GpReflectObject::SP>& vec = aProp.Vec_ObjectSP(aReflectDataPtr);

        for (GpReflectObject::SP& objectSP: vec)
        {
            if (objectSP.IsNotNULL())
            {
                GpReflectObject& object = objectSP.Vn();

                aIteratorStack.emplace
                (
                    object.ReflectModel(),
                    object.ReflectDataPtr()
                );
            }
        }
    } else if (propContainerType == GpReflectContainerType::MAP)
    {
        const GpReflectType::EnumT keyType = aProp.ContainerKeyType();

        const auto collectFn = []<typename T>(T& aMap, GpReflectUtils_Iterator::IteratorStackT& aIteratorStack)
        {
            for (auto&[key, val]: aMap)
            {
                if (auto* ptr = val.Pn())
                {
                    aIteratorStack.emplace
                    (
                        ptr->ReflectModel(),
                        ptr->ReflectDataPtr()
                    );
                }
            }
        };

        if (keyType == GpReflectType::U_INT_8)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::U_INT_8>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::S_INT_8)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::S_INT_8>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::U_INT_16)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::U_INT_16>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::S_INT_16)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::S_INT_16>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::U_INT_32)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::U_INT_32>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::S_INT_32)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::S_INT_32>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if  (keyType == GpReflectType::U_INT_64)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::U_INT_64>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::S_INT_64)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::S_INT_64>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::DOUBLE)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::DOUBLE>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::FLOAT)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::FLOAT>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::BOOLEAN)
        {
            THROW_GP("Booleans are not supported as map key");
        } else if (keyType == GpReflectType::UUID)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::UUID>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::STRING)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::STRING>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::BLOB)
        {
            auto& map = aProp.Map_ObjectSP<GpReflectUtils::Decltype<GpReflectType::BLOB>>(aReflectDataPtr);
            collectFn(map, aIteratorStack);
        } else if (keyType == GpReflectType::OBJECT)
        {
            THROW_GP("Objects are not supported as a map key");
        } else if (keyType == GpReflectType::OBJECT_SP)
        {
            THROW_GP("Objects SP are not supported as a map key");
        } else if (keyType == GpReflectType::ENUM)
        {
            THROW_GP("Enums are not supported as a map key");
        } else if (keyType == GpReflectType::ENUM_FLAGS)
        {
            THROW_GP("Enum flags are not supported as a map key");
        } else
        {
            THROW_GP(fmt::format("Unknown keyType '{}'", GpReflectType{keyType}));
        }
    } else
    {
        THROW_GP("Unsupported container keyType");
    }
}

}// namespace GPlatform
