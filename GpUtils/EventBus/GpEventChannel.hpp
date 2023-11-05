#pragma once

#include "../../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "../Macro/GpMacroClass.hpp"
#include "../Macro/GpMacroTags.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "../SyncPrimitives/GpSpinLock.hpp"
#include <mutex>

namespace GPlatform {

template<typename KeyT,
         typename ValueT>
class GpEventChannel
{
public:
    using EventChannelT = GpEventChannel<KeyT, ValueT>;

    CLASS_DD(EventChannelT)
    CLASS_REMOVE_CTRS_COPY(GpEventChannel)
    TAG_SET(THREAD_SAFE)

    using key_type      = KeyT;
    using value_type    = ValueT;

    using CallbackFnT   = std::function<void(const ValueT& aEvent)>;
    using SubscribersT  = std::variant
    <
        std::tuple<KeyT, CallbackFnT>,
        std::map<KeyT, CallbackFnT>
    >;

public:
                        GpEventChannel  (void) noexcept = default;
                        GpEventChannel  (GpEventChannel&& aEventChannel) noexcept;
                        ~GpEventChannel (void) noexcept = default;

    void                PushEvent       (const ValueT& aEvent) const;
    bool                Subscribe       (const KeyT&    aSubscriberUid,
                                         CallbackFnT&&  aCallbackFn);
    size_t              Unsubscribe     (const KeyT&    aSubscriberUid);

private:
    mutable GpSpinLock  iLock;
    SubscribersT        iSubscribers;
};

template<typename KeyT,
         typename ValueT>
GpEventChannel<KeyT, ValueT>::GpEventChannel (GpEventChannel&& aEventChannel) noexcept
{
    std::scoped_lock lock(aEventChannel.iLock);

    iSubscribers = std::move(aEventChannel.iSubscribers);
}

template<typename KeyT,
         typename ValueT>
void    GpEventChannel<KeyT, ValueT>::PushEvent (const ValueT& aEvent) const
{
    std::scoped_lock lock(iLock);

    if (iSubscribers.index() == 0) [[likely]] // EventCallbackFnT
    {
        const auto&[uid, fn] = std::get<0>(iSubscribers);
        if (fn)
        {
            fn(aEvent);
        }
    } else//std::map<KeyT, CallbackFnT>
    {
        const std::map<KeyT, CallbackFnT>& subscribersMap = std::get<1>(iSubscribers);

        for (const auto&[uid, fn]: subscribersMap)
        {
            if (fn)
            {
                fn(aEvent);
            }
        }
    }
}

template<typename KeyT,
         typename ValueT>
bool    GpEventChannel<KeyT, ValueT>::Subscribe
(
    const KeyT&     aSubscriberUid,
    CallbackFnT&&   aCallbackFn
)
{
    std::scoped_lock lock(iLock);

    if (iSubscribers.index() == 0) [[likely]] // std::tuple<KeyT, CallbackFnT>
    {
        auto&[uid, fn] = std::get<0>(iSubscribers);

        if (!fn)
        {
            iSubscribers = std::make_tuple(aSubscriberUid, std::move(aCallbackFn));

            return true;
        } else if (uid == aSubscriberUid)
        {
            fn = std::move(aCallbackFn);

            return false;
        } else
        {
            KeyT        subscriberId    = std::move(uid);
            CallbackFnT callbackFn      = std::move(fn);

            iSubscribers = std::map<KeyT, CallbackFnT>();
            std::map<KeyT, CallbackFnT>& subscribersMap = std::get<1>(iSubscribers);
            subscribersMap.emplace(std::move(subscriberId), std::move(callbackFn));
            subscribersMap.emplace(aSubscriberUid, std::move(aCallbackFn));

            return true;
        }
    } else//std::map<KeyT, CallbackFnT>
    {
        std::map<KeyT, CallbackFnT>& subscribersMap = std::get<1>(iSubscribers);
        const auto[iter, isInsertNew] = subscribersMap.insert_or_assign(aSubscriberUid, std::move(aCallbackFn));
        return isInsertNew;
    }
}

template<typename KeyT,
         typename ValueT>
size_t  GpEventChannel<KeyT, ValueT>::Unsubscribe (const KeyT& aSubscriberUid)
{
    std::scoped_lock lock(iLock);

    if (iSubscribers.index() == 0) [[likely]] // std::tuple<KeyT, CallbackFnT>
    {
        auto&[uid, fn] = std::get<0>(iSubscribers);

        if (uid == aSubscriberUid)
        {
            uid = KeyT();
            fn  = CallbackFnT();
            return 0;
        } else if (fn)
        {
            return 1;
        } else
        {
            return 0;
        }
    } else//std::map<KeyT, CallbackFnT>
    {
        std::map<KeyT, CallbackFnT>& subscribersMap = std::get<1>(iSubscribers);
        subscribersMap.erase(aSubscriberUid);

        return subscribersMap.size();
    }
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
