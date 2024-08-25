#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>

#include <functional>

namespace GPlatform {

template<typename FnT>
class GpRAIIonDestruct
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpRAIIonDestruct)

public:
                GpRAIIonDestruct    (FnT&& aFn) noexcept;
                ~GpRAIIonDestruct   (void) noexcept;

private:
    FnT         iFn;
};

template<typename FnT>
GpRAIIonDestruct<FnT>::GpRAIIonDestruct (FnT&& aFn) noexcept: iFn(std::move(aFn))
{
}

template<typename FnT>
GpRAIIonDestruct<FnT>::~GpRAIIonDestruct (void) noexcept
{
    try
    {
        iFn();
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpRAIIonDestruct::~GpRAIIonDestruct]: Unknown exception"_sv);
    }
}

}// namespace GPlatform
