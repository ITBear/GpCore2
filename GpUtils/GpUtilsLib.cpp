#include <GpCore2/GpUtils/GpUtilsLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpUtils)
GP_LIB_REGISTRATOR(GpUtilsLib)

void    GpUtils_StaticInitializer::OnInitialize (void)
{
    GpUtilsLib::SRegisterSelf();
}
