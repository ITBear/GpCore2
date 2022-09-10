#include "GpEnum.hpp"
#include "../Strings/GpStringOps.hpp"
#include "../../Algorithms/GpSplit.hpp"

#if defined(GP_USE_ENUMS)

namespace GPlatform {

void    GpEnum::FromID (value_type aId)
{
    for (const auto& item: Names())
    {
        if (aId == std::get<1>(item))
        {
            _SetID(aId);
            return;
        }
    }

    THROW_GP("Unknown Enum ID "_sv + aId);
}

std::string_view    GpEnum::ToString (void) const noexcept
{
    return _SToString(Names(), ID());
}

void    GpEnum::FromString (std::string_view aName)
{
    _SetID(_SFromString(Names(), aName, TypeName()));
}

std::string_view    GpEnum::_SToString
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

    return std::string_view();
}

GpEnum::value_type  GpEnum::_SFromString
(
    const NamesListT&   aNamesList,
    std::string_view    aName,
    std::string_view    aEnumTypeName
)
{
    for (auto&& [name, id]: aNamesList)
    {
        if (aName == name)
        {
            return id;
        }
    }

    THROW_GP("Failed to set enum ("_sv + aEnumTypeName + ") value from string '"_sv + aName + "'"_sv);
}

GpEnum::NamesListT  GpEnum::_SParseEnumElements
(
    std::string_view aEnumName,
    std::string_view aEnumElementsStr
)
{
    GpVector<GpTuple<std::string_view, value_type>> res;

    _SParseEnumValues(res, aEnumName, aEnumElementsStr);

    return res;
}

void    GpEnum::_SParseEnumValues
(
    NamesListT&         aNamesListOut,
    std::string_view    aEnumName,
    std::string_view    aEnumElementsStr
)
{
    //Split by ','
    const GpVector<std::string_view> elements = StrOps::SSplit
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

    for (std::string_view element: elements)
    {
        //Split by ' '
        const GpVector<std::string_view> parts = StrOps::SSplit
        (
            element,
            ' ',
            0,
            0,
            Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS
        );

        std::string_view    name;
        std::string_view    id_str;
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
            THROW_GP("Wrong enum '"_sv + aEnumName + "' element: "_sv + parts.at(0));
        }

        lastId = ssize_t(id);
        aNamesListOut.emplace_back(name, id);
    }
}

}//GPlatform

#endif//GP_USE_ENUMS
