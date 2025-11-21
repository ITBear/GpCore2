#include <GpCore2/GpTasks/GpTasksLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpTasks)
GP_LIB_REGISTRATOR(GpTasksLib)

void    GpTasks_StaticInitializer::OnInitialize (void)
{
    GpTasksLib::SRegisterSelf();
}
