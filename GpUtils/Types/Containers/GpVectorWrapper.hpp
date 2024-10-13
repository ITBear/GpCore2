#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <stdlib.h>
#include <vector>

namespace GPlatform {

template<typename BaseT>
class GpVectorWrapperBase
{
public:
    using value_type    = BaseT;
    using base_type     = BaseT;

protected:
                            GpVectorWrapperBase     (void) noexcept = default;

public:
    virtual                 ~GpVectorWrapperBase    (void) noexcept = default;

    virtual void            clear                   (void) = 0;
    virtual void            reserve                 (size_t aSize) = 0;
    virtual void            resize                  (size_t aSize) = 0;
    virtual void            shrink_to_fit           (void) = 0;

    virtual size_t          size                    (void) const noexcept = 0;
    virtual size_t          stride                  (void) const noexcept = 0;
    virtual const BaseT*    data                    (void) const noexcept = 0;
    virtual BaseT*          data                    (void) noexcept = 0;
    virtual bool            empty                   (void) const noexcept = 0;
    virtual const BaseT&    at                      (size_t aId) const = 0;
    virtual BaseT&          at                      (size_t aId) = 0;
    virtual const BaseT&    operator[]              (size_t aId) const noexcept = 0;
    virtual BaseT&          operator[]              (size_t aId) noexcept = 0;
    virtual void            emplace_back            (BaseT&& aValue) = 0;
};

template<typename                       T,
         typename                       BaseT,
         template<typename...> class    VectorT = std::vector>
class GpVectorWrapper final: public GpVectorWrapperBase<BaseT>
{
public:
    using this_type     = GpVectorWrapper<T, BaseT, VectorT>;
    using value_type    = T;
    using base_type     = BaseT;
    using vector_type   = VectorT<T>;

public:
                            GpVectorWrapper     (void) = default;
                            GpVectorWrapper     (const GpVectorWrapper& aVectorWrapper): iVector{aVectorWrapper.iVector} {}
                            GpVectorWrapper     (GpVectorWrapper&& aVectorWrapper) noexcept: iVector{std::move(aVectorWrapper.iVector)} {}
                            GpVectorWrapper     (const vector_type& aVector): iVector{aVector} {}
                            GpVectorWrapper     (vector_type&& aVector) noexcept: iVector{std::move(aVector)} {}
    virtual                 ~GpVectorWrapper    (void) noexcept override final {}

    const vector_type&      InnerVector         (void) const noexcept {return iVector;}
    vector_type&            InnerVector         (void) noexcept {return iVector;}

    this_type&              operator=           (const this_type& aVectorWrapper) {iVector = aVectorWrapper.iVector; return *this;}
    this_type&              operator=           (this_type&& aVectorWrapper) {iVector = std::move(aVectorWrapper.iVector); return *this;}

    virtual void            clear               (void) override final {iVector.clear();}
    virtual void            reserve             (size_t aSize) override final {return iVector.reserve(aSize);}
    virtual void            resize              (size_t aSize) override final {return iVector.resize(aSize);}
    virtual void            shrink_to_fit       (void) override final {return iVector.shrink_to_fit();}

    virtual size_t          size                (void) const noexcept override final {return std::size(iVector);}
    virtual size_t          stride              (void) const noexcept override final {return std::max(sizeof(T), alignof(T));}
    virtual const BaseT*    data                (void) const noexcept override final {return std::data(iVector);}
    virtual BaseT*          data                (void) noexcept override final {return std::data(iVector);}
    virtual bool            empty               (void) const noexcept override final {return std::empty(iVector);}
    virtual const BaseT&    at                  (size_t aId) const override final {return iVector.at(aId);}
    virtual BaseT&          at                  (size_t aId) override final {return iVector.at(aId);}
    virtual const BaseT&    operator[]          (size_t aId) const noexcept override final {return iVector[aId];}
    virtual BaseT&          operator[]          (size_t aId) noexcept override final {return iVector[aId];}
    virtual void            emplace_back        (BaseT&& aValue) override final {iVector.emplace_back(std::move(static_cast<T&&>(aValue)));}

private:
    vector_type             iVector;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
