#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../GpUtils/Exceptions/GpException.hpp"
#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Containers/GpContainersT.hpp"
#include "../../GpUtils/Types/Strings/GpUTF.hpp"

#include <variant>

namespace GPlatform {

template<typename T>
class GpItcResult
{
public:
    CLASS_DD(GpItcResult<T>)

    using OnSuccessFnT      = std::function<void(T&)>;
    using OnEmptyFnT        = std::function<void()>;
    using OnExceptionFnT    = std::function<void(const GpException&)>;

    struct EmptyValue{};

    using VariantsT = std::variant
    <
        EmptyValue,
        T,
        GpException
    >;

public:
                            GpItcResult     (void) noexcept;
                            GpItcResult     (const EmptyValue&) noexcept;
                            GpItcResult     (const this_type& aRes);
                            GpItcResult     (this_type&& aRes);
                            GpItcResult     (const GpException& aException);
                            GpItcResult     (GpException&& aException);
                            GpItcResult     (const T& aPayload);
                            GpItcResult     (T&& aPayload);
                            ~GpItcResult    (void) noexcept = default;

    GpItcResult&            operator=       (const this_type& aRes);
    GpItcResult&            operator=       (this_type&& aRes);

    bool                    IsException     (void) const noexcept;
    bool                    IsPayload       (void) const noexcept;
    bool                    IsEmpty         (void) const noexcept;

    const GpException&      Exception       (void) const;
    const T&                Payload         (void) const;
    T&                      Payload         (void);
    VariantsT&              Variants        (void) noexcept;

    static void             SExtract        (this_type&             aRes,
                                             const OnSuccessFnT&    aOnSuccessFn,
                                             const OnEmptyFnT&      aOnEmptyFn,
                                             const OnExceptionFnT&  aOnExceptionFn);

private:
    VariantsT               iVariants;
};

template<typename T>
GpItcResult<T>::GpItcResult (void) noexcept:
iVariants(EmptyValue())
{
}

template<typename T>
GpItcResult<T>::GpItcResult (const EmptyValue&) noexcept:
iVariants(EmptyValue())
{
}

template<typename T>
GpItcResult<T>::GpItcResult (const this_type& aRes):
iVariants(aRes.iVariants)
{
}

template<typename T>
GpItcResult<T>::GpItcResult (this_type&& aRes):
iVariants(std::move(aRes.iVariants))
{
}

template<typename T>
GpItcResult<T>::GpItcResult (const GpException& aException):
iVariants(aException)
{
}

template<typename T>
GpItcResult<T>::GpItcResult (GpException&& aException):
iVariants(std::move(aException))
{
}

template<typename T>
GpItcResult<T>::GpItcResult (const T& aPayload):
iVariants(aPayload)
{
}

template<typename T>
GpItcResult<T>::GpItcResult (T&& aPayload):
iVariants(std::move(aPayload))
{
}

template<typename T>
GpItcResult<T>& GpItcResult<T>::operator= (const this_type& aRes)
{
    iVariants = aRes.iVariants;

    return *this;
}

template<typename T>
GpItcResult<T>& GpItcResult<T>::operator= (this_type&& aRes)
{
    iVariants = std::move(aRes.iVariants);

    return *this;
}

template<typename T>
bool    GpItcResult<T>::IsException (void) const noexcept
{
    return std::holds_alternative<GpException>(iVariants);
}

template<typename T>
bool    GpItcResult<T>::IsPayload (void) const noexcept
{
    return std::holds_alternative<T>(iVariants);
}

template<typename T>
bool    GpItcResult<T>::IsEmpty (void) const noexcept
{
    return std::holds_alternative<EmptyValue>(iVariants);
}

template<typename T>
const GpException&  GpItcResult<T>::Exception (void) const
{
    return std::get<GpException>(iVariants);
}

template<typename T>
const T&    GpItcResult<T>::Payload (void) const
{
    return std::get<T>(iVariants);
}

template<typename T>
T&  GpItcResult<T>::Payload (void)
{
    return std::get<T>(iVariants);
}

template<typename T>
typename GpItcResult<T>::VariantsT& GpItcResult<T>::Variants (void) noexcept
{
    return iVariants;
}

template<typename T>
void    GpItcResult<T>::SExtract
(
    this_type&              aRes,
    const OnSuccessFnT&     aOnSuccessFn,
    const OnEmptyFnT&       aOnEmptyFn,
    const OnExceptionFnT&   aOnExceptionFn
)
{
    if (aRes.IsException())
    {
        aOnExceptionFn(aRes.Exception());
        return;
    }

    try
    {
        if (aRes.IsPayload())
        {
            aOnSuccessFn(aRes.Payload());
            return;
        }

        if (aRes.IsEmpty())
        {
            aOnEmptyFn();
            return;
        }
    } catch (const std::exception& e)
    {
        aOnExceptionFn(GpException{GpUTF::S_STR_To_UTF8(e.what())});
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
