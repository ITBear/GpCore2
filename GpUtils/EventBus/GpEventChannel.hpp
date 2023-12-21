#pragma once

#include "../../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "../Macro/GpMacroClass.hpp"
#include "../Macro/GpMacroTags.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "../SyncPrimitives/GpSpinLock.hpp"
#include "../SyncPrimitives/GpMutex.hpp"

#include <boost/container/flat_map.hpp>

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
    using SubscribersT  = boost::container::flat_map<UidT, CallbackFnT>;// TODO: add underlying container like boost::small_container

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
    GpUniqueLock<GpSpinLock> lock(aEventChannel.iSpinLock);

    iSubscribers = std::move(aEventChannel.iSubscribers);
}

template<typename UidT,
         typename ValueT>
void    GpEventChannel<UidT, ValueT>::PushEvent (const ValueT& aEvent) const
{
    GpUniqueLock<GpSpinLock> lock(iSpinLock);

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
    GpUniqueLock<GpSpinLock> lock(iSpinLock);

    const auto[iter, isInsertedNew] = iSubscribers.insert_or_assign(aSubscriberUid, std::move(aCallbackFn));

    return isInsertedNew;
}

template<typename UidT,
         typename ValueT>
size_t  GpEventChannel<UidT, ValueT>::Unsubscribe (const UidT& aSubscriberUid)
{
    GpUniqueLock<GpSpinLock> lock(iSpinLock);

    auto iter = iSubscribers.find(aSubscriberUid);

    if (iter != iSubscribers.end()) [[likely]]
    {
        iSubscribers.erase(iter);
    }

    return iSubscribers.size();
}

}// namespace GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
