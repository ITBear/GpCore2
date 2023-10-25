#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_ENUMS)

#include "GpEnumFlags.hpp"
#include <bit>

namespace GPlatform {

class GP_UTILS_API GpEnum
{
public:
    CLASS_REMOVE_CTRS_MOVE(GpEnum)
    CLASS_DD(GpEnum)
    TAG_SET(GpEnum)

    using value_type    = s_int_32;
    using NamesListT    = std::vector<std::tuple<std::u8string_view, value_type>>;

protected:
                                GpEnum              (void) noexcept:iId(value_type()) {}
                                GpEnum              (value_type aId) noexcept:iId(aId) {}
                                GpEnum              (const GpEnum& aEnum) noexcept:iId(aEnum.iId) {}

public:
    virtual                     ~GpEnum             (void) noexcept = default;

protected:
    void                        _SetID              (value_type aId) noexcept {iId = aId;}

public:
    value_type                  ID_at               (const size_t aPos) const {return std::get<1>(Names().at(aPos));}
    value_type                  ID                  (void) const noexcept {return iId;}
    void                        FromID              (const value_type aId);
    void                        FromNumPos          (const size_t aPos);
    std::u8string_view          ToString            (void) const noexcept;
    void                        FromString          (std::u8string_view aName);
    virtual const NamesListT&   Names               (void) const noexcept = 0;
    virtual std::u8string_view  TypeName            (void) const noexcept = 0;

