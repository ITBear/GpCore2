#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <variant>

namespace GPlatform {

class GpItcResult
{
public:
    CLASS_DD(GpItcResult)

public:
    inline                      GpItcResult     (void) noexcept;
                                GpItcResult     (const GpItcResult& aRes) noexcept = delete;
    inline                      GpItcResult     (GpItcResult&& aRes) noexcept;
    inline                      GpItcResult     (const GpException& aException);
    inline                      GpItcResult     (const std::exception&  aException,
                                                 const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    inline                      GpItcResult     (std::any&& aAny) noexcept;
    virtual                     ~GpItcResult    (void) noexcept = default;

    inline bool                 IsException     (void) const noexcept;
    inline const std::any&      Value           (void) const noexcept;
    inline std::any&            Value           (void) noexcept;

    template<typename T>
    static void                 SExtract        (std::optional<GpItcResult::SP>&        aRes,
                                                 std::function<void(T&&)>               aOnSuccessFn,
                                                 std::function<void(std::string_view)>  aOnErrorFn,
                                                 std::function<void()>                  aOnNullResFn);

private:
    std::any                    iValue;
};

GpItcResult::GpItcResult (void) noexcept:
iValue(size_t(EXIT_SUCCESS))
{
}

GpItcResult::GpItcResult (GpItcResult&& aRes) noexcept:
iValue (std::move(aRes.iValue))
{
}

GpItcResult::GpItcResult (const GpException& aException):
iValue(aException)
{
}

GpItcResult::GpItcResult
(
    const std::exception&   aException,
    const SourceLocationT&  aSourceLocation
) noexcept:
iValue(GpException
(
    aException.what(),
    aSourceLocation
))
{
}

GpItcResult::GpItcResult (std::any&& aAny) noexcept:
iValue(std::move(aAny))
{
}

bool    GpItcResult::IsException (void) const noexcept
{
    return iValue.type() == typeid(GpException);
}

const std::any& GpItcResult::Value (void) const noexcept
{
    return iValue;
}

std::any&   GpItcResult::Value (void) noexcept
{
    return iValue;
}

template<typename T>
void    GpItcResult::SExtract
(
    std::optional<GpItcResult::SP>&         aRes,
    std::function<void(T&&)>                aOnSuccessFn,
    std::function<void(std::string_view)>   aOnErrorFn,
    std::function<void()>                   aOnNullResFn
)
{
    if (aRes.has_value() == false)
    {
        aOnNullResFn();
        return;
    }

    GpItcResult::SP& resSP = aRes.value();

    if (resSP.IsNULL())
    {
        aOnNullResFn();
        return;
    }

    GpItcResult&    res     = resSP.Vn();
    std::any&       value   = res.Value();

    if (value.type() == typeid(GpException))
    {
        return aOnErrorFn(std::any_cast<GpException>(value).what());
    }

    try
    {
        return aOnSuccessFn(std::move(std::any_cast<T>(value)));
    } catch (const std::exception& e)
    {
        return aOnErrorFn(e.what());
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
