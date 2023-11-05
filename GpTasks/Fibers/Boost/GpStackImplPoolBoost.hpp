#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#include "../../GpTasks_global.hpp"
#include "GpStackBoost.hpp"
#include "../../../GpUtils/Types/Containers/GpElementsPool.hpp"
#include "../../../GpUtils/Types/Units/Other/size_byte_t.hpp"

namespace GPlatform {

class GP_TASKS_API GpStackImplPoolBoost final: protected GpElementsPool<GpStackBoost::StackImplT>
{
public:
                                    GpStackImplPoolBoost    (void) noexcept = default;
    virtual                         ~GpStackImplPoolBoost   (void) noexcept override final = default;

    static GpStackImplPoolBoost&    S                       (void) noexcept {return sInstance;}

    inline void                     Configure               (const size_t       aMaxElementsCount,
                                                             const size_byte_t  aStackSize);

    using                           GpElementsPool::Acquire;
    using                           GpElementsPool::Release;
    using                           GpElementsPool::Clear;

protected:
    inline virtual void             PreInit                 (const size_t aCount) override final;
    inline virtual value_type       NewElement              (void) override final;
    inline virtual void             OnClear                 (void) noexcept override final;

private:
    size_byte_t                     iStackSize = 0_byte;

    static GpStackImplPoolBoost     sInstance;
};

void    GpStackImplPoolBoost::Configure
(
    const size_t        aMaxElementsCount,
    const size_byte_t   aStackSize
)
{
    iStackSize = aStackSize;
    Init(0, aMaxElementsCount);
}

void    GpStackImplPoolBoost::PreInit (const size_t /*aCount*/)
{
    //NOP
}

GpStackImplPoolBoost::value_type    GpStackImplPoolBoost::NewElement (void)
{
    return GpStackBoost::StackImplT(iStackSize.As<size_t>());
}

void    GpStackImplPoolBoost::OnClear (void) noexcept
{
    //NOP
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
