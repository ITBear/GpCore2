#include "GpEnum.hpp"
#include "../Strings/GpStringOps.hpp"
#include "../Strings/GpStringLiterals.hpp"

#if defined(GP_USE_ENUMS)

namespace GPlatform {

void    GpEnum::FromID (const value_type aId)
{
    for (const auto& item: Names())
    {
        if (aId == std::get<1>(item))
        {
            _SetID(aId);
            return;
        }
    }

    THROW_GP(u8"Unknown Enum ID "_sv + aId);
}

void    GpEnum::FromNumPos (const size_t aPos)
{
    iId = std::get<1>(Names().at(aPos));
}

std::u8string_view  GpEnum::ToString (void) const noexcept
{
    return _SToString(Names(), ID());
}

void    GpEnum::FromString (std::u8string_view aName)
{
    _SetID(_SFromString(Names(), aName, TypeName()));
}

std::u8string_view  GpEnum::_SToString
(
    const NamesListT&   aNamesList,
    const value_type    aId
) noexcept
{
    for (const auto&[name, id]: aNamesList)
    {
        if (id == aId)
        {
            return name;
        }
    }

    return std::u8string_view();
}

GpEnum::value_type  GpEnum::_SFromString
(
    const NamesListT&   aNamesList,
    std::u8string_view  aName,
    std::u8string_view  aEnumTypeName
)
{
    for (auto&& [name, id]: aNamesList)
    {
        if (aName == name)
        {
            return id;
        }
    }

    THROW_GP(u8"Failed to set enum ("_sv + aEnumTypeName + u8") value from string '"_sv + aName + u8"'"_sv);
}

GpEnum::NamesListT  GpEnum::_SParseEnumElements
(
    std::u8string_view aEnumName,
    std::u8string_view aEnumElementsStr
)
{
    std::vector<std::tuple<std::u8string_view, value_type>> res;

    _SParseEnumValues(res, aEnumName, aEnumElementsStr);

    return res;
}

void    GpEnum::_SParseEnumValues
(
    NamesListT&         aNamesListOut,
    std::u8string_view  aEnumName,
    std::u8string_view  aEnumElementsStr
)
{
    //Split by ','
    const std::vector<std::u8string_view> elements = StrOps::SSplit
    (
        aEnumElementsStr,
        ',',
        0,
        0,
        Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS
    );

    //Split each element (may be 'VALUE' or 'VALUE = ID')
    ssize_t lastId = -1;

    aNamesListOut.reserve(elements.size());

    for (std::u8string_view element: elements)
    {
        //Split by ' '
        const std::vector<std::u8string_view> parts = StrOps::SSplit
        (
            element,
            ' ',
            0,
            0,
            Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS
        );

        std::u8string_view  name;
        std::u8string_view  id_str;
        value_type          id  = value_type(lastId + 1);

        if (parts.size() == 1)
        {
            name    = parts.at(0);
        } else if (parts.size() == 3)
        {
            name    = parts.at(0);
            id_str  = parts.at(2);
            id      = NumOps::SConvert<value_type>(StrOps::SToUI64(id_str));
        } else
        {
            THROW_GP(u8"Wrong enum '"_sv + aEnumName + u8"' element: "_sv + parts.at(0));
        }

        lastId = ssize_t(id);
        aNamesListOut.emplace_back(name, id);
    }
}

}//GPlatform

#endif//GP_USE_ENUMS
