#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

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
        virtual GpReflectObject::SP NewInstanceSP       (const GpUUID& aModelUid) const override final;
        virtual void                ConstructInplace    (void* aDataPtr) const override final;
        virtual void                DestructInplace     (void* aDataPtr) const override final;
        virtual const VecWrapInfoT& VecWrapInfo         (void) const noexcept override final;
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
    virtual GpReflectModel::CSP     _ReflectModel           (void) const override final;
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
iData    (aData)
{
}

}// namespace GPlatform
