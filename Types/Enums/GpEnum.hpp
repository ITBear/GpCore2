#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_ENUMS)

#include "../Classes/GpClassesDefines.hpp"
#include "../Numerics/GpNumerics.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Containers/GpRawPtrByte.hpp"
#include "../Pointers/GpSharedPtr.hpp"

namespace GPlatform {

class GPCORE_API GpEnum
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT_COPY(GpEnum)
    CLASS_DECLARE_DEFAULTS(GpEnum)
    CLASS_TAG(GpEnum)
    CLASS_TAG_DETECTOR(GpEnum)

    using value_type    = size_t;
    using NamesListT    = GpVector<GpTuple<std::string_view, value_type>>;

protected:
                                GpEnum              (void) noexcept:iId(value_type()) {}
                                GpEnum              (value_type aId) noexcept:iId(aId) {}
                                GpEnum              (const GpEnum& aEnum) noexcept:iId(aEnum.iId) {}

public:
    virtual                     ~GpEnum             (void) noexcept = default;

protected:
    void                        _SetID              (value_type aId) noexcept {iId = aId;}

public:
    value_type                  ID                  (void) const noexcept {return iId;}
    void                        FromID              (value_type aId);
    std::string_view            ToString            (void) const noexcept;
    void                        FromString          (GpRawPtrCharR aName);
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
                                                     GpRawPtrCharR      aName,
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
    CLASS_DECLARE_DEFAULTS(TYPE_NAME) \
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
    static std::string_view SEnumValuesStr (void)\
    {\
        static const std::string s(#__VA_ARGS__);\
        return s;\
    }\
\
    static constexpr count_t SCount (void) noexcept\
    {\
        constexpr count_t cnt = StrOps::SCountChars(#__VA_ARGS__, ',') + 1_cnt; \
        return cnt;\
    }\
\
    constexpr count_t Count (void) const noexcept\
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
private: \
    static const NamesListT&    SNames      (void) noexcept;\
    static std::string_view     STypeName   (void) noexcept;\
}

#define GP_ENUM_IMPL(TYPE_NAME, ...) \
 \
const GpEnum::NamesListT& TYPE_NAME::SNames (void) noexcept \
{ \
    static NamesListT sNamesList = TYPE_NAME::_SParseEnumElements(#TYPE_NAME, TYPE_NAME::SEnumValuesStr()); \
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

}//GPlatform

//********************** Hash *********************
namespace std {

template<> struct hash<::GPlatform::GpEnum>
{
    using argument_type = ::GPlatform::GpEnum;
    using result_type   = std::size_t;

    result_type operator()(argument_type const& aArg) const noexcept
    {
        return result_type(aArg.ID());
    }
};

inline string to_string(const ::GPlatform::GpEnum& aEnum)
{
    return string(aEnum.ToString());
}

}//std

#endif//GP_USE_ENUMS
