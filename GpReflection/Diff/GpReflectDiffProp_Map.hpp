#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include <GpCore2/GpReflection/GpReflectUtils_IsEqual.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_TotalMemoryUse.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropBLOB.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropString.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropObjectSP.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_Map
{
public:
    CLASS_DD(GpReflectDiffProp_Map)

    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

    using KeysVarT = std::variant
    <
        std::vector<u_int_8>,       // GpReflectType::U_INT_8
        std::vector<s_int_8>,       // GpReflectType::S_INT_8
        std::vector<u_int_16>,      // GpReflectType::U_INT_16
        std::vector<s_int_16>,      // GpReflectType::S_INT_16
        std::vector<u_int_32>,      // GpReflectType::U_INT_32
        std::vector<s_int_32>,      // GpReflectType::S_INT_32
        std::vector<u_int_64>,      // GpReflectType::U_INT_64
        std::vector<s_int_64>,      // GpReflectType::S_INT_64
        std::vector<double>,        // GpReflectType::DOUBLE
        std::vector<float>,         // GpReflectType::FLOAT
        std::vector<GpUUID>,        // GpReflectType::UUID
        std::vector<GpBytesArray>,  // GpReflectType::BLOB
        std::vector<std::string>    // GpReflectType::STRING
    >;

    using ValuesDiffVarT = std::variant
    <
        std::vector<u_int_8>,                   // GpReflectType::U_INT_8
        std::vector<s_int_8>,                   // GpReflectType::S_INT_8
        std::vector<u_int_16>,                  // GpReflectType::U_INT_16
        std::vector<s_int_16>,                  // GpReflectType::S_INT_16
        std::vector<u_int_32>,                  // GpReflectType::U_INT_32
        std::vector<s_int_32>,                  // GpReflectType::S_INT_32
        std::vector<u_int_64>,                  // GpReflectType::U_INT_64
        std::vector<s_int_64>,                  // GpReflectType::S_INT_64
        std::vector<double>,                    // GpReflectType::DOUBLE
        std::vector<float>,                     // GpReflectType::FLOAT
        std::vector<GpUUID>,                    // GpReflectType::UUID
        std::vector<GpReflectDiffPropBLOB>,     // GpReflectType::BLOB
        std::vector<GpReflectDiffPropString>,   // GpReflectType::STRING
        std::vector<GpReflectDiffPropObjectSP>  // GpReflectType::OBJECT_SP
    >;

    using ValuesVarT = std::variant
    <
        std::vector<u_int_8>,               // GpReflectType::U_INT_8
        std::vector<s_int_8>,               // GpReflectType::S_INT_8
        std::vector<u_int_16>,              // GpReflectType::U_INT_16
        std::vector<s_int_16>,              // GpReflectType::S_INT_16
        std::vector<u_int_32>,              // GpReflectType::U_INT_32
        std::vector<s_int_32>,              // GpReflectType::S_INT_32
        std::vector<u_int_64>,              // GpReflectType::U_INT_64
        std::vector<s_int_64>,              // GpReflectType::S_INT_64
        std::vector<double>,                // GpReflectType::DOUBLE
        std::vector<float>,                 // GpReflectType::FLOAT
        std::vector<GpUUID>,                // GpReflectType::UUID
        std::vector<GpBytesArray>,          // GpReflectType::BLOB
        std::vector<std::string>,           // GpReflectType::STRING
        std::vector<GpReflectObject::SP>    // GpReflectType::OBJECT_SP
    >;

public:
                            GpReflectDiffProp_Map       (void) noexcept = default;
    inline                  GpReflectDiffProp_Map       (const GpReflectDiffProp_Map& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_Map       (GpReflectDiffProp_Map&& aDiffProp) noexcept;
                            ~GpReflectDiffProp_Map      (void) noexcept;

    template<typename KeyT,
             typename ValT>
    static C::Opt::Val      SCheckAndMake               (const std::map<KeyT, ValT, std::less<>>&   aMapFrom,
                                                         const std::map<KeyT, ValT, std::less<>>&   aMapTo,
                                                         IsEqualCacheRefOptT&                       aIsEqualCache,
                                                         TotalMemoryUseCacheRefOptT&                aTotalMemoryUseCache);

private:
    template<typename T>
    static auto&            SGetKeysFromVar             (KeysVarT& aVar);

    template<typename T>
    static auto&            SGetValuesToUpdateFromVar   (ValuesDiffVarT& aVar);

    template<typename T>
    static auto&            SGetValuesToAddFromVar      (ValuesVarT& aVar);

    template<typename T>
    static auto             SCalcValueDiff              (const T&                       aValFrom,
                                                         const T&                       aValTo,
                                                         IsEqualCacheRefOptT&           aIsEqualCache,
                                                         TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);

    template<typename T>
    static auto             SMakeValueCopy              (const T& aVal);

private:
    // Keys to erase
    KeysVarT        iKeysToErase;

    // Keys to rename
    // TODO: implement

    // [Key,value] to update value
    KeysVarT        iKeysToUpdate;
    ValuesDiffVarT  iValuesToUpdate;

    // [Key,value] to add new
    KeysVarT        iKeysToAdd;
    ValuesVarT      iValuesToAdd;
};

GpReflectDiffProp_Map::GpReflectDiffProp_Map ([[maybe_unused]] const GpReflectDiffProp_Map& aDiffProp) noexcept:
iKeysToErase   (aDiffProp.iKeysToErase   ),
iKeysToUpdate  (aDiffProp.iKeysToUpdate  ),
iValuesToUpdate(aDiffProp.iValuesToUpdate),
iKeysToAdd     (aDiffProp.iKeysToAdd     ),
iValuesToAdd   (aDiffProp.iValuesToAdd   )
{
}

GpReflectDiffProp_Map::GpReflectDiffProp_Map ([[maybe_unused]] GpReflectDiffProp_Map&& aDiffProp) noexcept:
iKeysToErase   (std::move(aDiffProp.iKeysToErase   )),
iKeysToUpdate  (std::move(aDiffProp.iKeysToUpdate  )),
iValuesToUpdate(std::move(aDiffProp.iValuesToUpdate)),
iKeysToAdd     (std::move(aDiffProp.iKeysToAdd     )),
iValuesToAdd   (std::move(aDiffProp.iValuesToAdd   ))
{
}

template<typename KeyT,
         typename ValT>
auto    GpReflectDiffProp_Map::SCheckAndMake
(
    const std::map<KeyT, ValT, std::less<>>&    aMapFrom,
    const std::map<KeyT, ValT, std::less<>>&    aMapTo,
    IsEqualCacheRefOptT&                        aIsEqualCache,
    TotalMemoryUseCacheRefOptT&                 aTotalMemoryUseCache
) -> C::Opt::Val
{
    GpReflectDiffProp_Map res;

    // Keys to erase
    KeysVarT&       keysToEraseVar      = res.iKeysToErase;
    KeysVarT&       keysToUpdateVar     = res.iKeysToUpdate;
    ValuesDiffVarT& valuesToUpdateVar   = res.iValuesToUpdate;
    KeysVarT&       keysToAddVar        = res.iKeysToAdd;
    ValuesVarT&     valuesToAddVar      = res.iValuesToAdd;

    auto& keysToErase       = SGetKeysFromVar<KeyT>(keysToEraseVar);
    auto& keysToUpdate      = SGetKeysFromVar<KeyT>(keysToUpdateVar);
    auto& valuesToUpdate    = SGetValuesToUpdateFromVar<ValT>(valuesToUpdateVar);
    auto& keysToAdd         = SGetKeysFromVar<KeyT>(keysToAddVar);
    auto& valuesToAdd       = SGetValuesToAddFromVar<ValT>(valuesToAddVar);

    for (const auto&[keyFrom, valFrom]: aMapFrom)
    {
        // Check for keyFrom in TO
        const auto iterTo = aMapTo.find(keyFrom);
        if (iterTo == std::end(aMapTo))
        {
            keysToErase.emplace_back(keyFrom);
            continue;
        }

        // Compare values
        const auto& keyTo = iterTo->first;
        const auto& valTo = iterTo->second;

        if (GpReflectUtils_IsEqual::SDo(valFrom, valTo, aIsEqualCache))
        {
            continue;
        }

        // Keys are equal but values are not
        keysToUpdate.emplace_back(keyTo);
        valuesToUpdate.emplace_back
        (
            SCalcValueDiff<ValT>
            (
                valFrom,
                valTo,
                aIsEqualCache,
                aTotalMemoryUseCache
            )
        );
    }

    // New items
    for (const auto&[keyTo, valTo]: aMapTo)
    {
        if (!aMapFrom.contains(keyTo))
        {
            keysToAdd.emplace_back(keyTo);
            valuesToAdd.emplace_back(SMakeValueCopy<ValT>(valTo));
        }
    }

    if (   !keysToErase.empty()
        || !keysToUpdate.empty()
        || !keysToAdd.empty())
    {
        return res;
    } else
    {
        return std::nullopt;
    }
}

template<typename T>
auto&   GpReflectDiffProp_Map::SGetKeysFromVar (KeysVarT& aVar)
{
    if constexpr(std::is_same_v<T, u_int_8>)
    {
        aVar = std::vector<u_int_8>{};
        return std::get<0>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_8>)
    {
        aVar = std::vector<s_int_8>{};
        return std::get<1>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_16>)
    {
        aVar = std::vector<u_int_16>{};
        return std::get<2>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_16>)
    {
        aVar = std::vector<s_int_16>{};
        return std::get<3>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_32>)
    {
        aVar = std::vector<u_int_32>{};
        return std::get<4>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_32>)
    {
        aVar = std::vector<s_int_32>{};
        return std::get<5>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_64>)
    {
        aVar = std::vector<u_int_64>{};
        return std::get<6>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_64>)
    {
        aVar = std::vector<s_int_64>{};
        return std::get<7>(aVar);
    } else if constexpr(std::is_same_v<T, double>)
    {
        aVar = std::vector<double>{};
        return std::get<8>(aVar);
    } else if constexpr(std::is_same_v<T, float>)
    {
        aVar = std::vector<float>{};
        return std::get<9>(aVar);
    } else if constexpr(std::is_same_v<T, GpUUID>)
    {
        aVar = std::vector<GpUUID>{};
        return std::get<10>(aVar);
    } else if constexpr(std::is_same_v<T, GpBytesArray>)
    {
        aVar = std::vector<GpBytesArray>{};
        return std::get<11>(aVar);
    } else if constexpr(std::is_same_v<T, std::string>)
    {
        aVar = std::vector<std::string>{};
        return std::get<12>(aVar);
    } else
    {
        return aVar;
    }
}

template<typename T>
auto&   GpReflectDiffProp_Map::SGetValuesToUpdateFromVar (ValuesDiffVarT& aVar)
{
    if constexpr(std::is_same_v<T, u_int_8>)
    {
        aVar = std::vector<u_int_8>{};
        return std::get<0>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_8>)
    {
        aVar = std::vector<s_int_8>{};
        return std::get<1>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_16>)
    {
        aVar = std::vector<u_int_16>{};
        return std::get<2>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_16>)
    {
        aVar = std::vector<s_int_16>{};
        return std::get<3>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_32>)
    {
        aVar = std::vector<u_int_32>{};
        return std::get<4>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_32>)
    {
        aVar = std::vector<s_int_32>{};
        return std::get<5>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_64>)
    {
        aVar = std::vector<u_int_64>{};
        return std::get<6>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_64>)
    {
        aVar = std::vector<s_int_64>{};
        return std::get<7>(aVar);
    } else if constexpr(std::is_same_v<T, double>)
    {
        aVar = std::vector<double>{};
        return std::get<8>(aVar);
    } else if constexpr(std::is_same_v<T, float>)
    {
        aVar = std::vector<float>{};
        return std::get<9>(aVar);
    } else if constexpr(std::is_same_v<T, GpUUID>)
    {
        aVar = std::vector<GpUUID>{};
        return std::get<10>(aVar);
    } else if constexpr(std::is_same_v<T, GpBytesArray>)
    {
        aVar = std::vector<GpReflectDiffPropBLOB>{};
        return std::get<11>(aVar);
    } else if constexpr(std::is_same_v<T, std::string>)
    {
        aVar = std::vector<GpReflectDiffPropString>{};
        return std::get<12>(aVar);
    } else if constexpr(std::is_same_v<T, GpReflectObject::SP>)
    {
        aVar = std::vector<GpReflectDiffPropObjectSP>{};
        return std::get<13>(aVar);
    } else
    {
        return aVar;
    }
}

template<typename T>
auto&   GpReflectDiffProp_Map::SGetValuesToAddFromVar (ValuesVarT& aVar)
{
    if constexpr(std::is_same_v<T, u_int_8>)
    {
        aVar = std::vector<u_int_8>{};
        return std::get<0>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_8>)
    {
        aVar = std::vector<s_int_8>{};
        return std::get<1>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_16>)
    {
        aVar = std::vector<u_int_16>{};
        return std::get<2>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_16>)
    {
        aVar = std::vector<s_int_16>{};
        return std::get<3>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_32>)
    {
        aVar = std::vector<u_int_32>{};
        return std::get<4>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_32>)
    {
        aVar = std::vector<s_int_32>{};
        return std::get<5>(aVar);
    } else if constexpr(std::is_same_v<T, u_int_64>)
    {
        aVar = std::vector<u_int_64>{};
        return std::get<6>(aVar);
    } else if constexpr(std::is_same_v<T, s_int_64>)
    {
        aVar = std::vector<s_int_64>{};
        return std::get<7>(aVar);
    } else if constexpr(std::is_same_v<T, double>)
    {
        aVar = std::vector<double>{};
        return std::get<8>(aVar);
    } else if constexpr(std::is_same_v<T, float>)
    {
        aVar = std::vector<float>{};
        return std::get<9>(aVar);
    } else if constexpr(std::is_same_v<T, GpUUID>)
    {
        aVar = std::vector<GpUUID>{};
        return std::get<10>(aVar);
    } else if constexpr(std::is_same_v<T, GpBytesArray>)
    {
        aVar = std::vector<GpBytesArray>{};
        return std::get<11>(aVar);
    } else if constexpr(std::is_same_v<T, std::string>)
    {
        aVar = std::vector<std::string>{};
        return std::get<12>(aVar);
    } else if constexpr(std::is_same_v<T, GpReflectObject::SP>)
    {
        aVar = std::vector<GpReflectObject::SP>{};
        return std::get<13>(aVar);
    } else
    {
        return aVar;
    }
}

template<typename T>
auto    GpReflectDiffProp_Map::SCalcValueDiff
(
    const T&                                        aValFrom,
    const T&                                        aValTo,
    [[maybe_unused]] IsEqualCacheRefOptT&           aIsEqualCache,
    [[maybe_unused]] TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache
)
{
    if constexpr(std::is_same_v<T, u_int_8>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, s_int_8>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, u_int_16>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, s_int_16>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, u_int_32>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, s_int_32>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, u_int_64>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, s_int_64>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, double>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, float>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, GpUUID>)
    {
        return aValTo;
    } else if constexpr(std::is_same_v<T, GpBytesArray>)
    {
        return GpReflectDiffPropBLOB::SCheckAndMake(aValFrom, aValTo).value();
    } else if constexpr(std::is_same_v<T, std::string>)
    {
        return GpReflectDiffPropString::SCheckAndMake(aValFrom, aValTo).value();
    } else if constexpr(std::is_same_v<T, GpReflectObject::SP>)
    {
        return GpReflectDiffPropObjectSP::SCheckAndMake(aValFrom, aValTo, aIsEqualCache, aTotalMemoryUseCache).value();
    } else
    {
        return std::nullopt;
    }
}

template<typename T>
auto    GpReflectDiffProp_Map::SMakeValueCopy (const T& aVal)
{
    if constexpr(std::is_same_v<T, u_int_8>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, s_int_8>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, u_int_16>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, s_int_16>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, u_int_32>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, s_int_32>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, u_int_64>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, s_int_64>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, double>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, float>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, GpUUID>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, GpBytesArray>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, std::string>)
    {
        return aVal;
    } else if constexpr(std::is_same_v<T, GpReflectObject::SP>)
    {
        if (aVal.IsNotNULL())
        {
            return aVal.Vn().ReflectClone();
        } else
        {
            return GpReflectObject::SP::SNull();
        }
    } else
    {
        return nullptr;
    }
}

}// namespace GPlatform
