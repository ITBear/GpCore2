#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>

namespace GPlatform {

template<typename FnT>
class GpDefer
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDefer)

public:
                GpDefer     (FnT&& aFn) noexcept;
                ~GpDefer    (void) noexcept;

private:
    FnT         iFn;
};

template<typename FnT>
GpDefer<FnT>::GpDefer (FnT&& aFn) noexcept: iFn(std::move(aFn))
{
}

template<typename FnT>
GpDefer<FnT>::~GpDefer (void) noexcept
{
    try
    {
        iFn();
    } catch (const GpException& e)
    {
        GpOutUtils::S().Err(e.what());
    } catch (const std::exception& e)
    {
        GpOutUtils::S().Err(e.what());
    } catch (...)
    {
        GpOutUtils::S().Err("[GpDefer::~GpDefer]: Unknown exception"_sv);
    }
}

}// namespace GPlatform
