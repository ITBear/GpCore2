#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

#include <variant>

namespace GPlatform {

template<typename T>
class GpItcResult
{
public:
    CLASS_DD(GpItcResult<T>)

    using VariantsT = std::variant
    <
        GpException,
        T
    >;

public:
                            GpItcResult     (void) noexcept = delete;
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

    const GpException&      Exception       (void) const;
    T&                      PayloadOrThrow  (const SourceLocationT& aSourceLocation = SourceLocationT::current());
    const T&                PayloadOrThrow  (const SourceLocationT& aSourceLocation = SourceLocationT::current()) const;

    VariantsT&              Variants        (void) noexcept;

private:
    VariantsT               iVariants;
};

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
const GpException&  GpItcResult<T>::Exception (void) const
{
    return std::get<GpException>(iVariants);
}

template<typename T>
const T&    GpItcResult<T>::PayloadOrThrow (const SourceLocationT& aSourceLocation) const
{
    const auto index = iVariants.index();

    if (index == 0) [[unlikely]] //GpException
    {
        THROW_GP
        (
            "Result is exception: "_sv + std::get<GpException>(iVariants).what(),
            aSourceLocation
        );
    }

    return std::get<T>(iVariants);
}

template<typename T>
T&  GpItcResult<T>::PayloadOrThrow (const SourceLocationT& aSourceLocation)
{
    const auto index = iVariants.index();

    if (index == 0) [[unlikely]] //GpException
    {
        THROW_GP
        (
            "Result is exception: "_sv + std::get<GpException>(iVariants).what(),
            aSourceLocation
        );
    }

    return std::get<T>(iVariants);
}

template<typename T>
typename GpItcResult<T>::VariantsT& GpItcResult<T>::Variants (void) noexcept
{
    return iVariants;
}

}// namespace GPlatform
