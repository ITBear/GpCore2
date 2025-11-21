#include <GpCore2/GpReflection/GpReflectionLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpReflection)
GP_LIB_REGISTRATOR(GpReflectionLib)

void    GpReflection_StaticInitializer::OnInitialize (void)
{
    GpReflectionLib::SRegisterSelf();
}