    bool                        operator<           (const GpEnum& aEnum) const noexcept {return iId < aEnum.iId;}
    bool                        operator>           (const GpEnum& aEnum) const noexcept {return iId > aEnum.iId;}
    bool                        operator<=          (const GpEnum& aEnum) const noexcept {return iId <= aEnum.iId;}
    bool                        operator>=          (const GpEnum& aEnum) const noexcept {return iId >= aEnum.iId;}
    bool                        operator==          (const GpEnum& aEnum) const noexcept {return iId == aEnum.iId;}
    bool                        operator!=          (const GpEnum& aEnum) const noexcept {return iId != aEnum.iId;}

protected:
    static std::u8string_view   _SToString          (const NamesListT&  aNamesList,
                                                     const value_type   aId) noexcept;
    static value_type           _SFromString        (const NamesListT&  aNamesList,
                                                     std::u8string_view aName,
                                                     std::u8string_view aEnumTypeName);
    static NamesListT           _SParseEnumElements (std::u8string_view aEnumName,
                                                     std::u8string_view aEnumElementsStr);

private:
    static void                 _SParseEnumValues   (NamesListT&        aNamesListOut,
                                                     std::u8string_view aEnumName,
                                                     std::u8string_view aEnumElementsStr);
private:
    value_type                  iId;
};

#define GP_ENUM(PREFIX, TYPE_NAME, ...) \
class PREFIX TYPE_NAME final: public GpEnum \
{ \
public: \
    using EnumFlagsT = GpEnumFlagsST<TYPE_NAME>; \
\
public: \
    CLASS_DD(TYPE_NAME) \
\
    enum EnumT: value_type \
    { \
        __VA_ARGS__ \
    }; \
\
    TYPE_NAME (void) noexcept: \
    GpEnum(value_type(EnumT())) \
    { \
    } \
\
    TYPE_NAME (const TYPE_NAME& aTypeName) noexcept:\
    GpEnum(aTypeName)\
    {\
    }\
\
    TYPE_NAME (const TYPE_NAME::EnumT aEnumValue) noexcept:\
    GpEnum(value_type(aEnumValue))\
    {\
    }\
\
    virtual ~TYPE_NAME (void) noexcept override final {}\
\
    static TYPE_NAME SFromID (const value_type aId) {TYPE_NAME e; e.FromID(aId); return e;} \
\
    static std::u8string_view SEnumValuesStr (void)\
    {\
        static const std::u8string s = std::u8string(GpUTF::S_STR_To_UTF8(#__VA_ARGS__));\
        return s;\
    }\
\
    static constexpr size_t SCount (void) noexcept\
    {\
        const constexpr size_t cnt = StrOps::SCharsCount(#__VA_ARGS__, ',') + 1; \
        return cnt;\
    }\
\
    constexpr size_t Count (void) const noexcept\
    {\
        return SCount();\
    }\
\
    EnumT Value (void) const noexcept\
    {\
        return EnumT(ID());\
    }\
\
    void SetValue (EnumT aEnumValue) noexcept\
    {\
        return _SetID(value_type(aEnumValue));\
    }\
\
    static std::u8string_view SToString (EnumT aEnumValue) noexcept; \
\
    static EnumT SFromString (std::u8string_view aName);\
\
    virtual const NamesListT&   Names       (void) const noexcept override final;\
    virtual std::u8string_view  TypeName    (void) const noexcept override final;\
\
    bool operator < (const TYPE_NAME& aTypeName) const noexcept     {return ID() < aTypeName.ID();}\
    bool operator < (const TYPE_NAME::EnumT aValue) const noexcept  {return ID() < value_type(aValue);}\
    bool operator > (const TYPE_NAME& aTypeName) const noexcept     {return ID() > aTypeName.ID();}\
    bool operator > (const TYPE_NAME::EnumT aValue) const noexcept  {return ID() > value_type(aValue);}\
    bool operator <=(const TYPE_NAME& aTypeName) const noexcept     {return ID() <= aTypeName.ID();}\
    bool operator <=(const TYPE_NAME::EnumT aValue) const noexcept  {return ID() <= value_type(aValue);}\
    bool operator >=(const TYPE_NAME& aTypeName) const noexcept     {return ID() >= aTypeName.ID();}\
    bool operator >=(const TYPE_NAME::EnumT aValue) const noexcept  {return ID() >= value_type(aValue);}\
    bool operator ==(const TYPE_NAME& aTypeName) const noexcept     {return ID() == aTypeName.ID();}\
    bool operator ==(const TYPE_NAME::EnumT aValue) const noexcept  {return ID() == value_type(aValue);}\
    bool operator !=(const TYPE_NAME& aTypeName) const noexcept     {return ID() != aTypeName.ID();}\
    bool operator !=(const TYPE_NAME::EnumT aValue) const noexcept  {return ID() != value_type(aValue);}\
    TYPE_NAME& operator = (const TYPE_NAME& aTypeName) noexcept     {_SetID(aTypeName.ID()); return *this;}\
    TYPE_NAME& operator = (const TYPE_NAME::EnumT aValue) noexcept  {_SetID(value_type(aValue)); return *this;}\
\
    friend EnumFlagsT operator| (const EnumT aFlagLeft, const EnumT aFlagRight) noexcept \
    { \
        return EnumFlagsT(value_type(value_type(1) << aFlagLeft) | value_type(value_type(1) << aFlagRight)); \
    } \
\
    static const NamesListT&    SNames      (void) noexcept;\
    static value_type           SID_at      (const size_t aPos) {return std::get<1>(SNames().at(aPos));} \
    static std::u8string_view   STypeName   (void) noexcept;\
}

#define GP_ENUM_IMPL(TYPE_NAME, ...) \
 \
const GpEnum::NamesListT& TYPE_NAME::SNames (void) noexcept \
{ \
    static NamesListT sNamesList = TYPE_NAME::_SParseEnumElements \
    ( \
        GpUTF::S_STR_To_UTF8(#TYPE_NAME), \
        TYPE_NAME::SEnumValuesStr() \
    ); \
    return sNamesList; \
} \
 \
std::u8string_view  TYPE_NAME::STypeName (void) noexcept \
{ \
    static std::u8string sTypeName(GpUTF::S_STR_To_UTF8(#TYPE_NAME)); \
    return sTypeName; \
} \
\
std::u8string_view  TYPE_NAME::SToString (EnumT aEnumValue) noexcept\
{\
    return _SToString(SNames(), value_type(aEnumValue));\
}\
\
TYPE_NAME::EnumT    TYPE_NAME::SFromString (std::u8string_view aName)\
{\
    return EnumT(_SFromString(SNames(), aName, GpUTF::S_STR_To_UTF8(#TYPE_NAME)));\
}\
\
const TYPE_NAME::NamesListT& TYPE_NAME::Names (void) const noexcept\
{\
    return SNames();\
}\
\
std::u8string_view  TYPE_NAME::TypeName (void) const noexcept\
{\
    return STypeName();\
}

}//GPlatform

//********************** Hash *********************
namespace std {

template<> struct hash<::GPlatform::GpEnum>
{
    using enum_type             = ::GPlatform::GpEnum;
    using value_type            = enum_type::value_type;
    using value_type_unsigned   = std::make_unsigned_t<value_type>;
    using result_type           = std::size_t;

    result_type operator()(enum_type const& aArg) const noexcept
    {
        static_assert(sizeof(result_type) >= sizeof(value_type), "");
        const value_type_unsigned tmpVal = std::bit_cast<value_type_unsigned>(aArg.ID());
        return result_type(tmpVal);
    }
};

inline u8string to_string(const ::GPlatform::GpEnum& aEnum)
{
    return u8string(aEnum.ToString());
}

}//std

#endif//GP_USE_ENUMS
