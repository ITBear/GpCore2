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

    using ExceptionT                = std::tuple<std::u8string, SourceLocationT>;
    using ExtractFnT_OnSuccess      = std::function<void(T&)>;
    using ExtractFnT_OnException    = std::function<void(const ExceptionT&)>;

    using VariantsT = std::variant
    <
        ExceptionT,
        T
    >;

public:
                                GpItcResult     (void) noexcept = delete;
                                GpItcResult     (const this_type& aRes) = delete;
    inline                      GpItcResult     (this_type&& aRes);
    inline                      GpItcResult     (const GpException& aException);
    inline                      GpItcResult     (const std::exception&  aException,
                                                 const SourceLocationT& aSourceLocation = SourceLocationT::current());
    inline                      GpItcResult     (T aPayload);
    virtual                     ~GpItcResult    (void) noexcept = default;

    inline bool                 IsException     (void) const noexcept;
    inline bool                 IsPayload       (void) const noexcept;

    inline const ExceptionT&    Exception       (void) const;
    inline const T&             Payload         (void) const;
    inline T&                   Payload         (void);
    inline VariantsT&           Variants        (void) noexcept;

    static void                 SExtract        (std::optional<this_type::SP>&  aRes,
                                                 ExtractFnT_OnSuccess           aOnSuccessFn,
                                                 ExtractFnT_OnException         aOnExceptionFn);

private:
    VariantsT                   iVariants;
};

template<typename T>
GpItcResult<T>::GpItcResult (this_type&& aRes):
iVariants(std::move(aRes.iVariants))
{
}

template<typename T>
GpItcResult<T>::GpItcResult (const GpException& aException):
iVariants
(
    ExceptionT
    (
        GpUTF::S_STR_To_UTF8(aException.what()),
        aException.SourceLocation()
    )
)
{
}

template<typename T>
GpItcResult<T>::GpItcResult
(
    const std::exception&   aException,
    const SourceLocationT&  aSourceLocation
):
iVariants(ExceptionT
(
    GpUTF::S_STR_To_UTF8(aException.what()),
    aSourceLocation
))
{
}

template<typename T>
GpItcResult<T>::GpItcResult (T aPayload):
iVariants(std::move(aPayload))
{
}

template<typename T>
bool    GpItcResult<T>::IsException (void) const noexcept
{
    return std::holds_alternative<ExceptionT>(iVariants);
}

template<typename T>
bool    GpItcResult<T>::IsPayload (void) const noexcept
{
    return std::holds_alternative<T>(iVariants);
}

template<typename T>
const typename GpItcResult<T>::ExceptionT&  GpItcResult<T>::Exception (void) const
{
    return std::get<ExceptionT>(iVariants);
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
    std::optional<this_type::SP>&   aRes,
    ExtractFnT_OnSuccess            aOnSuccessFn,
    ExtractFnT_OnException          aOnExceptionFn
)
{
    if (aRes.has_value() == false)
    {
        aOnExceptionFn(ExceptionT{u8"Result is std::nullopt"_sv, SourceLocationT::current()});
        return;
    }

    this_type::SP& resSP = aRes.value();

    if (resSP.IsNULL())
    {
        aOnExceptionFn(ExceptionT{u8"Result is nullptr"_sv, SourceLocationT::current()});
        return;
    }

    this_type& res = resSP.Vn();

    if (res.IsException())
    {
        return aOnExceptionFn(res.Exception());
    }

    try
    {
        return aOnSuccessFn(res.Payload());
    } catch (const std::exception& e)
    {
        return aOnExceptionFn(ExceptionT{GpUTF::S_STR_To_UTF8(e.what()), SourceLocationT::current()});
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
