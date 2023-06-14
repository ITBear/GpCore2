#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include "GpReferenceCounter.hpp"

namespace GPlatform {

template <typename  T>
class GpReferenceStorage final: public GpReferenceCounter
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReferenceStorage)

public:
    using this_type     = GpReferenceStorage<T>;
    using value_type    = T;

public:
    template<typename... Ts>
    explicit        GpReferenceStorage  (Ts&&... aArgs):
                    GpReferenceCounter(&iValue),
                    iValue(std::forward<Ts>(aArgs)...)
                    {
                    }

    virtual         ~GpReferenceStorage (void) noexcept override final = default;

private:
    T               iValue;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_SHARED_POINTERS)
