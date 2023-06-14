#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectObject.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectObjectDynamic final: public GpReflectObject
{
public:
    CLASS_DD(GpReflectObjectDynamic)

    using BaseT = GpReflectObject;

public:
    class Factory final: public GpReflectObjectFactory
    {
    public:
        virtual GpReflectObject::SP NewInstance (const GpUUID& aModelUid) const override final;
        virtual void                Construct   (void* aDataPtr) const override final;
        virtual void                Destruct    (void* aDataPtr) const override final;
    };

public:
    inline                          GpReflectObjectDynamic  (const GpUUID&  aModelUid,
                                                             void*          aData) noexcept;
                                    GpReflectObjectDynamic  (void) noexcept = delete;
                                    GpReflectObjectDynamic  (const GpReflectObjectDynamic& aStruct) = delete;
                                    GpReflectObjectDynamic  (GpReflectObjectDynamic&& aStruct) = delete;
    virtual                         ~GpReflectObjectDynamic (void) noexcept override final;

    static const GpReflectModel&    SReflectModel           (void) noexcept = delete;
    static constexpr GpUUID         SReflectModelUid        (void) noexcept = delete;
    static const GpReflectModel&    _SReflectModelInit      (void) = delete;

protected:
    virtual void                    _type_id_tag_fn         (_type_id_tag_t&) const noexcept override final {}
    virtual const GpReflectModel&   _ReflectModel           (void) const override final;
    virtual GpReflectObject::SP     _ReflectNewInstance     (void) const override final;
    virtual GpReflectObject::SP     _ReflectClone           (void) const override final;
    virtual const void*             _ReflectDataPtr         (void) const noexcept override final;
    virtual void*                   _ReflectDataPtr         (void) noexcept override final;

private:
    const GpUUID                    iModelUid;
    void*                           iData = nullptr;
};

GpReflectObjectDynamic::GpReflectObjectDynamic
(
    const GpUUID&   aModelUid,
    void*           aData
) noexcept:
iModelUid(aModelUid),
iData(aData)
{
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
