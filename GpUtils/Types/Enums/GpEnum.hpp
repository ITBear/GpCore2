#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_ENUMS)

#include <GpCore2/GpUtils/Types/Enums/GpEnumFlags.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringLiterals.hpp>

#include <bit>
#include <initializer_list>

namespace GPlatform {

class GP_UTILS_API GpEnum
{
public:
    CLASS_REMOVE_CTRS_MOVE(GpEnum)
    CLASS_DD(GpEnum)
    TAG_SET(GpEnum)

    using value_type    = typename GpEnumFlags::value_type;
    using NamesListT    = typename GpEnumFlags::NamesListT;

protected:
                                GpEnum              (void) noexcept:iId(value_type()) {}
    explicit                    GpEnum              (value_type aId) noexcept:iId(aId) {}
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
    std::string_view            ToString            (void) const noexcept;
    void                        FromString          (std::string_view aName);
    virtual const NamesListT&   Names               (void) const noexcept = 0;
    virtual std::string_view    TypeName            (void) const noexcept = 0;

    bool                        operator<           (const GpEnum& aEnum) const noexcept {return iId < aEnum.iId;}
    bool                        operator>           (const GpEnum& aEnum) const noexcept {return iId > aEnum.iId;}
    bool                        operator<=          (const GpEnum& aEnum) const noexcept {return iId <= aEnum.iId;}
    bool                        operator>=          (const GpEnum& aEnum) const noexcept {return iId >= aEnum.iId;}
    bool                        operator==          (const GpEnum& aEnum) const noexcept {return iId == aEnum.iId;}
    bool                        operator!=          (const GpEnum& aEnum) const noexcept {return iId != aEnum.iId;}

protected:
    static std::string_view     _SToString          (const NamesListT&  aNamesList,
                                                     const value_type   aId) noexcept;
    static value_type           _SFromString        (const NamesListT&  aNamesList,
                                                     std::string_view   aName,
                                                     std::string_view   aEnumTypeName);
    static NamesListT           _SParseEnumElements (std::string_view aEnumName,
                                                     std::string_view aEnumElementsStr);

private:
    static void                 _SParseEnumValues   (NamesListT&        aNamesListOut,
                                                     std::string_view   aEnumName,
                                                     std::string_view   aEnumElementsStr);
private:
    value_type                  iId;
};

#define GP_ENUM(PREFIX, TYPE_NAME, ...) \
class PREFIX TYPE_NAME final: public GpEnum \
{ \
public: \
    CLASS_DD(TYPE_NAME) \
\
    enum EnumT: value_type \
    { \
        __VA_ARGS__ \
    }; \
\
public: \
    using EnumFlagsT = GpEnumFlagsST<TYPE_NAME>; \
\
    TYPE_NAME (void) noexcept: \
    GpEnum(value_type(EnumT())) \
    { \
    } \
\
    TYPE_NAME (const TYPE_NAME& aTypeName) noexcept: \
    GpEnum(aTypeName) \
    { \
    } \
\
    TYPE_NAME (const TYPE_NAME::EnumT aEnumValue) noexcept: \
    GpEnum(value_type(aEnumValue)) \
    { \
    } \
\
    virtual ~TYPE_NAME (void) noexcept override final {} \
\
    static TYPE_NAME SFromID (const value_type aId) {TYPE_NAME e; e.FromID(aId); return e;} \
\
    static std::string_view SEnumValuesStr (void) \
    {\
        static const std::string s = std::string(#__VA_ARGS__); \
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
    operator EnumT() const noexcept { return Value(); } \
\
    static std::string_view SToString (EnumT aEnumValue) noexcept; \
\
    static EnumT SFromString (std::string_view aName);\
\
    virtual const NamesListT&   Names       (void) const noexcept override final;\
    virtual std::string_view    TypeName    (void) const noexcept override final;\
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
    static std::string_view     STypeName   (void) noexcept;\
}

#define GP_ENUM_IMPL(TYPE_NAME, ...) \
 \
const GpEnum::NamesListT& TYPE_NAME::SNames (void) noexcept \
{ \
    static NamesListT sNamesList = TYPE_NAME::_SParseEnumElements \
    ( \
        #TYPE_NAME, \
        TYPE_NAME::SEnumValuesStr() \
    ); \
    return sNamesList; \
} \
 \
std::string_view    TYPE_NAME::STypeName (void) noexcept \
{ \
    static std::string sTypeName(#TYPE_NAME); \
    return sTypeName; \
} \
\
std::string_view    TYPE_NAME::SToString (EnumT aEnumValue) noexcept\
{\
    return _SToString(SNames(), value_type(aEnumValue));\
}\
\
TYPE_NAME::EnumT    TYPE_NAME::SFromString (std::string_view aName)\
{\
    return EnumT(_SFromString(SNames(), aName, #TYPE_NAME));\
}\
\
const TYPE_NAME::NamesListT& TYPE_NAME::Names (void) const noexcept\
{\
    return SNames();\
}\
\
std::string_view    TYPE_NAME::TypeName (void) const noexcept\
{\
    return STypeName();\
}

}// namespace GPlatform

//********************** fmt *********************
namespace FMT_NAMESPASE {

template<typename T>
requires ::GPlatform::EnumConcepts::IsEnum<T>
struct formatter<T>
{
    using enum_type = T;

    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(const enum_type& aEnum, FormatContext& aCtx) const
    {
        return ::fmt::format_to(aCtx.out(), "{}", aEnum.ToString());
    }
};

}// namespace std

//********************** Hash *********************
namespace std {

template<>
struct hash<::GPlatform::GpEnum>
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

inline string to_string(const ::GPlatform::GpEnum& aEnum)
{
    return string(aEnum.ToString());
}

}// namespace std

#endif// GP_USE_ENUMS
