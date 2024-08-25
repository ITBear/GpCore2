#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>

#if defined(GP_USE_ENUMS)

namespace GPlatform {

void    GpEnum::FromID (const value_type aId)
{
    for (const auto& item: Names())
    {
        if (aId == std::get<1>(item)) [[unlikely]]
        {
            _SetID(aId);
            return;
        }
    }

    THROW_GP
    (
        fmt::format
        (
            "Unknown Enum ID {}",
            aId
        )
    );
}

void    GpEnum::FromNumPos (const size_t aPos)
{
    iId = std::get<1>(Names().at(aPos));
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

    THROW_GP
    (
        fmt::format
        (
            "Failed to set enum ({}) value from string '{}'",
            aEnumTypeName,
            aName
        )
    );
}

GpEnum::NamesListT  GpEnum::_SParseEnumElements
(
    std::string_view aEnumName,
    std::string_view aEnumElementsStr
)
{
    std::vector<std::tuple<std::string_view, value_type>> res;

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
    // Split by ','
    const std::vector<std::string_view> elements = StrOps::SSplit
    (
        aEnumElementsStr,
        ',',
        0,
        0,
        Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS
    );

    // Split each element (may be 'VALUE' or 'VALUE = ID')
    ssize_t lastId = -1;

    aNamesListOut.reserve(std::size(elements));

    for (std::string_view element: elements)
    {
        // Split by ' '
        const std::vector<std::string_view> parts = StrOps::SSplit
        (
            element,
            ' ',
            0,
            0,
            Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS
        );

        std::string_view    name;
        std::string_view    id_str;
        value_type          id          = value_type(lastId + 1);
        const size_t        partsSize   = std::size(parts);

        if (partsSize == 1)
        {
            name    = parts.at(0);
        } else if (partsSize == 3)
        {
            name    = parts.at(0);
            id_str  = parts.at(2);
            id      = NumOps::SConvert<value_type>(StrOps::SToUI64(id_str));
        } else
        {
            THROW_GP
            (
                fmt::format
                (
                    "Wrong enum '{}' element: {}",
                    aEnumName,
                    parts.at(0)
                )
            );
        }

        lastId = ssize_t{id};
        aNamesListOut.emplace_back(name, id);
    }
}

}// namespace GPlatform

#endif// GP_USE_ENUMS
