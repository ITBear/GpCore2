#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined (GP_USE_EVENT_BUS)

#include "../Macro/GpMacroClass.hpp"
#include "../Macro/GpMacroTags.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "../SyncPrimitives/GpSpinLock.hpp"
#include "../SyncPrimitives/GpMutex.hpp"

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>

namespace GPlatform {

template<typename UidT,
         typename ValueT>
class GpEventChannel
{
public:
    using EventChannelT = GpEventChannel<UidT, ValueT>;

    CLASS_DD(EventChannelT)
    CLASS_REMOVE_CTRS_COPY(GpEventChannel)
    TAG_SET(THREAD_SAFE)

    using key_type      = UidT;
    using value_type    = ValueT;

    using CallbackFnT   = std::function<void(const UidT& aUid, const ValueT& aEvent)>;
    using SubscribersT  = boost::container::small_flat_map<UidT, CallbackFnT, 16>;// TODO: add underlying container like boost::small_container

public:
                        GpEventChannel  (void) noexcept = default;
                        GpEventChannel  (GpEventChannel&& aEventChannel) noexcept;
                        ~GpEventChannel (void) noexcept = default;

    void                PushEvent       (const ValueT& aEvent) const;
    bool                Subscribe       (const UidT&    aSubscriberUid,
                                         CallbackFnT    aCallbackFn);
    size_t              Unsubscribe     (const UidT&    aSubscriberUid);

private:
    mutable GpSpinLock  iSpinLock;
    SubscribersT        iSubscribers GUARDED_BY(iSpinLock);
};

template<typename UidT,
         typename ValueT>
GpEventChannel<UidT, ValueT>::GpEventChannel (GpEventChannel&& aEventChannel) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{aEventChannel.iSpinLock};

    iSubscribers = std::move(aEventChannel.iSubscribers);
}

template<typename UidT,
         typename ValueT>
void    GpEventChannel<UidT, ValueT>::PushEvent (const ValueT& aEvent) const
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    for (const auto&[uid, fn]: iSubscribers)
    {
        if (fn)
        {
            fn(uid, aEvent);
        }
    }
}

template<typename UidT,
         typename ValueT>
bool    GpEventChannel<UidT, ValueT>::Subscribe
(
    const UidT& aSubscriberUid,
    CallbackFnT aCallbackFn
)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    const auto[iter, isInsertedNew] = iSubscribers.insert_or_assign(aSubscriberUid, std::move(aCallbackFn));

    return isInsertedNew;
}

template<typename UidT,
         typename ValueT>
size_t  GpEventChannel<UidT, ValueT>::Unsubscribe (const UidT& aSubscriberUid)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    auto iter = iSubscribers.find(aSubscriberUid);

    if (iter != std::end(iSubscribers)) [[likely]]
    {
        iSubscribers.erase(iter);
    }

    return std::size(iSubscribers);
}

}// namespace GPlatform

#endif// #if defined (GP_USE_EVENT_BUS)
