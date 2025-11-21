#include <GpCore2/GpUtils/Other/GpStaticInitializer.hpp>
#include <atomic>

namespace GPlatform {

GpStaticInitializer::GpStaticInitializer (void) noexcept
{
}

GpStaticInitializer::~GpStaticInitializer (void) noexcept
{
}

void    GpStaticInitializer::Initialize (void)
{
    static std::atomic_flag sIsInitialized = false;

    if (sIsInitialized.test_and_set() == true)
    {
        return;
    }

    OnInitialize();
}

}// namespace GPlatform
