#pragma once

#include "../Macro/GpMacroClass.hpp"
#include "../Types/Strings/GpStringUtils.hpp"
#include "../Exceptions/GpExceptionUtils.hpp"

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
        GpStringUtils::SCerr(GpExceptionUtils::SToString(e));
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(GpExceptionUtils::SToString(e));
    } catch (...)
    {
        GpStringUtils::SCerr("[GpRAIIonDestruct::~GpRAIIonDestruct]: Unknown exception"_sv);
    }
}

}// namespace GPlatform
