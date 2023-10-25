#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpUtils/Macro/GpMacroClass.hpp"
#include "../GpUtils/Types/Containers/GpContainersT.hpp"

namespace GPlatform {

class GpReflectObject;

class GpReflectObjectFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectObjectFactory)
    using SP = GpSP<GpReflectObjectFactory>;

    struct VecWrapInfoT
    {
        size_t  _align_of   = 0;
        size_t  _size_of    = 0;
    };

protected:
                                    GpReflectObjectFactory  (void) noexcept = default;

public:
    virtual                         ~GpReflectObjectFactory (void) noexcept = default;

    virtual GpSP<GpReflectObject>   NewInstanceSP           (const GpUUID& aModelUid) const = 0;
    virtual void                    ConstructInplace        (void* aDataPtr) const = 0;
    virtual void                    DestructInplace         (void* aDataPtr) const = 0;
    virtual const VecWrapInfoT&     VecWrapInfo             (void) const noexcept = 0;
};

}//GPlatform

#endif//GP_USE_REFLECTION
