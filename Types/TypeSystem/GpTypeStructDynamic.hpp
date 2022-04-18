#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructBase.hpp"

namespace GPlatform {

class GPCORE_API GpTypeStructDynamic final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeStructDynamic)

    using BaseT = GpTypeStructBase;

public:
    class Factory final: public GpTypeStructFactory
    {
    public:
        virtual GpTypeStructBase::SP    NewInstance     (const GpTypeStructInfo& aStructInfo) const override final;
        virtual void                    ConstructStruct (void* aStructDataPtr) const override final;
        virtual void                    DestructStruct  (void* aStructDataPtr) const override final;
    };

public:
                                    GpTypeStructDynamic     (const GpTypeStructInfo&    aTypeInfo,
                                                             void*                      aData) noexcept;
                                    GpTypeStructDynamic     (void) noexcept = delete;
                                    GpTypeStructDynamic     (const GpTypeStructDynamic& aStruct) = delete;
                                    GpTypeStructDynamic     (GpTypeStructDynamic&& aStruct) = delete;
    virtual                         ~GpTypeStructDynamic    (void) noexcept override final;

protected:
    virtual void                    _type_id_tag_fn         (_type_id_tag_t&) const noexcept override final;
    virtual const GpTypeStructInfo& _TypeInfo               (void) const noexcept override final;
    virtual GpTypeStructBase::SP    _NewInstance            (void) const override final;
    virtual GpTypeStructBase::SP    _Clone                  (void) const override final;
    virtual const void*             _DataPtr                (void) const noexcept override final;
    virtual void*                   _DataPtr                (void) noexcept override final;

private:
    const GpTypeStructInfo&         iTypeInfo;
    void*                           iData = nullptr;
};

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
