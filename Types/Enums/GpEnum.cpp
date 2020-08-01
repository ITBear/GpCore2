#include "GpEnum.hpp"

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

    THROW_GPE("Unknown Enum ID "_sv + aId);
}

std::string_view    GpEnum::ToString (void) const noexcept
{
    return _SToString(Names(), ID());
}

void    GpEnum::FromString (std::string_view aName)
{
    _SetID(_SFromString(Names(), aName, TypeName()));
}

std::string_view    GpEnum::_SToString (const NamesListT&   aNamesList,
                                        const value_type    aId) noexcept
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

GpEnum::value_type  GpEnum::_SFromString (const NamesListT& aNamesList,
                                          std::string_view  aName,
                                          std::string_view  aEnumTypeName)
{
    for (auto&& [name, id]: aNamesList)
    {
        if (aName == name)
        {
            return id;
        }
    }

    THROW_GPE("Failed to set enum ("_sv + aEnumTypeName + ") value from string '"_sv + aName + "'"_sv);
}

GpEnum::NamesListT  GpEnum::_SParseEnumElements (std::string_view aEnumName,
                                                 std::string_view aEnumElementsStr)
{
    GpVector<GpTuple<std::string_view, value_type>> res;

    _SParseEnumValues(res, aEnumName, aEnumElementsStr);

    return res;
}

void    GpEnum::_SParseEnumValues (NamesListT&      aNamesListOut,
                                   std::string_view aEnumName,
                                   std::string_view aEnumElementsStr)
{
    //Split by ','
    const GpVector<GpRawPtrCharR> elements = GpStringOps::SSplit(aEnumElementsStr,
                                                                 ',',
                                                                 0_cnt,
                                                                 0_cnt,
                                                                 Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS);

    //Split each element (may be 'VALUE' or 'VALUE = ID')
    ssize_t lastId = -1;

    aNamesListOut.reserve(elements.size());

    for (const GpRawPtrCharR& element: elements)
    {
        //Split by ' '
        const GpVector<GpRawPtrCharR> parts = GpStringOps::SSplit(element,
                                                                  ' ',
                                                                  0_cnt,
                                                                  0_cnt,
                                                                  Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS);

        GpRawPtrCharR   name;
        GpRawPtrCharR   id_str;
        value_type      id  = value_type(lastId + 1);

        if (parts.size() == 1)
        {
            name    = parts.at(0);
        } else if (parts.size() == 3)
        {
            name    = parts.at(0);
            id_str  = parts.at(2);
            id      = GpStringOps::SToUI64(id_str).ValueAs<value_type>();
        } else
        {
            THROW_GPE("Wrong enum '"_sv + aEnumName + "' element: "_sv + parts.at(0).AsStringView());
        }

        lastId = ssize_t(id);
        aNamesListOut.emplace_back(name.AsStringView(), id);
    }
}

}//GPlatform

#endif//GP_USE_ENUMS
