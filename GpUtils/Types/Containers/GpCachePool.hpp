#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../../SyncPrimitives/GpSpinlock.hpp"
#include "../Strings/GpStringOps.hpp"
#include "../Strings/GpUTF.hpp"
#include "../Strings/GpStringUtils.hpp"

#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace GPlatform {

template<typename KeyT,
         typename ValueT>
class GpCachePoolElement
{
    CLASS_REMOVE_CTRS_COPY(GpCachePoolElement)

public:
    using this_type     = GpCachePoolElement<KeyT, ValueT>;
    using key_type      = KeyT;
    using value_type    = ValueT;

public:
                    GpCachePoolElement  (void) noexcept = default;
                    GpCachePoolElement  (KeyT&&     aKey,
                                         ValueT&&   aValue) noexcept;
                    GpCachePoolElement  (GpCachePoolElement&& aElement) noexcept;
                    ~GpCachePoolElement (void) noexcept = default;

    const KeyT&     Key                 (void) const noexcept {return iKey;}
    const ValueT&   Value               (void) const noexcept {return iValue;}
    ValueT&         Value               (void) noexcept {return iValue;}

private:
    KeyT            iKey;
    ValueT          iValue;
};

template<typename KeyT,
         typename ValueT>
GpCachePoolElement<KeyT, ValueT>::GpCachePoolElement
(
    KeyT&&      aKey,
    ValueT&&    aValue
) noexcept:
iKey(std::move(aKey)),
iValue(std::move(aValue))
{
}

template<typename KeyT,
         typename ValueT>
GpCachePoolElement<KeyT, ValueT>::GpCachePoolElement (GpCachePoolElement&& aElement) noexcept:
iKey(std::move(aElement.iKey)),
iValue(std::move(aElement.iValue))
{
}

template<typename ElementT,
         typename CacheT>
class GpCachePoolElementGuard
{
    CLASS_REMOVE_CTRS_DEFAULT_COPY(GpCachePoolElementGuard)

public:
    using this_type     = GpCachePoolElementGuard<ElementT, CacheT>;
    using CacheOptRefT  = std::optional<std::reference_wrapper<CacheT>>;

public:
                    GpCachePoolElementGuard     (CacheT&    aCacheRef,
                                                 ElementT&& aElement) noexcept;
                    GpCachePoolElementGuard     (GpCachePoolElementGuard&& aGuard) noexcept;
                    ~GpCachePoolElementGuard    (void) noexcept;

    const ElementT& Element                     (void) const noexcept {return iElement;}
    ElementT&       Element                     (void) noexcept {return iElement;}

private:
    CacheOptRefT    iCacheRef;
    ElementT        iElement;
};

template<typename ElementT,
         typename CacheT>
GpCachePoolElementGuard<ElementT, CacheT>::GpCachePoolElementGuard
(
    CacheT&     aCacheRef,
    ElementT&&  aElement
) noexcept:
iCacheRef(aCacheRef),
iElement(std::move(aElement))
{
}

template<typename ElementT,
         typename CacheT>
GpCachePoolElementGuard<ElementT, CacheT>::GpCachePoolElementGuard (GpCachePoolElementGuard&& aGuard) noexcept:
iCacheRef(std::move(aGuard.iCacheRef)),
iElement(std::move(aGuard.iElement))
{
    aGuard.iCacheRef.reset();
}

template<typename ElementT,
         typename CacheT>
GpCachePoolElementGuard<ElementT, CacheT>::~GpCachePoolElementGuard (void) noexcept
{
    try
    {
        if (iCacheRef.has_value())
        {
            iCacheRef.value().get().Release(std::move(iElement));
        }
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpCachePoolElementGuard::~GpCachePoolElementGuard]: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpCachePoolElementGuard::~GpCachePoolElementGuard]: unkown exception"_sv);
    }
}

template<typename KeyT,
         typename ValueT>
class GpCachePool
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpCachePool)

public:
    using this_type                 = GpCachePool<KeyT, ValueT>;
    using key_type                  = KeyT;
    using value_type                = ValueT;
    using CachePoolElementT         = GpCachePoolElement<KeyT, ValueT>;
    using CachePoolElementGuardT    = GpCachePoolElementGuard<CachePoolElementT, this_type>;
    using CachePoolElementGuardOptT = std::optional<CachePoolElementGuardT>;
    using ContainerReleaseT         = std::unordered_map<key_type, CachePoolElementT>;
    using ContainerAcquireT         = std::unordered_set<key_type>;
    using ValueGenFnT               = std::function<ValueT()>;

public:
                                GpCachePool     (const size_t aCountLimit) noexcept;
                                ~GpCachePool    (void) noexcept = default;

    size_t                      CountTotal      (void) const noexcept;

    CachePoolElementGuardOptT   Acquire         (const KeyT&    aKey,
                                                 ValueGenFnT    aGenFn);
    void                        Release         (CachePoolElementT&& aElement);

private:
    void                        _EliminateOne   (void);

private:
    mutable GpSpinlock          iLock;
    ContainerAcquireT           iContainerAcquire;
    ContainerReleaseT           iContainerRelease;
    size_t                      iCountLimit = 0;
};

template<typename KeyT,
         typename ValueT>
GpCachePool<KeyT, ValueT>::GpCachePool (const size_t aCountLimit) noexcept:
iCountLimit(aCountLimit)
{
}

template<typename KeyT,
         typename ValueT>

size_t  GpCachePool<KeyT, ValueT>::CountTotal (void) const noexcept
{
    std::scoped_lock lock(iLock);

    return iContainerAcquire.size() + iContainerRelease.size();
}

template<typename KeyT,
         typename ValueT>
typename GpCachePool<KeyT, ValueT>::CachePoolElementGuardOptT   GpCachePool<KeyT, ValueT>::Acquire
(
    const KeyT&     aKey,
    ValueGenFnT     aGenFn
)
{
    std::scoped_lock lock(iLock);

    //Try to find in acquire
    {
        if (iContainerAcquire.count(aKey) > 0)
        {
            return std::nullopt;
        }
    }

    //Try to find in release
    {
        auto iter = iContainerRelease.find(aKey);

        if (iter != iContainerRelease.end())
        {
            CachePoolElementGuardOptT res(CachePoolElementGuardT
            (
                *this,
                std::move(iter->second)
            ));

            iContainerRelease.erase(iter);
            iContainerAcquire.emplace(aKey);

            return res;
        }
    }

    //Check limits
    if ((iContainerAcquire.size() + iContainerRelease.size()) >= iCountLimit)
    {
        if (iContainerRelease.size() > 0)
        {
            _EliminateOne();
        } else
        {
            THROW_GP(u8"Cache limit was reached"_sv);
        }
    }

    //Generate new
    {
        CachePoolElementGuardOptT res(CachePoolElementGuardT
        (
            *this,
            CachePoolElementT
            (
                typename CachePoolElementT::key_type(aKey),
                aGenFn()
            )
        ));

        iContainerAcquire.emplace(aKey);

        return res;
    }
}

template<typename KeyT,
         typename ValueT>
void    GpCachePool<KeyT, ValueT>::Release (CachePoolElementT&& aElement)
{
    std::scoped_lock lock(iLock);

    iContainerAcquire.erase(aElement.Key());
    iContainerRelease.try_emplace(KeyT(aElement.Key()), std::move(aElement));
}

template<typename KeyT,
         typename ValueT>
void    GpCachePool<KeyT, ValueT>::_EliminateOne (void)
{
    //TODO: reimplement with priority queue
    iContainerRelease.erase(iContainerRelease.begin());
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
