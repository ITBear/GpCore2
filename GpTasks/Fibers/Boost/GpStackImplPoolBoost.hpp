#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#include <GpCore2/GpTasks/GpTasks_global.hpp>
#include <GpCore2/GpTasks/Fibers/Boost/GpStackBoost.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedPool.hpp>
#include <GpCore2/GpUtils/Types/Units/Other/size_byte_t.hpp>

namespace GPlatform {

class GP_TASKS_API GpStackImplPoolBoost final: protected GpSharedPool<GpStackBoost::StackImplT>
{
public:
                                    GpStackImplPoolBoost    (void) noexcept = default;
    virtual                         ~GpStackImplPoolBoost   (void) noexcept override final = default;

    static GpStackImplPoolBoost&    S                       (void) noexcept {return sInstance;}

    inline void                     Configure               (size_t         aMaxElementsCount,
                                                             size_byte_t    aStackSize);

    using                           GpSharedPool::Acquire;
    using                           GpSharedPool::Release;
    using                           GpSharedPool::Clear;

protected:
    inline virtual value_type       NewElement              (void) override final;

private:
    size_byte_t                 iStackSize = 0_byte;

    static GpStackImplPoolBoost sInstance;
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

GpStackImplPoolBoost::value_type    GpStackImplPoolBoost::NewElement (void)
{
    return GpStackBoost::StackImplT(iStackSize.As<size_t>());
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
