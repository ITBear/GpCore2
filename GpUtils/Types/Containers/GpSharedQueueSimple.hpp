#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>

#include <queue>
#include <optional>

namespace GPlatform {

template <typename T>
class GpSharedQueueSimple
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSharedQueueSimple)
    CLASS_DD(GpSharedQueueSimple<T>)
    TAG_SET(THREAD_SAFE)

    using value_type            = T;
    using underlying_container  = std::queue<value_type>;

public:
                                    GpSharedQueueSimple     (void) noexcept;
                                    GpSharedQueueSimple     (size_t aMaxSize) noexcept;
                                    ~GpSharedQueueSimple    (void) noexcept = default;

    size_t                          MaxSize                 (void) const noexcept;
    void                            SetMaxSize              (size_t aMaxSize) noexcept;
    size_t                          Size                    (void) const noexcept;
    bool                            Empty                   (void) const noexcept;
    void                            Clear                   (void);

    [[nodiscard]] bool              Push                    (const value_type& aValue);
    [[nodiscard]] bool              Push                    (value_type&& aValue);

    std::optional<value_type>       Front                   (void);
    std::optional<value_type>       Pop                     (void);

    underlying_container&           UnderlyingContainer     (void) noexcept REQUIRES(iSpinLockRW);
    const underlying_container&     UnderlyingContainer     (void) const noexcept REQUIRES(iSpinLockRW);

private:
    mutable GpSpinLockRW            iSpinLockRW;
    underlying_container            iContainer  GUARDED_BY(iSpinLockRW);
    size_t                          iMaxSize    GUARDED_BY(iSpinLockRW) = std::numeric_limits<size_t>::max();
};

template <typename T>
GpSharedQueueSimple<T>::GpSharedQueueSimple (void) noexcept
{
}

template <typename T>
GpSharedQueueSimple<T>::GpSharedQueueSimple (size_t aMaxSize) noexcept:
iMaxSize{aMaxSize}
{
}

template <typename T>
size_t  GpSharedQueueSimple<T>::MaxSize (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock(iSpinLockRW);

    return iMaxSize;
}

template <typename T>
void    GpSharedQueueSimple<T>::SetMaxSize (size_t aMaxSize) noexcept
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iMaxSize = aMaxSize;
}

template <typename T>
size_t  GpSharedQueueSimple<T>::Size (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock(iSpinLockRW);

    return std::size(iContainer);
}

template <typename T>
bool    GpSharedQueueSimple<T>::Empty (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock(iSpinLockRW);

    return iContainer.empty();
}

template <typename T>
void    GpSharedQueueSimple<T>::Clear (void)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    while (!iContainer.empty())
    {
        iContainer.pop();
    }
}

template <typename T>
bool    GpSharedQueueSimple<T>::Push (const value_type& aValue)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpSharedQueueSimple<T>::Push (value_type&& aValue)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        return true;
    } else
    {
        return false;
    }
}

template <typename T>
std::optional<typename GpSharedQueueSimple<T>::value_type>  GpSharedQueueSimple<T>::Front (void)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    if (iContainer.empty()) [[unlikely]]
    {
        return std::nullopt;
    }

    return iContainer.front();
}

template <typename T>
std::optional<typename GpSharedQueueSimple<T>::value_type>  GpSharedQueueSimple<T>::Pop (void)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    if (iContainer.empty()) [[unlikely]]
    {
        return std::nullopt;
    }

    value_type e = std::move(iContainer.front());
    iContainer.pop();

    return e;
}

template <typename T>
typename GpSharedQueueSimple<T>::underlying_container&  GpSharedQueueSimple<T>::UnderlyingContainer (void) noexcept
{
    return iContainer;
}

template <typename T>
const typename GpSharedQueueSimple<T>::underlying_container&    GpSharedQueueSimple<T>::UnderlyingContainer (void) const noexcept
{
    return iContainer;
}

}// namespace GPlatform
