#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpMemoryStorageViewR.hpp"
#include "GpMemoryStorageViewRW.hpp"

namespace GPlatform {

class GPCORE_API GpMemoryStorage
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpMemoryStorage);
    CLASS_DECLARE_DEFAULTS(GpMemoryStorage);

public:
    using ViewRT    = GpMemoryStorageViewR;
    using ViewRWT   = GpMemoryStorageViewRW;

protected:
                                GpMemoryStorage     (void) noexcept = default;

public:
    virtual                     ~GpMemoryStorage    (void) noexcept = default;

    virtual void                Clear               (void) = 0;
    virtual void                Allocate            (const size_byte_t aSize) = 0;
    virtual void                Resize              (const size_byte_t aSize) = 0;
    virtual size_byte_t         Size                (void) const noexcept = 0;
    virtual bool                IsEmpty             (void) const noexcept = 0;
    virtual bool                IsViewing           (void) const noexcept = 0;

    virtual void                Set                 (GpMemoryStorage&& aStorage) = 0;
    virtual void                Set                 (std::string_view aData) = 0;
    virtual void                Set                 (GpRawPtrByteR aData) = 0;
    virtual void                Set                 (GpRawPtrByteRW aData) = 0;

    virtual ViewRT::SP          VR                  (void) const = 0;
    virtual ViewRWT::SP         VRW                 (void) = 0;

    //virtual GpMemoryStorage::SP   New                 (void) const = 0;
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
