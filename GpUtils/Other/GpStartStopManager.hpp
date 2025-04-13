#pragma once

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Other/GpStartStopManager.hpp>

namespace GPlatform {

class GP_UTILS_API GpStartStopManager
{
public:
    CLASS_DD(GpStartStopManager)

    using ExceptionsT   = GpException::C::SmallVec<8>;
    using StartFnT      = std::function<void()>;
    using StopFnT       = std::function<void(ExceptionsT& aStopExceptions)>;
    using LevelT        = std::tuple<StartFnT, StopFnT>;
    using LevelsT       = boost::container::small_vector<LevelT, 16>;

public:
                                        GpStartStopManager  (void) noexcept;
                                        GpStartStopManager  (const GpStartStopManager& aManager);
                                        GpStartStopManager  (GpStartStopManager&& aManager) noexcept;
                                        ~GpStartStopManager (void) noexcept;

    GpStartStopManager&                 operator=           (const GpStartStopManager& aManager);
    GpStartStopManager&                 operator=           (GpStartStopManager&& aManager) noexcept;

    [[nodiscard]] GpException::C::Opt   StartAll            (void) noexcept;
    [[nodiscard]] ExceptionsT           StopAll             (void) noexcept;
    void                                StopAll             (ExceptionsT& aStopExceptions) noexcept;

    void                                AddLevel            (StartFnT   aStartFn,
                                                             StopFnT    aStopFn);

private:
    LevelsT                             iLevels;
    size_t                              iCurrentLevel = 0;
};

}// namespace GPlatform
