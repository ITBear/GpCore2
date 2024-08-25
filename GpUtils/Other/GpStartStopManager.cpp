#include <GpCore2/GpUtils/Other/GpStartStopManager.hpp>

namespace GPlatform {

GpStartStopManager::GpStartStopManager (void) noexcept
{
}

GpStartStopManager::GpStartStopManager (const GpStartStopManager& aManager):
iLevels{aManager.iLevels}
{
}

GpStartStopManager::GpStartStopManager (GpStartStopManager&& aManager) noexcept:
iLevels{std::move(aManager.iLevels)}
{
}

GpStartStopManager::~GpStartStopManager (void) noexcept
{
}

GpStartStopManager& GpStartStopManager::operator= (const GpStartStopManager& aManager)
{
    iLevels = aManager.iLevels;

    return *this;
}

GpStartStopManager& GpStartStopManager::operator= (GpStartStopManager&& aManager) noexcept
{
    iLevels = std::move(aManager.iLevels);

    return *this;
}

GpException::C::Opt GpStartStopManager::StartAll (void) noexcept
{
    const size_t levelsCount = std::size(iLevels);

    if (levelsCount == 0) [[unlikely]]
    {
        return {};
    }

    for (iCurrentLevel = 0; iCurrentLevel < levelsCount; iCurrentLevel++)
    {
        const auto& [startFn, stopFn] = iLevels[iCurrentLevel];

        // Try to call start Fn
        try
        {
            if (startFn)
            {
                startFn();
            }
        } catch (const GpException& ex)
        {
            return ex;
        } catch (const std::exception& ex)
        {
            return GpException{ex.what()};
        } catch (...)
        {
            return GpException{"Unknown exception"};
        }
    }

    iCurrentLevel = levelsCount - 1;

    return std::nullopt;
}

GpStartStopManager::StopExceptionsT GpStartStopManager::StopAll (void) noexcept
{
    StopExceptionsT stopExceptions;
    StopAll(stopExceptions);

    return stopExceptions;
}

void    GpStartStopManager::StopAll (StopExceptionsT& aStopExceptions) noexcept
{
    for (ssize_t id = NumOps::SConvert<ssize_t>(iCurrentLevel); id >= 0; id--)
    {
        const auto& [startFn, stopFn] = iLevels[static_cast<size_t>(id)];

        // Try to call start Fn
        try
        {
            if (stopFn)
            {
                stopFn();
            }
        } catch (const GpException& ex)
        {
            aStopExceptions.emplace_back(ex);
        } catch (const std::exception& ex)
        {
            aStopExceptions.emplace_back(GpException{ex.what()});
        } catch (...)
        {
            aStopExceptions.emplace_back(GpException{"Unknown exception"});
        }
    }
}

void    GpStartStopManager::AddLevel
(
    StartFnT    aStartFn,
    StopFnT     aStopFn
)
{
    iLevels.emplace_back
    (
        LevelT
        {
            std::move(aStartFn),
            std::move(aStopFn)
        }
    );
}

}// namespace GPlatform
