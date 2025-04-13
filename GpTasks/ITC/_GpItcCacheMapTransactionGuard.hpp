#pragma once

/*
#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpTasks/ITC/GpItcCacheMap.hpp>
#include <GpCore2/GpUtils/Concepts/GpConcepts.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>

namespace GPlatform {

TAG_REGISTER(GpItcCacheMapTransactionGuard)

template<typename ContainerT>
class GpItcCacheMapTransactionGuard
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_COPY(GpItcCacheMapTransactionGuard)
    TAG_SET(THREAD_SAFE)
    TAG_SET(GpItcCacheMapTransactionGuard)

    using CommitResStatus   = GpItcCacheMapTransactionGuardCommitResStatus;
    using this_type         = GpItcCacheMapTransactionGuard<ContainerT>;
    using cache_type        = GpItcCacheMap<ContainerT>;
    using CommitResT        = std::tuple<std::optional<typename cache_type::mapped_type>, CommitResStatus>;

public:
                                    GpItcCacheMapTransactionGuard   (GpItcCacheMapTransactionGuard&& aGuard) noexcept;

    template<typename K, typename V>
                                    GpItcCacheMapTransactionGuard   (K&&            aKey,
                                                                     V&&            aValue,
                                                                     cache_type&    aCache);

                                    ~GpItcCacheMapTransactionGuard  (void) noexcept = default;

    GpItcCacheMapTransactionGuard&  operator=                       (GpItcCacheMapTransactionGuard&&) noexcept;

    CommitResT                      Commit                          (const cache_type::IsUseGeneratedValueFnT&                                                  aIsUseGeneratedValueFn,
                                                                     const cache_type::EvictionFnT&                                                             aEvictionFn,
                                                                     const typename cache_type::template CheckExtUpdateLockFnT<typename cache_type::key_type>&  aCheckExtUpdateLockFn,
                                                                     const typename cache_type::template MakeExtUpdateLockFnT<typename cache_type::key_type>&   aMakeExtUpdateLockFn);
    void                            Rollback                        (void);

private:
    cache_type::key_type            iKey;
    cache_type::mapped_type         iValue;
    cache_type*                     iCache = nullptr;
};


te  mplate<typename ContainerT>
GpItcCacheMapTransactionGuard<ContainerT>::GpItcCacheMapTransactionGuard (GpItcCacheMapTransactionGuard&& aGuard) noexcept:
iKey  {std::move(aGuard.iKey)},
iValue{std::move(aGuard.iValue)},
iCache{std::move(aGuard.iCache)}
{
    aGuard.iCache = nullptr;
}

template<typename ContainerT>
template<typename K,
         typename V>
GpItcCacheMapTransactionGuard<ContainerT>::GpItcCacheMapTransactionGuard
(
    K&&         aKey,
    V&&         aValue,
    cache_type& aCache
):
iKey  {std::forward<K>(aKey)},
iValue{std::forward<V>(aValue)},
iCache{&aCache}
{
}

template<typename ContainerT>
auto GpItcCacheMapTransactionGuard<ContainerT>::operator= (GpItcCacheMapTransactionGuard&& aGuard) noexcept -> GpItcCacheMapTransactionGuard&
{
    iKey    = std::move(aGuard.iKey);
    iValue  = std::move(aGuard.iValue);
    iCache  = aGuard.iCache;

    aGuard.iCache = nullptr;

    return *this;
}

template<typename ContainerT>
auto    GpItcCacheMapTransactionGuard<ContainerT>::Commit
(
    const cache_type::IsUseGeneratedValueFnT&                                                   aIsUseGeneratedValueFn,
    const cache_type::EvictionFnT&                                                              aEvictionFn,
    const typename cache_type::template CheckExtUpdateLockFnT<typename cache_type::key_type>&   aCheckExtUpdateLockFn,
    const typename cache_type::template MakeExtUpdateLockFnT<typename cache_type::key_type>&    aMakeExtUpdateLockFn
) -> CommitResT
{
    // Check cache
    if (iCache == nullptr) [[unlikely]]
    {
        return CommitResT{std::nullopt, CommitResStatus::NO_OPEN_TRANSACTION};
    }

    GpRAIIonDestruct onDestruct = [&](){iCache = nullptr;};

    auto[value, status] = iCache->Update
    (
        std::move(iKey),
        std::move(iValue),
        aIsUseGeneratedValueFn,
        aEvictionFn,
        aCheckExtUpdateLockFn,
        aMakeExtUpdateLockFn
    );

    return CommitResT
    {
        std::move(value),
        status == GpItcCacheMapUpdateStatus::EXISTING ? CommitResStatus::EXISTING : CommitResStatus::GENERATED
    };
}

template<typename ContainerT>
void    GpItcCacheMapTransactionGuard<ContainerT>::Rollback (void)
{
    iCache = nullptr;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
*/
