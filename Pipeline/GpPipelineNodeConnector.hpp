#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "../GpCore_global.hpp"
#include "../Types/Classes/GpClassesDefines.hpp"
#include "../Types/Containers/GpContainersT.hpp"

namespace GPlatform {

class GpPipelineNodeSocket;

class GPCORE_API GpPipelineNodeConnector
{
public:
    CLASS_DECLARE_DEFAULTS(GpPipelineNodeConnector)

    using SocketT = GpPipelineNodeSocket;

public:
                                    GpPipelineNodeConnector     (void) noexcept = default;
    inline                          GpPipelineNodeConnector     (SocketT& aFrom, SocketT& aTo) noexcept;
    inline                          GpPipelineNodeConnector     (const GpPipelineNodeConnector& aConnector) noexcept;
    inline                          GpPipelineNodeConnector     (GpPipelineNodeConnector&& aConnector) noexcept;
                                    ~GpPipelineNodeConnector    (void) noexcept = default;

    inline GpPipelineNodeConnector& operator=                   (const GpPipelineNodeConnector& aConnector) noexcept;
    inline GpPipelineNodeConnector& operator=                   (GpPipelineNodeConnector&& aConnector) noexcept;

    const SocketT&                  From                        (void) const noexcept {return *iFrom;}
    SocketT&                        From                        (void) noexcept {return *iFrom;}

    const SocketT&                  To                          (void) const noexcept {return *iTo;}
    SocketT&                        To                          (void) noexcept {return *iTo;}

    inline bool                     IsSame                      (const GpPipelineNodeConnector& aConnector) const noexcept;

private:
    SocketT*                        iFrom   = nullptr;
    SocketT*                        iTo     = nullptr;
};

GpPipelineNodeConnector::GpPipelineNodeConnector (SocketT& aFrom, SocketT& aTo) noexcept:
iFrom(&aFrom),
iTo(&aTo)
{
}

GpPipelineNodeConnector::GpPipelineNodeConnector (const GpPipelineNodeConnector& aConnector) noexcept:
iFrom(aConnector.iFrom),
iTo(aConnector.iTo)
{
}

GpPipelineNodeConnector::GpPipelineNodeConnector (GpPipelineNodeConnector&& aConnector) noexcept:
iFrom(std::move(aConnector.iFrom)),
iTo(std::move(aConnector.iTo))
{
}

GpPipelineNodeConnector&    GpPipelineNodeConnector::operator= (const GpPipelineNodeConnector& aConnector) noexcept
{
    iFrom   = aConnector.iFrom;
    iTo     = aConnector.iTo;

    return *this;
}

GpPipelineNodeConnector&    GpPipelineNodeConnector::operator= (GpPipelineNodeConnector&& aConnector) noexcept
{
    iFrom   = std::move(aConnector.iFrom);
    iTo     = std::move(aConnector.iTo);

    return *this;
}

bool    GpPipelineNodeConnector::IsSame (const GpPipelineNodeConnector& aConnector) const noexcept
{
    return     (iTo   == aConnector.iTo)
            && (iFrom == aConnector.iFrom);
}

}//namespace GPlatform

#endif//GP_USE_PIPELINE
