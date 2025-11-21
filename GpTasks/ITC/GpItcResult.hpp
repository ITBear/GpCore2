#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

#include <variant>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

template<typename T>
class GpItcResult
{
public:
    CLASS_REMOVE_CTRS_COPY(GpItcResult)
    CLASS_DD(GpItcResult<T>)

    struct NotSetT{};
    struct ExtractedT{};

    using VariantsT = std::variant
    <
        NotSetT,
        ExtractedT,
        GpException,
        T
    >;

public:
                    GpItcResult         (void) noexcept;
                    GpItcResult         (this_type&& aItcResult);
                    GpItcResult         (const GpException& aException);
                    GpItcResult         (GpException&& aException);
                    GpItcResult         (const T& aPayload);
                    GpItcResult         (T&& aPayload);
                    ~GpItcResult        (void) noexcept = default;

    GpItcResult&    operator=           (this_type&& aItcResult);

    template<typename R>
    GpItcResult&    operator=           (R&& aResult);

    bool            IsNotSet            (void) const noexcept;
    bool            IsExtracted         (void) const noexcept;
    bool            IsException         (void) const noexcept;
    bool            IsPayload           (void) const noexcept;

    GpException     ExtractException    (void);
    T               ExtractPayload      (void);

    size_t          VariantIdx          (void) const noexcept;

private:
    VariantsT iVariants;
};

template<typename T>
GpItcResult<T>::GpItcResult (void) noexcept:
iVariants{NotSetT{}}
{
}

template<typename T>
GpItcResult<T>::GpItcResult (this_type&& aItcResult):
iVariants{std::move(aItcResult.iVariants)}
{
    aItcResult.iVariants = ExtractedT{};
}

template<typename T>
GpItcResult<T>::GpItcResult (const GpException& aException):
iVariants{aException}
{
}

template<typename T>
GpItcResult<T>::GpItcResult (GpException&& aException):
iVariants{std::move(aException)}
{
}

template<typename T>
GpItcResult<T>::GpItcResult (const T& aPayload):
iVariants{aPayload}
{
}

template<typename T>
GpItcResult<T>::GpItcResult (T&& aPayload):
iVariants{std::move(aPayload)}
{
}

template<typename T>
GpItcResult<T>& GpItcResult<T>::operator= (this_type&& aItcResult)
{
    iVariants               = std::move(aItcResult.iVariants);
    aItcResult.iVariants    = ExtractedT{};

    return *this;
}

template<typename T>
template<typename R>
GpItcResult<T>& GpItcResult<T>::operator= (R&& aResult)
{
    iVariants = std::forward<R>(aResult);

    return *this;
}

template<typename T>
bool    GpItcResult<T>::IsNotSet (void) const noexcept
{
    return std::holds_alternative<NotSetT>(iVariants);
}

template<typename T>
bool    GpItcResult<T>::IsExtracted (void) const noexcept
{
    return std::holds_alternative<ExtractedT>(iVariants);
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
GpException GpItcResult<T>::ExtractException (void)
{
    GpException ex = std::move(std::get<GpException>(iVariants));
    iVariants = ExtractedT{};

    return ex;
}

template<typename T>
T   GpItcResult<T>::ExtractPayload (void)
{
    T value = std::move(std::get<T>(iVariants));
    iVariants = ExtractedT{};

    return value;
}

template<typename T>
size_t  GpItcResult<T>::VariantIdx (void) const noexcept
{
    return iVariants.index();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
