#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Strings/GpStringOps.hpp"
#include "GpContainersT.hpp"

namespace GPlatform {

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
class GpFlatMap
{
public:
    using KeyContainerT = std::array<KeyT, MaxSizeV>;
    using ValContainerT = std::array<ValueT, MaxSizeV>;
    using this_type     = GpFlatMap<KeyT, ValueT, MaxSizeV>;
    using key_type      = KeyT;
    using value_type    = ValueT;
    using ValueRefOptT  = std::optional<std::reference_wrapper<ValueT>>;
    using ValueCRefOptT = std::optional<std::reference_wrapper<const ValueT>>;

    template<typename T>
    struct IsReferenceWrapper: std::false_type {};

    template<typename T>
    struct IsReferenceWrapper<std::reference_wrapper<T>> : std::true_type{};

public:
                        GpFlatMap               (void) noexcept;
                        GpFlatMap               (const GpFlatMap& aFlatMap);
                        GpFlatMap               (GpFlatMap&& aFlatMap) noexcept;
                        ~GpFlatMap              (void) noexcept;

    void                Clear                   (void) noexcept;

    void                Insert                  (const KeyT&    aKey,
                                                 ValueT&&       aValue);
    void                Insert                  (const KeyT&    aKey,
                                                 ValueT         aValue);
    void                Update                  (const KeyT&    aKey,
                                                 ValueT&&       aValue);
    void                Update                  (const KeyT&    aKey,
                                                 ValueT         aValue);

    const ValueT&       Find                    (const KeyT&    aKey) const;
    ValueT&             Find                    (const KeyT&    aKey);
    ValueCRefOptT       FindOpt                 (const KeyT&    aKey) const noexcept;
    ValueRefOptT        FindOpt                 (const KeyT&    aKey) noexcept;
    ValueT              FindCopy                (const KeyT&    aKey) const;
    ValueT              FindCopyOrDefault       (const KeyT&    aKey) const noexcept;

    ValueT              Remove                  (const KeyT&    aKey);

private:
    KeyContainerT       iKeys;
    ValContainerT       iValues;
};

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
GpFlatMap<KeyT, ValueT, MaxSizeV>::GpFlatMap (void) noexcept
{
    Clear();
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
GpFlatMap<KeyT, ValueT, MaxSizeV>::GpFlatMap (const GpFlatMap& aFlatMap):
iKeys(aFlatMap.iKeys),
iValues(aFlatMap.iValues)
{
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
GpFlatMap<KeyT, ValueT, MaxSizeV>::GpFlatMap (GpFlatMap&& aFlatMap) noexcept:
iKeys(std::move(aFlatMap.iKeys)),
iValues(std::move(aFlatMap.iValues))
{
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
GpFlatMap<KeyT, ValueT, MaxSizeV>::~GpFlatMap (void) noexcept
{
    Clear();
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
void    GpFlatMap<KeyT, ValueT, MaxSizeV>::Clear (void) noexcept
{
    iKeys.fill(KeyT());
    iValues.fill(ValueT());
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
void    GpFlatMap<KeyT, ValueT, MaxSizeV>::Insert
(
    const KeyT& aKey,
    ValueT&&    aValue
)
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        KeyT& key = iKeys[id];

        if (key == KeyT())
        {
            key = aKey;
            iValues[id] = std::forward(aValue);
            return;
        }
    }

    THROW_GP(u8"Out of rane"_sv);
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
void    GpFlatMap<KeyT, ValueT, MaxSizeV>::Insert
(
    const KeyT& aKey,
    ValueT      aValue
)
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        KeyT& key = iKeys[id];

        if (key == KeyT())
        {
            key = aKey;
            iValues[id] = aValue;
            return;
        }
    }

    THROW_GP(u8"Out of rane"_sv);
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
void    GpFlatMap<KeyT, ValueT, MaxSizeV>::Update
(
    const KeyT& aKey,
    ValueT&&    aValue
)
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        if (iKeys[id] == aKey)
        {
            iValues[id] = std::forward(aValue);
            return;
        }
    }

    THROW_GP(u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv);
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
void    GpFlatMap<KeyT, ValueT, MaxSizeV>::Update
(
    const KeyT& aKey,
    ValueT      aValue
)
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        if (iKeys[id] == aKey)
        {
            iValues[id] = std::move(aValue);
            return;
        }
    }

    THROW_GP(u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv);
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
const ValueT&   GpFlatMap<KeyT, ValueT, MaxSizeV>::Find (const KeyT& aKey) const
{
    return const_cast<this_type&>(*this).Find(aKey);
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
ValueT& GpFlatMap<KeyT, ValueT, MaxSizeV>::Find (const KeyT& aKey)
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        if (iKeys[id] == aKey)
        {
            return iValues[id];
        }
    }

    THROW_GP(u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv);
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
auto    GpFlatMap<KeyT, ValueT, MaxSizeV>::FindOpt (const KeyT& aKey) const noexcept -> ValueCRefOptT
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        if (iKeys[id] != aKey)
        {
            continue;
        }

        if constexpr (IsReferenceWrapper<ValueT>::value)
        {
            return iValues[id];
        } else
        {
            return std::cref(iValues[id]);
        }
    }

    return std::nullopt;
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
auto    GpFlatMap<KeyT, ValueT, MaxSizeV>::FindOpt (const KeyT& aKey) noexcept -> ValueRefOptT
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        if (iKeys[id] != aKey)
        {
            continue;
        }

        if constexpr (IsReferenceWrapper<ValueT>::value)
        {
            return iValues[id];
        } else
        {
            return std::ref(iValues[id]);
        }
    }

    return std::nullopt;
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
ValueT  GpFlatMap<KeyT, ValueT, MaxSizeV>::FindCopy (const KeyT& aKey) const
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        if (iKeys[id] == aKey)
        {
            return iValues[id];
        }
    }

    THROW_GP(u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv);
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
ValueT  GpFlatMap<KeyT, ValueT, MaxSizeV>::FindCopyOrDefault (const KeyT& aKey) const noexcept
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        if (iKeys[id] == aKey)
        {
            return iValues[id];
        }
    }

    return ValueT();
}

template<typename   KeyT,
         typename   ValueT,
         size_t     MaxSizeV>
ValueT  GpFlatMap<KeyT, ValueT, MaxSizeV>::Remove (const KeyT& aKey)
{
    for (size_t id = 0; id < MaxSizeV; id++)
    {
        KeyT& key = iKeys[id];

        if (key == aKey)
        {
            key = KeyT();
            ValueT v = std::move(iValues[id]);
            iValues[id] = ValueT();
            return v;
        }
    }

    THROW_GP(u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv);
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
