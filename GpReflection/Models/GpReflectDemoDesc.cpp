#include "GpReflectDemoDesc.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GP_ENUM_IMPL(GpReflectDemoEnum);

REFLECT_IMPLEMENT(GpReflectDemoDesc, GP_MODULE_UUID)

GpReflectDemoDesc::GpReflectDemoDesc (void) noexcept
{
}

GpReflectDemoDesc::GpReflectDemoDesc (const GpReflectDemoDesc& aDesc):
GpReflectObject(aDesc),
ui8(aDesc.ui8),
si8(aDesc.si8),
ui16(aDesc.ui16),
si16(aDesc.si16),
ui32(aDesc.ui32),
si32(aDesc.si32),
ui64(aDesc.ui64),
si64(aDesc.si64),
uts_s(aDesc.uts_s),
uts_ms(aDesc.uts_ms),
real_f(aDesc.real_f),
real_d(aDesc.real_d),
b(aDesc.b),
uuid(aDesc.uuid),
str(aDesc.str),
blob(aDesc.blob),
e1(aDesc.e1),
ef1(aDesc.ef1),
s1(aDesc.s1),
s2(aDesc.s2)
{
}

GpReflectDemoDesc::GpReflectDemoDesc (GpReflectDemoDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
ui8(std::move(aDesc.ui8)),
si8(std::move(aDesc.si8)),
ui16(std::move(aDesc.ui16)),
si16(std::move(aDesc.si16)),
ui32(std::move(aDesc.ui32)),
si32(std::move(aDesc.si32)),
ui64(std::move(aDesc.ui64)),
si64(std::move(aDesc.si64)),
uts_s(std::move(aDesc.uts_s)),
uts_ms(std::move(aDesc.uts_ms)),
real_f(std::move(aDesc.real_f)),
real_d(std::move(aDesc.real_d)),
b(std::move(aDesc.b)),
uuid(std::move(aDesc.uuid)),
str(std::move(aDesc.str)),
blob(std::move(aDesc.blob)),
e1(std::move(aDesc.e1)),
ef1(std::move(aDesc.ef1)),
s1(std::move(aDesc.s1)),
s2(std::move(aDesc.s2))
{
}

GpReflectDemoDesc::~GpReflectDemoDesc (void) noexcept
{
}

void    GpReflectDemoDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(ui8);
    PROP(si8);
    PROP(ui16);
    PROP(si16);
    PROP(ui32);
    PROP(si32);
    PROP(ui64);
    PROP(si64);
    PROP(uts_s);
    PROP(uts_ms);
    PROP(real_f);
    PROP(real_d);
    PROP(b);
    PROP(uuid);
    PROP(str);
    PROP(blob);
    PROP(e1);
    PROP(ef1);
    PROP(s1);
    PROP(s2);
}

REFLECT_IMPLEMENT(GpReflectDemoVecDesc, GP_MODULE_UUID)

GpReflectDemoVecDesc::GpReflectDemoVecDesc (void) noexcept
{
}

GpReflectDemoVecDesc::GpReflectDemoVecDesc (const GpReflectDemoVecDesc& aDesc):
GpReflectDemoDesc(aDesc),
vec_ui8(aDesc.vec_ui8),
vec_si8(aDesc.vec_si8),
vec_ui16(aDesc.vec_ui16),
vec_si16(aDesc.vec_si16),
vec_ui32(aDesc.vec_ui32),
vec_si32(aDesc.vec_si32),
vec_ui64(aDesc.vec_ui64),
vec_si64(aDesc.vec_si64),
vec_uts_s(aDesc.vec_uts_s),
vec_uts_ms(aDesc.vec_uts_ms),
vec_real_f(aDesc.vec_real_f),
vec_real_d(aDesc.vec_real_d),
vec_b(aDesc.vec_b),
vec_uuid(aDesc.vec_uuid),
vec_str(aDesc.vec_str),
vec_blob(aDesc.vec_blob),
vec_s2(aDesc.vec_s2)
{
}

GpReflectDemoVecDesc::GpReflectDemoVecDesc (GpReflectDemoVecDesc&& aDesc) noexcept:
GpReflectDemoDesc(std::move(aDesc)),
vec_ui8(std::move(aDesc.vec_ui8)),
vec_si8(std::move(aDesc.vec_si8)),
vec_ui16(std::move(aDesc.vec_ui16)),
vec_si16(std::move(aDesc.vec_si16)),
vec_ui32(std::move(aDesc.vec_ui32)),
vec_si32(std::move(aDesc.vec_si32)),
vec_ui64(std::move(aDesc.vec_ui64)),
vec_si64(std::move(aDesc.vec_si64)),
vec_uts_s(std::move(aDesc.vec_uts_s)),
vec_uts_ms(std::move(aDesc.vec_uts_ms)),
vec_real_f(std::move(aDesc.vec_real_f)),
vec_real_d(std::move(aDesc.vec_real_d)),
vec_b(std::move(aDesc.vec_b)),
vec_uuid(std::move(aDesc.vec_uuid)),
vec_str(std::move(aDesc.vec_str)),
vec_blob(std::move(aDesc.vec_blob)),
vec_s2(std::move(aDesc.vec_s2))
{
}

GpReflectDemoVecDesc::~GpReflectDemoVecDesc (void) noexcept
{
}

void    GpReflectDemoVecDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(vec_ui8);
    PROP(vec_si8);
    PROP(vec_ui16);
    PROP(vec_si16);
    PROP(vec_ui32);
    PROP(vec_si32);
    PROP(vec_ui64);
    PROP(vec_si64);
    PROP(vec_uts_s);
    PROP(vec_uts_ms);
    PROP(vec_real_f);
    PROP(vec_real_d);
    PROP(vec_b);
    PROP(vec_uuid);
    PROP(vec_str);
    PROP(vec_blob);
    PROP(vec_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapUi8Desc, GP_MODULE_UUID)

GpReflectDemoMapUi8Desc::GpReflectDemoMapUi8Desc (void) noexcept
{
}

GpReflectDemoMapUi8Desc::GpReflectDemoMapUi8Desc (const GpReflectDemoMapUi8Desc& aDesc):
GpReflectDemoVecDesc(aDesc),
map_ui8_ui8(aDesc.map_ui8_ui8),
map_ui8_si8(aDesc.map_ui8_si8),
map_ui8_ui16(aDesc.map_ui8_ui16),
map_ui8_si16(aDesc.map_ui8_si16),
map_ui8_ui32(aDesc.map_ui8_ui32),
map_ui8_si32(aDesc.map_ui8_si32),
map_ui8_ui64(aDesc.map_ui8_ui64),
map_ui8_si64(aDesc.map_ui8_si64),
map_ui8_uts_s(aDesc.map_ui8_uts_s),
map_ui8_uts_ms(aDesc.map_ui8_uts_ms),
map_ui8_real_f(aDesc.map_ui8_real_f),
map_ui8_real_d(aDesc.map_ui8_real_d),
map_ui8_b(aDesc.map_ui8_b),
map_ui8_uuid(aDesc.map_ui8_uuid),
map_ui8_str(aDesc.map_ui8_str),
map_ui8_blob(aDesc.map_ui8_blob),
map_ui8_s2(aDesc.map_ui8_s2)
{
}

GpReflectDemoMapUi8Desc::GpReflectDemoMapUi8Desc (GpReflectDemoMapUi8Desc&& aDesc) noexcept:
GpReflectDemoVecDesc(std::move(aDesc)),
map_ui8_ui8(std::move(aDesc.map_ui8_ui8)),
map_ui8_si8(std::move(aDesc.map_ui8_si8)),
map_ui8_ui16(std::move(aDesc.map_ui8_ui16)),
map_ui8_si16(std::move(aDesc.map_ui8_si16)),
map_ui8_ui32(std::move(aDesc.map_ui8_ui32)),
map_ui8_si32(std::move(aDesc.map_ui8_si32)),
map_ui8_ui64(std::move(aDesc.map_ui8_ui64)),
map_ui8_si64(std::move(aDesc.map_ui8_si64)),
map_ui8_uts_s(std::move(aDesc.map_ui8_uts_s)),
map_ui8_uts_ms(std::move(aDesc.map_ui8_uts_ms)),
map_ui8_real_f(std::move(aDesc.map_ui8_real_f)),
map_ui8_real_d(std::move(aDesc.map_ui8_real_d)),
map_ui8_b(std::move(aDesc.map_ui8_b)),
map_ui8_uuid(std::move(aDesc.map_ui8_uuid)),
map_ui8_str(std::move(aDesc.map_ui8_str)),
map_ui8_blob(std::move(aDesc.map_ui8_blob)),
map_ui8_s2(std::move(aDesc.map_ui8_s2))
{
}

GpReflectDemoMapUi8Desc::~GpReflectDemoMapUi8Desc (void) noexcept
{
}

void    GpReflectDemoMapUi8Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_ui8_ui8);
    PROP(map_ui8_si8);
    PROP(map_ui8_ui16);
    PROP(map_ui8_si16);
    PROP(map_ui8_ui32);
    PROP(map_ui8_si32);
    PROP(map_ui8_ui64);
    PROP(map_ui8_si64);
    PROP(map_ui8_uts_s);
    PROP(map_ui8_uts_ms);
    PROP(map_ui8_real_f);
    PROP(map_ui8_real_d);
    PROP(map_ui8_b);
    PROP(map_ui8_uuid);
    PROP(map_ui8_str);
    PROP(map_ui8_blob);
    PROP(map_ui8_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapSi8Desc, GP_MODULE_UUID)

GpReflectDemoMapSi8Desc::GpReflectDemoMapSi8Desc (void) noexcept
{
}

GpReflectDemoMapSi8Desc::GpReflectDemoMapSi8Desc (const GpReflectDemoMapSi8Desc& aDesc):
GpReflectDemoMapUi8Desc(aDesc),
map_si8_ui8(aDesc.map_si8_ui8),
map_si8_si8(aDesc.map_si8_si8),
map_si8_ui16(aDesc.map_si8_ui16),
map_si8_si16(aDesc.map_si8_si16),
map_si8_ui32(aDesc.map_si8_ui32),
map_si8_si32(aDesc.map_si8_si32),
map_si8_ui64(aDesc.map_si8_ui64),
map_si8_si64(aDesc.map_si8_si64),
map_si8_uts_s(aDesc.map_si8_uts_s),
map_si8_uts_ms(aDesc.map_si8_uts_ms),
map_si8_real_f(aDesc.map_si8_real_f),
map_si8_real_d(aDesc.map_si8_real_d),
map_si8_b(aDesc.map_si8_b),
map_si8_uuid(aDesc.map_si8_uuid),
map_si8_str(aDesc.map_si8_str),
map_si8_blob(aDesc.map_si8_blob),
map_si8_s2(aDesc.map_si8_s2)
{
}

GpReflectDemoMapSi8Desc::GpReflectDemoMapSi8Desc (GpReflectDemoMapSi8Desc&& aDesc) noexcept:
GpReflectDemoMapUi8Desc(std::move(aDesc)),
map_si8_ui8(std::move(aDesc.map_si8_ui8)),
map_si8_si8(std::move(aDesc.map_si8_si8)),
map_si8_ui16(std::move(aDesc.map_si8_ui16)),
map_si8_si16(std::move(aDesc.map_si8_si16)),
map_si8_ui32(std::move(aDesc.map_si8_ui32)),
map_si8_si32(std::move(aDesc.map_si8_si32)),
map_si8_ui64(std::move(aDesc.map_si8_ui64)),
map_si8_si64(std::move(aDesc.map_si8_si64)),
map_si8_uts_s(std::move(aDesc.map_si8_uts_s)),
map_si8_uts_ms(std::move(aDesc.map_si8_uts_ms)),
map_si8_real_f(std::move(aDesc.map_si8_real_f)),
map_si8_real_d(std::move(aDesc.map_si8_real_d)),
map_si8_b(std::move(aDesc.map_si8_b)),
map_si8_uuid(std::move(aDesc.map_si8_uuid)),
map_si8_str(std::move(aDesc.map_si8_str)),
map_si8_blob(std::move(aDesc.map_si8_blob)),
map_si8_s2(std::move(aDesc.map_si8_s2))
{
}

GpReflectDemoMapSi8Desc::~GpReflectDemoMapSi8Desc (void) noexcept
{
}

void    GpReflectDemoMapSi8Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_si8_ui8);
    PROP(map_si8_si8);
    PROP(map_si8_ui16);
    PROP(map_si8_si16);
    PROP(map_si8_ui32);
    PROP(map_si8_si32);
    PROP(map_si8_ui64);
    PROP(map_si8_si64);
    PROP(map_si8_uts_s);
    PROP(map_si8_uts_ms);
    PROP(map_si8_real_f);
    PROP(map_si8_real_d);
    PROP(map_si8_b);
    PROP(map_si8_uuid);
    PROP(map_si8_str);
    PROP(map_si8_blob);
    PROP(map_si8_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapUi16Desc, GP_MODULE_UUID)

GpReflectDemoMapUi16Desc::GpReflectDemoMapUi16Desc (void) noexcept
{
}

GpReflectDemoMapUi16Desc::GpReflectDemoMapUi16Desc (const GpReflectDemoMapUi16Desc& aDesc):
GpReflectDemoMapSi8Desc(aDesc),
map_ui16_ui8(aDesc.map_ui16_ui8),
map_ui16_si8(aDesc.map_ui16_si8),
map_ui16_ui16(aDesc.map_ui16_ui16),
map_ui16_si16(aDesc.map_ui16_si16),
map_ui16_ui32(aDesc.map_ui16_ui32),
map_ui16_si32(aDesc.map_ui16_si32),
map_ui16_ui64(aDesc.map_ui16_ui64),
map_ui16_si64(aDesc.map_ui16_si64),
map_ui16_uts_s(aDesc.map_ui16_uts_s),
map_ui16_uts_ms(aDesc.map_ui16_uts_ms),
map_ui16_real_f(aDesc.map_ui16_real_f),
map_ui16_real_d(aDesc.map_ui16_real_d),
map_ui16_b(aDesc.map_ui16_b),
map_ui16_uuid(aDesc.map_ui16_uuid),
map_ui16_str(aDesc.map_ui16_str),
map_ui16_blob(aDesc.map_ui16_blob),
map_ui16_s2(aDesc.map_ui16_s2)
{
}

GpReflectDemoMapUi16Desc::GpReflectDemoMapUi16Desc (GpReflectDemoMapUi16Desc&& aDesc) noexcept:
GpReflectDemoMapSi8Desc(std::move(aDesc)),
map_ui16_ui8(std::move(aDesc.map_ui16_ui8)),
map_ui16_si8(std::move(aDesc.map_ui16_si8)),
map_ui16_ui16(std::move(aDesc.map_ui16_ui16)),
map_ui16_si16(std::move(aDesc.map_ui16_si16)),
map_ui16_ui32(std::move(aDesc.map_ui16_ui32)),
map_ui16_si32(std::move(aDesc.map_ui16_si32)),
map_ui16_ui64(std::move(aDesc.map_ui16_ui64)),
map_ui16_si64(std::move(aDesc.map_ui16_si64)),
map_ui16_uts_s(std::move(aDesc.map_ui16_uts_s)),
map_ui16_uts_ms(std::move(aDesc.map_ui16_uts_ms)),
map_ui16_real_f(std::move(aDesc.map_ui16_real_f)),
map_ui16_real_d(std::move(aDesc.map_ui16_real_d)),
map_ui16_b(std::move(aDesc.map_ui16_b)),
map_ui16_uuid(std::move(aDesc.map_ui16_uuid)),
map_ui16_str(std::move(aDesc.map_ui16_str)),
map_ui16_blob(std::move(aDesc.map_ui16_blob)),
map_ui16_s2(std::move(aDesc.map_ui16_s2))
{
}

GpReflectDemoMapUi16Desc::~GpReflectDemoMapUi16Desc (void) noexcept
{
}

void    GpReflectDemoMapUi16Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_ui16_ui8);
    PROP(map_ui16_si8);
    PROP(map_ui16_ui16);
    PROP(map_ui16_si16);
    PROP(map_ui16_ui32);
    PROP(map_ui16_si32);
    PROP(map_ui16_ui64);
    PROP(map_ui16_si64);
    PROP(map_ui16_uts_s);
    PROP(map_ui16_uts_ms);
    PROP(map_ui16_real_f);
    PROP(map_ui16_real_d);
    PROP(map_ui16_b);
    PROP(map_ui16_uuid);
    PROP(map_ui16_str);
    PROP(map_ui16_blob);
    PROP(map_ui16_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapSi16Desc, GP_MODULE_UUID)

GpReflectDemoMapSi16Desc::GpReflectDemoMapSi16Desc (void) noexcept
{
}

GpReflectDemoMapSi16Desc::GpReflectDemoMapSi16Desc (const GpReflectDemoMapSi16Desc& aDesc):
GpReflectDemoMapUi16Desc(aDesc),
map_si16_ui8(aDesc.map_si16_ui8),
map_si16_si8(aDesc.map_si16_si8),
map_si16_ui16(aDesc.map_si16_ui16),
map_si16_si16(aDesc.map_si16_si16),
map_si16_ui32(aDesc.map_si16_ui32),
map_si16_si32(aDesc.map_si16_si32),
map_si16_ui64(aDesc.map_si16_ui64),
map_si16_si64(aDesc.map_si16_si64),
map_si16_uts_s(aDesc.map_si16_uts_s),
map_si16_uts_ms(aDesc.map_si16_uts_ms),
map_si16_real_f(aDesc.map_si16_real_f),
map_si16_real_d(aDesc.map_si16_real_d),
map_si16_b(aDesc.map_si16_b),
map_si16_uuid(aDesc.map_si16_uuid),
map_si16_str(aDesc.map_si16_str),
map_si16_blob(aDesc.map_si16_blob),
map_si16_s2(aDesc.map_si16_s2)
{
}

GpReflectDemoMapSi16Desc::GpReflectDemoMapSi16Desc (GpReflectDemoMapSi16Desc&& aDesc) noexcept:
GpReflectDemoMapUi16Desc(std::move(aDesc)),
map_si16_ui8(std::move(aDesc.map_si16_ui8)),
map_si16_si8(std::move(aDesc.map_si16_si8)),
map_si16_ui16(std::move(aDesc.map_si16_ui16)),
map_si16_si16(std::move(aDesc.map_si16_si16)),
map_si16_ui32(std::move(aDesc.map_si16_ui32)),
map_si16_si32(std::move(aDesc.map_si16_si32)),
map_si16_ui64(std::move(aDesc.map_si16_ui64)),
map_si16_si64(std::move(aDesc.map_si16_si64)),
map_si16_uts_s(std::move(aDesc.map_si16_uts_s)),
map_si16_uts_ms(std::move(aDesc.map_si16_uts_ms)),
map_si16_real_f(std::move(aDesc.map_si16_real_f)),
map_si16_real_d(std::move(aDesc.map_si16_real_d)),
map_si16_b(std::move(aDesc.map_si16_b)),
map_si16_uuid(std::move(aDesc.map_si16_uuid)),
map_si16_str(std::move(aDesc.map_si16_str)),
map_si16_blob(std::move(aDesc.map_si16_blob)),
map_si16_s2(std::move(aDesc.map_si16_s2))
{
}

GpReflectDemoMapSi16Desc::~GpReflectDemoMapSi16Desc (void) noexcept
{
}

void    GpReflectDemoMapSi16Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_si16_ui8);
    PROP(map_si16_si8);
    PROP(map_si16_ui16);
    PROP(map_si16_si16);
    PROP(map_si16_ui32);
    PROP(map_si16_si32);
    PROP(map_si16_ui64);
    PROP(map_si16_si64);
    PROP(map_si16_uts_s);
    PROP(map_si16_uts_ms);
    PROP(map_si16_real_f);
    PROP(map_si16_real_d);
    PROP(map_si16_b);
    PROP(map_si16_uuid);
    PROP(map_si16_str);
    PROP(map_si16_blob);
    PROP(map_si16_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapUi32Desc, GP_MODULE_UUID)

GpReflectDemoMapUi32Desc::GpReflectDemoMapUi32Desc (void) noexcept
{
}

GpReflectDemoMapUi32Desc::GpReflectDemoMapUi32Desc (const GpReflectDemoMapUi32Desc& aDesc):
GpReflectDemoMapSi16Desc(aDesc),
map_ui32_ui8(aDesc.map_ui32_ui8),
map_ui32_si8(aDesc.map_ui32_si8),
map_ui32_ui16(aDesc.map_ui32_ui16),
map_ui32_si16(aDesc.map_ui32_si16),
map_ui32_ui32(aDesc.map_ui32_ui32),
map_ui32_si32(aDesc.map_ui32_si32),
map_ui32_ui64(aDesc.map_ui32_ui64),
map_ui32_si64(aDesc.map_ui32_si64),
map_ui32_uts_s(aDesc.map_ui32_uts_s),
map_ui32_uts_ms(aDesc.map_ui32_uts_ms),
map_ui32_real_f(aDesc.map_ui32_real_f),
map_ui32_real_d(aDesc.map_ui32_real_d),
map_ui32_b(aDesc.map_ui32_b),
map_ui32_uuid(aDesc.map_ui32_uuid),
map_ui32_str(aDesc.map_ui32_str),
map_ui32_blob(aDesc.map_ui32_blob),
map_ui32_s2(aDesc.map_ui32_s2)
{
}

GpReflectDemoMapUi32Desc::GpReflectDemoMapUi32Desc (GpReflectDemoMapUi32Desc&& aDesc) noexcept:
GpReflectDemoMapSi16Desc(std::move(aDesc)),
map_ui32_ui8(std::move(aDesc.map_ui32_ui8)),
map_ui32_si8(std::move(aDesc.map_ui32_si8)),
map_ui32_ui16(std::move(aDesc.map_ui32_ui16)),
map_ui32_si16(std::move(aDesc.map_ui32_si16)),
map_ui32_ui32(std::move(aDesc.map_ui32_ui32)),
map_ui32_si32(std::move(aDesc.map_ui32_si32)),
map_ui32_ui64(std::move(aDesc.map_ui32_ui64)),
map_ui32_si64(std::move(aDesc.map_ui32_si64)),
map_ui32_uts_s(std::move(aDesc.map_ui32_uts_s)),
map_ui32_uts_ms(std::move(aDesc.map_ui32_uts_ms)),
map_ui32_real_f(std::move(aDesc.map_ui32_real_f)),
map_ui32_real_d(std::move(aDesc.map_ui32_real_d)),
map_ui32_b(std::move(aDesc.map_ui32_b)),
map_ui32_uuid(std::move(aDesc.map_ui32_uuid)),
map_ui32_str(std::move(aDesc.map_ui32_str)),
map_ui32_blob(std::move(aDesc.map_ui32_blob)),
map_ui32_s2(std::move(aDesc.map_ui32_s2))
{
}

GpReflectDemoMapUi32Desc::~GpReflectDemoMapUi32Desc (void) noexcept
{
}

void    GpReflectDemoMapUi32Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_ui32_ui8);
    PROP(map_ui32_si8);
    PROP(map_ui32_ui16);
    PROP(map_ui32_si16);
    PROP(map_ui32_ui32);
    PROP(map_ui32_si32);
    PROP(map_ui32_ui64);
    PROP(map_ui32_si64);
    PROP(map_ui32_uts_s);
    PROP(map_ui32_uts_ms);
    PROP(map_ui32_real_f);
    PROP(map_ui32_real_d);
    PROP(map_ui32_b);
    PROP(map_ui32_uuid);
    PROP(map_ui32_str);
    PROP(map_ui32_blob);
    PROP(map_ui32_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapSi32Desc, GP_MODULE_UUID)

GpReflectDemoMapSi32Desc::GpReflectDemoMapSi32Desc (void) noexcept
{
}

GpReflectDemoMapSi32Desc::GpReflectDemoMapSi32Desc (const GpReflectDemoMapSi32Desc& aDesc):
GpReflectDemoMapUi32Desc(aDesc),
map_si32_ui8(aDesc.map_si32_ui8),
map_si32_si8(aDesc.map_si32_si8),
map_si32_ui16(aDesc.map_si32_ui16),
map_si32_si16(aDesc.map_si32_si16),
map_si32_ui32(aDesc.map_si32_ui32),
map_si32_si32(aDesc.map_si32_si32),
map_si32_ui64(aDesc.map_si32_ui64),
map_si32_si64(aDesc.map_si32_si64),
map_si32_uts_s(aDesc.map_si32_uts_s),
map_si32_uts_ms(aDesc.map_si32_uts_ms),
map_si32_real_f(aDesc.map_si32_real_f),
map_si32_real_d(aDesc.map_si32_real_d),
map_si32_b(aDesc.map_si32_b),
map_si32_uuid(aDesc.map_si32_uuid),
map_si32_str(aDesc.map_si32_str),
map_si32_blob(aDesc.map_si32_blob),
map_si32_s2(aDesc.map_si32_s2)
{
}

GpReflectDemoMapSi32Desc::GpReflectDemoMapSi32Desc (GpReflectDemoMapSi32Desc&& aDesc) noexcept:
GpReflectDemoMapUi32Desc(std::move(aDesc)),
map_si32_ui8(std::move(aDesc.map_si32_ui8)),
map_si32_si8(std::move(aDesc.map_si32_si8)),
map_si32_ui16(std::move(aDesc.map_si32_ui16)),
map_si32_si16(std::move(aDesc.map_si32_si16)),
map_si32_ui32(std::move(aDesc.map_si32_ui32)),
map_si32_si32(std::move(aDesc.map_si32_si32)),
map_si32_ui64(std::move(aDesc.map_si32_ui64)),
map_si32_si64(std::move(aDesc.map_si32_si64)),
map_si32_uts_s(std::move(aDesc.map_si32_uts_s)),
map_si32_uts_ms(std::move(aDesc.map_si32_uts_ms)),
map_si32_real_f(std::move(aDesc.map_si32_real_f)),
map_si32_real_d(std::move(aDesc.map_si32_real_d)),
map_si32_b(std::move(aDesc.map_si32_b)),
map_si32_uuid(std::move(aDesc.map_si32_uuid)),
map_si32_str(std::move(aDesc.map_si32_str)),
map_si32_blob(std::move(aDesc.map_si32_blob)),
map_si32_s2(std::move(aDesc.map_si32_s2))
{
}

GpReflectDemoMapSi32Desc::~GpReflectDemoMapSi32Desc (void) noexcept
{
}

void    GpReflectDemoMapSi32Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_si32_ui8);
    PROP(map_si32_si8);
    PROP(map_si32_ui16);
    PROP(map_si32_si16);
    PROP(map_si32_ui32);
    PROP(map_si32_si32);
    PROP(map_si32_ui64);
    PROP(map_si32_si64);
    PROP(map_si32_uts_s);
    PROP(map_si32_uts_ms);
    PROP(map_si32_real_f);
    PROP(map_si32_real_d);
    PROP(map_si32_b);
    PROP(map_si32_uuid);
    PROP(map_si32_str);
    PROP(map_si32_blob);
    PROP(map_si32_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapUi64Desc, GP_MODULE_UUID)

GpReflectDemoMapUi64Desc::GpReflectDemoMapUi64Desc (void) noexcept
{
}

GpReflectDemoMapUi64Desc::GpReflectDemoMapUi64Desc (const GpReflectDemoMapUi64Desc& aDesc):
GpReflectDemoMapSi32Desc(aDesc),
map_ui64_ui8(aDesc.map_ui64_ui8),
map_ui64_si8(aDesc.map_ui64_si8),
map_ui64_ui16(aDesc.map_ui64_ui16),
map_ui64_si16(aDesc.map_ui64_si16),
map_ui64_ui32(aDesc.map_ui64_ui32),
map_ui64_si32(aDesc.map_ui64_si32),
map_ui64_ui64(aDesc.map_ui64_ui64),
map_ui64_si64(aDesc.map_ui64_si64),
map_ui64_uts_s(aDesc.map_ui64_uts_s),
map_ui64_uts_ms(aDesc.map_ui64_uts_ms),
map_ui64_real_f(aDesc.map_ui64_real_f),
map_ui64_real_d(aDesc.map_ui64_real_d),
map_ui64_b(aDesc.map_ui64_b),
map_ui64_uuid(aDesc.map_ui64_uuid),
map_ui64_str(aDesc.map_ui64_str),
map_ui64_blob(aDesc.map_ui64_blob),
map_ui64_s2(aDesc.map_ui64_s2)
{
}

GpReflectDemoMapUi64Desc::GpReflectDemoMapUi64Desc (GpReflectDemoMapUi64Desc&& aDesc) noexcept:
GpReflectDemoMapSi32Desc(std::move(aDesc)),
map_ui64_ui8(std::move(aDesc.map_ui64_ui8)),
map_ui64_si8(std::move(aDesc.map_ui64_si8)),
map_ui64_ui16(std::move(aDesc.map_ui64_ui16)),
map_ui64_si16(std::move(aDesc.map_ui64_si16)),
map_ui64_ui32(std::move(aDesc.map_ui64_ui32)),
map_ui64_si32(std::move(aDesc.map_ui64_si32)),
map_ui64_ui64(std::move(aDesc.map_ui64_ui64)),
map_ui64_si64(std::move(aDesc.map_ui64_si64)),
map_ui64_uts_s(std::move(aDesc.map_ui64_uts_s)),
map_ui64_uts_ms(std::move(aDesc.map_ui64_uts_ms)),
map_ui64_real_f(std::move(aDesc.map_ui64_real_f)),
map_ui64_real_d(std::move(aDesc.map_ui64_real_d)),
map_ui64_b(std::move(aDesc.map_ui64_b)),
map_ui64_uuid(std::move(aDesc.map_ui64_uuid)),
map_ui64_str(std::move(aDesc.map_ui64_str)),
map_ui64_blob(std::move(aDesc.map_ui64_blob)),
map_ui64_s2(std::move(aDesc.map_ui64_s2))
{
}

GpReflectDemoMapUi64Desc::~GpReflectDemoMapUi64Desc (void) noexcept
{
}

void    GpReflectDemoMapUi64Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_ui64_ui8);
    PROP(map_ui64_si8);
    PROP(map_ui64_ui16);
    PROP(map_ui64_si16);
    PROP(map_ui64_ui32);
    PROP(map_ui64_si32);
    PROP(map_ui64_ui64);
    PROP(map_ui64_si64);
    PROP(map_ui64_uts_s);
    PROP(map_ui64_uts_ms);
    PROP(map_ui64_real_f);
    PROP(map_ui64_real_d);
    PROP(map_ui64_b);
    PROP(map_ui64_uuid);
    PROP(map_ui64_str);
    PROP(map_ui64_blob);
    PROP(map_ui64_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapSi64Desc, GP_MODULE_UUID)

GpReflectDemoMapSi64Desc::GpReflectDemoMapSi64Desc (void) noexcept
{
}

GpReflectDemoMapSi64Desc::GpReflectDemoMapSi64Desc (const GpReflectDemoMapSi64Desc& aDesc):
GpReflectDemoMapUi64Desc(aDesc),
map_si64_ui8(aDesc.map_si64_ui8),
map_si64_si8(aDesc.map_si64_si8),
map_si64_ui16(aDesc.map_si64_ui16),
map_si64_si16(aDesc.map_si64_si16),
map_si64_ui32(aDesc.map_si64_ui32),
map_si64_si32(aDesc.map_si64_si32),
map_si64_ui64(aDesc.map_si64_ui64),
map_si64_si64(aDesc.map_si64_si64),
map_si64_uts_s(aDesc.map_si64_uts_s),
map_si64_uts_ms(aDesc.map_si64_uts_ms),
map_si64_real_f(aDesc.map_si64_real_f),
map_si64_real_d(aDesc.map_si64_real_d),
map_si64_b(aDesc.map_si64_b),
map_si64_uuid(aDesc.map_si64_uuid),
map_si64_str(aDesc.map_si64_str),
map_si64_blob(aDesc.map_si64_blob),
map_si64_s2(aDesc.map_si64_s2)
{
}

GpReflectDemoMapSi64Desc::GpReflectDemoMapSi64Desc (GpReflectDemoMapSi64Desc&& aDesc) noexcept:
GpReflectDemoMapUi64Desc(std::move(aDesc)),
map_si64_ui8(std::move(aDesc.map_si64_ui8)),
map_si64_si8(std::move(aDesc.map_si64_si8)),
map_si64_ui16(std::move(aDesc.map_si64_ui16)),
map_si64_si16(std::move(aDesc.map_si64_si16)),
map_si64_ui32(std::move(aDesc.map_si64_ui32)),
map_si64_si32(std::move(aDesc.map_si64_si32)),
map_si64_ui64(std::move(aDesc.map_si64_ui64)),
map_si64_si64(std::move(aDesc.map_si64_si64)),
map_si64_uts_s(std::move(aDesc.map_si64_uts_s)),
map_si64_uts_ms(std::move(aDesc.map_si64_uts_ms)),
map_si64_real_f(std::move(aDesc.map_si64_real_f)),
map_si64_real_d(std::move(aDesc.map_si64_real_d)),
map_si64_b(std::move(aDesc.map_si64_b)),
map_si64_uuid(std::move(aDesc.map_si64_uuid)),
map_si64_str(std::move(aDesc.map_si64_str)),
map_si64_blob(std::move(aDesc.map_si64_blob)),
map_si64_s2(std::move(aDesc.map_si64_s2))
{
}

GpReflectDemoMapSi64Desc::~GpReflectDemoMapSi64Desc (void) noexcept
{
}

void    GpReflectDemoMapSi64Desc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_si64_ui8);
    PROP(map_si64_si8);
    PROP(map_si64_ui16);
    PROP(map_si64_si16);
    PROP(map_si64_ui32);
    PROP(map_si64_si32);
    PROP(map_si64_ui64);
    PROP(map_si64_si64);
    PROP(map_si64_uts_s);
    PROP(map_si64_uts_ms);
    PROP(map_si64_real_f);
    PROP(map_si64_real_d);
    PROP(map_si64_b);
    PROP(map_si64_uuid);
    PROP(map_si64_str);
    PROP(map_si64_blob);
    PROP(map_si64_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapUtsSDesc, GP_MODULE_UUID)

GpReflectDemoMapUtsSDesc::GpReflectDemoMapUtsSDesc (void) noexcept
{
}

GpReflectDemoMapUtsSDesc::GpReflectDemoMapUtsSDesc (const GpReflectDemoMapUtsSDesc& aDesc):
GpReflectDemoMapSi64Desc(aDesc),
map_utss_ui8(aDesc.map_utss_ui8),
map_utss_si8(aDesc.map_utss_si8),
map_utss_ui16(aDesc.map_utss_ui16),
map_utss_si16(aDesc.map_utss_si16),
map_utss_ui32(aDesc.map_utss_ui32),
map_utss_si32(aDesc.map_utss_si32),
map_utss_ui64(aDesc.map_utss_ui64),
map_utss_si64(aDesc.map_utss_si64),
map_utss_uts_s(aDesc.map_utss_uts_s),
map_utss_uts_ms(aDesc.map_utss_uts_ms),
map_utss_real_f(aDesc.map_utss_real_f),
map_utss_real_d(aDesc.map_utss_real_d),
map_utss_b(aDesc.map_utss_b),
map_utss_uuid(aDesc.map_utss_uuid),
map_utss_str(aDesc.map_utss_str),
map_utss_blob(aDesc.map_utss_blob),
map_utss_s2(aDesc.map_utss_s2)
{
}

GpReflectDemoMapUtsSDesc::GpReflectDemoMapUtsSDesc (GpReflectDemoMapUtsSDesc&& aDesc) noexcept:
GpReflectDemoMapSi64Desc(std::move(aDesc)),
map_utss_ui8(std::move(aDesc.map_utss_ui8)),
map_utss_si8(std::move(aDesc.map_utss_si8)),
map_utss_ui16(std::move(aDesc.map_utss_ui16)),
map_utss_si16(std::move(aDesc.map_utss_si16)),
map_utss_ui32(std::move(aDesc.map_utss_ui32)),
map_utss_si32(std::move(aDesc.map_utss_si32)),
map_utss_ui64(std::move(aDesc.map_utss_ui64)),
map_utss_si64(std::move(aDesc.map_utss_si64)),
map_utss_uts_s(std::move(aDesc.map_utss_uts_s)),
map_utss_uts_ms(std::move(aDesc.map_utss_uts_ms)),
map_utss_real_f(std::move(aDesc.map_utss_real_f)),
map_utss_real_d(std::move(aDesc.map_utss_real_d)),
map_utss_b(std::move(aDesc.map_utss_b)),
map_utss_uuid(std::move(aDesc.map_utss_uuid)),
map_utss_str(std::move(aDesc.map_utss_str)),
map_utss_blob(std::move(aDesc.map_utss_blob)),
map_utss_s2(std::move(aDesc.map_utss_s2))
{
}

GpReflectDemoMapUtsSDesc::~GpReflectDemoMapUtsSDesc (void) noexcept
{
}

void    GpReflectDemoMapUtsSDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_utss_ui8);
    PROP(map_utss_si8);
    PROP(map_utss_ui16);
    PROP(map_utss_si16);
    PROP(map_utss_ui32);
    PROP(map_utss_si32);
    PROP(map_utss_ui64);
    PROP(map_utss_si64);
    PROP(map_utss_uts_s);
    PROP(map_utss_uts_ms);
    PROP(map_utss_real_f);
    PROP(map_utss_real_d);
    PROP(map_utss_b);
    PROP(map_utss_uuid);
    PROP(map_utss_str);
    PROP(map_utss_blob);
    PROP(map_utss_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapUtmsSDesc, GP_MODULE_UUID)

GpReflectDemoMapUtmsSDesc::GpReflectDemoMapUtmsSDesc (void) noexcept
{
}

GpReflectDemoMapUtmsSDesc::GpReflectDemoMapUtmsSDesc (const GpReflectDemoMapUtmsSDesc& aDesc):
GpReflectDemoMapUtsSDesc(aDesc),
map_utsms_ui8(aDesc.map_utsms_ui8),
map_utsms_si8(aDesc.map_utsms_si8),
map_utsms_ui16(aDesc.map_utsms_ui16),
map_utsms_si16(aDesc.map_utsms_si16),
map_utsms_ui32(aDesc.map_utsms_ui32),
map_utsms_si32(aDesc.map_utsms_si32),
map_utsms_ui64(aDesc.map_utsms_ui64),
map_utsms_si64(aDesc.map_utsms_si64),
map_utsms_uts_s(aDesc.map_utsms_uts_s),
map_utsms_uts_ms(aDesc.map_utsms_uts_ms),
map_utsms_real_f(aDesc.map_utsms_real_f),
map_utsms_real_d(aDesc.map_utsms_real_d),
map_utsms_b(aDesc.map_utsms_b),
map_utsms_uuid(aDesc.map_utsms_uuid),
map_utsms_str(aDesc.map_utsms_str),
map_utsms_blob(aDesc.map_utsms_blob),
map_utsms_s2(aDesc.map_utsms_s2)
{
}

GpReflectDemoMapUtmsSDesc::GpReflectDemoMapUtmsSDesc (GpReflectDemoMapUtmsSDesc&& aDesc) noexcept:
GpReflectDemoMapUtsSDesc(std::move(aDesc)),
map_utsms_ui8(std::move(aDesc.map_utsms_ui8)),
map_utsms_si8(std::move(aDesc.map_utsms_si8)),
map_utsms_ui16(std::move(aDesc.map_utsms_ui16)),
map_utsms_si16(std::move(aDesc.map_utsms_si16)),
map_utsms_ui32(std::move(aDesc.map_utsms_ui32)),
map_utsms_si32(std::move(aDesc.map_utsms_si32)),
map_utsms_ui64(std::move(aDesc.map_utsms_ui64)),
map_utsms_si64(std::move(aDesc.map_utsms_si64)),
map_utsms_uts_s(std::move(aDesc.map_utsms_uts_s)),
map_utsms_uts_ms(std::move(aDesc.map_utsms_uts_ms)),
map_utsms_real_f(std::move(aDesc.map_utsms_real_f)),
map_utsms_real_d(std::move(aDesc.map_utsms_real_d)),
map_utsms_b(std::move(aDesc.map_utsms_b)),
map_utsms_uuid(std::move(aDesc.map_utsms_uuid)),
map_utsms_str(std::move(aDesc.map_utsms_str)),
map_utsms_blob(std::move(aDesc.map_utsms_blob)),
map_utsms_s2(std::move(aDesc.map_utsms_s2))
{
}

GpReflectDemoMapUtmsSDesc::~GpReflectDemoMapUtmsSDesc (void) noexcept
{
}

void    GpReflectDemoMapUtmsSDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_utsms_ui8);
    PROP(map_utsms_si8);
    PROP(map_utsms_ui16);
    PROP(map_utsms_si16);
    PROP(map_utsms_ui32);
    PROP(map_utsms_si32);
    PROP(map_utsms_ui64);
    PROP(map_utsms_si64);
    PROP(map_utsms_uts_s);
    PROP(map_utsms_uts_ms);
    PROP(map_utsms_real_f);
    PROP(map_utsms_real_d);
    PROP(map_utsms_b);
    PROP(map_utsms_uuid);
    PROP(map_utsms_str);
    PROP(map_utsms_blob);
    PROP(map_utsms_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapFloatSDesc, GP_MODULE_UUID)

GpReflectDemoMapFloatSDesc::GpReflectDemoMapFloatSDesc (void) noexcept
{
}

GpReflectDemoMapFloatSDesc::GpReflectDemoMapFloatSDesc (const GpReflectDemoMapFloatSDesc& aDesc):
GpReflectDemoMapUtmsSDesc(aDesc),
map_float_ui8(aDesc.map_float_ui8),
map_float_si8(aDesc.map_float_si8),
map_float_ui16(aDesc.map_float_ui16),
map_float_si16(aDesc.map_float_si16),
map_float_ui32(aDesc.map_float_ui32),
map_float_si32(aDesc.map_float_si32),
map_float_ui64(aDesc.map_float_ui64),
map_float_si64(aDesc.map_float_si64),
map_float_uts_s(aDesc.map_float_uts_s),
map_float_uts_ms(aDesc.map_float_uts_ms),
map_float_real_f(aDesc.map_float_real_f),
map_float_real_d(aDesc.map_float_real_d),
map_float_b(aDesc.map_float_b),
map_float_uuid(aDesc.map_float_uuid),
map_float_str(aDesc.map_float_str),
map_float_blob(aDesc.map_float_blob),
map_float_s2(aDesc.map_float_s2)
{
}

GpReflectDemoMapFloatSDesc::GpReflectDemoMapFloatSDesc (GpReflectDemoMapFloatSDesc&& aDesc) noexcept:
GpReflectDemoMapUtmsSDesc(std::move(aDesc)),
map_float_ui8(std::move(aDesc.map_float_ui8)),
map_float_si8(std::move(aDesc.map_float_si8)),
map_float_ui16(std::move(aDesc.map_float_ui16)),
map_float_si16(std::move(aDesc.map_float_si16)),
map_float_ui32(std::move(aDesc.map_float_ui32)),
map_float_si32(std::move(aDesc.map_float_si32)),
map_float_ui64(std::move(aDesc.map_float_ui64)),
map_float_si64(std::move(aDesc.map_float_si64)),
map_float_uts_s(std::move(aDesc.map_float_uts_s)),
map_float_uts_ms(std::move(aDesc.map_float_uts_ms)),
map_float_real_f(std::move(aDesc.map_float_real_f)),
map_float_real_d(std::move(aDesc.map_float_real_d)),
map_float_b(std::move(aDesc.map_float_b)),
map_float_uuid(std::move(aDesc.map_float_uuid)),
map_float_str(std::move(aDesc.map_float_str)),
map_float_blob(std::move(aDesc.map_float_blob)),
map_float_s2(std::move(aDesc.map_float_s2))
{
}

GpReflectDemoMapFloatSDesc::~GpReflectDemoMapFloatSDesc (void) noexcept
{
}

void    GpReflectDemoMapFloatSDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_float_ui8);
    PROP(map_float_si8);
    PROP(map_float_ui16);
    PROP(map_float_si16);
    PROP(map_float_ui32);
    PROP(map_float_si32);
    PROP(map_float_ui64);
    PROP(map_float_si64);
    PROP(map_float_uts_s);
    PROP(map_float_uts_ms);
    PROP(map_float_real_f);
    PROP(map_float_real_d);
    PROP(map_float_b);
    PROP(map_float_uuid);
    PROP(map_float_str);
    PROP(map_float_blob);
    PROP(map_float_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapDoubleSDesc, GP_MODULE_UUID)

GpReflectDemoMapDoubleSDesc::GpReflectDemoMapDoubleSDesc (void) noexcept
{
}

GpReflectDemoMapDoubleSDesc::GpReflectDemoMapDoubleSDesc (const GpReflectDemoMapDoubleSDesc& aDesc):
GpReflectDemoMapFloatSDesc(aDesc),
map_double_ui8(aDesc.map_double_ui8),
map_double_si8(aDesc.map_double_si8),
map_double_ui16(aDesc.map_double_ui16),
map_double_si16(aDesc.map_double_si16),
map_double_ui32(aDesc.map_double_ui32),
map_double_si32(aDesc.map_double_si32),
map_double_ui64(aDesc.map_double_ui64),
map_double_si64(aDesc.map_double_si64),
map_double_uts_s(aDesc.map_double_uts_s),
map_double_uts_ms(aDesc.map_double_uts_ms),
map_double_real_f(aDesc.map_double_real_f),
map_double_real_d(aDesc.map_double_real_d),
map_double_b(aDesc.map_double_b),
map_double_uuid(aDesc.map_double_uuid),
map_double_str(aDesc.map_double_str),
map_double_blob(aDesc.map_double_blob),
map_double_s2(aDesc.map_double_s2)
{
}

GpReflectDemoMapDoubleSDesc::GpReflectDemoMapDoubleSDesc (GpReflectDemoMapDoubleSDesc&& aDesc) noexcept:
GpReflectDemoMapFloatSDesc(std::move(aDesc)),
map_double_ui8(std::move(aDesc.map_double_ui8)),
map_double_si8(std::move(aDesc.map_double_si8)),
map_double_ui16(std::move(aDesc.map_double_ui16)),
map_double_si16(std::move(aDesc.map_double_si16)),
map_double_ui32(std::move(aDesc.map_double_ui32)),
map_double_si32(std::move(aDesc.map_double_si32)),
map_double_ui64(std::move(aDesc.map_double_ui64)),
map_double_si64(std::move(aDesc.map_double_si64)),
map_double_uts_s(std::move(aDesc.map_double_uts_s)),
map_double_uts_ms(std::move(aDesc.map_double_uts_ms)),
map_double_real_f(std::move(aDesc.map_double_real_f)),
map_double_real_d(std::move(aDesc.map_double_real_d)),
map_double_b(std::move(aDesc.map_double_b)),
map_double_uuid(std::move(aDesc.map_double_uuid)),
map_double_str(std::move(aDesc.map_double_str)),
map_double_blob(std::move(aDesc.map_double_blob)),
map_double_s2(std::move(aDesc.map_double_s2))
{
}

GpReflectDemoMapDoubleSDesc::~GpReflectDemoMapDoubleSDesc (void) noexcept
{
}

void    GpReflectDemoMapDoubleSDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_double_ui8);
    PROP(map_double_si8);
    PROP(map_double_ui16);
    PROP(map_double_si16);
    PROP(map_double_ui32);
    PROP(map_double_si32);
    PROP(map_double_ui64);
    PROP(map_double_si64);
    PROP(map_double_uts_s);
    PROP(map_double_uts_ms);
    PROP(map_double_real_f);
    PROP(map_double_real_d);
    PROP(map_double_b);
    PROP(map_double_uuid);
    PROP(map_double_str);
    PROP(map_double_blob);
    PROP(map_double_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapUuidSDesc, GP_MODULE_UUID)

GpReflectDemoMapUuidSDesc::GpReflectDemoMapUuidSDesc (void) noexcept
{
}

GpReflectDemoMapUuidSDesc::GpReflectDemoMapUuidSDesc (const GpReflectDemoMapUuidSDesc& aDesc):
GpReflectDemoMapDoubleSDesc(aDesc),
map_uuid_ui8(aDesc.map_uuid_ui8),
map_uuid_si8(aDesc.map_uuid_si8),
map_uuid_ui16(aDesc.map_uuid_ui16),
map_uuid_si16(aDesc.map_uuid_si16),
map_uuid_ui32(aDesc.map_uuid_ui32),
map_uuid_si32(aDesc.map_uuid_si32),
map_uuid_ui64(aDesc.map_uuid_ui64),
map_uuid_si64(aDesc.map_uuid_si64),
map_uuid_uts_s(aDesc.map_uuid_uts_s),
map_uuid_uts_ms(aDesc.map_uuid_uts_ms),
map_uuid_real_f(aDesc.map_uuid_real_f),
map_uuid_real_d(aDesc.map_uuid_real_d),
map_uuid_b(aDesc.map_uuid_b),
map_uuid_uuid(aDesc.map_uuid_uuid),
map_uuid_str(aDesc.map_uuid_str),
map_uuid_blob(aDesc.map_uuid_blob),
map_uuid_s2(aDesc.map_uuid_s2)
{
}

GpReflectDemoMapUuidSDesc::GpReflectDemoMapUuidSDesc (GpReflectDemoMapUuidSDesc&& aDesc) noexcept:
GpReflectDemoMapDoubleSDesc(std::move(aDesc)),
map_uuid_ui8(std::move(aDesc.map_uuid_ui8)),
map_uuid_si8(std::move(aDesc.map_uuid_si8)),
map_uuid_ui16(std::move(aDesc.map_uuid_ui16)),
map_uuid_si16(std::move(aDesc.map_uuid_si16)),
map_uuid_ui32(std::move(aDesc.map_uuid_ui32)),
map_uuid_si32(std::move(aDesc.map_uuid_si32)),
map_uuid_ui64(std::move(aDesc.map_uuid_ui64)),
map_uuid_si64(std::move(aDesc.map_uuid_si64)),
map_uuid_uts_s(std::move(aDesc.map_uuid_uts_s)),
map_uuid_uts_ms(std::move(aDesc.map_uuid_uts_ms)),
map_uuid_real_f(std::move(aDesc.map_uuid_real_f)),
map_uuid_real_d(std::move(aDesc.map_uuid_real_d)),
map_uuid_b(std::move(aDesc.map_uuid_b)),
map_uuid_uuid(std::move(aDesc.map_uuid_uuid)),
map_uuid_str(std::move(aDesc.map_uuid_str)),
map_uuid_blob(std::move(aDesc.map_uuid_blob)),
map_uuid_s2(std::move(aDesc.map_uuid_s2))
{
}

GpReflectDemoMapUuidSDesc::~GpReflectDemoMapUuidSDesc (void) noexcept
{
}

void    GpReflectDemoMapUuidSDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_uuid_ui8);
    PROP(map_uuid_si8);
    PROP(map_uuid_ui16);
    PROP(map_uuid_si16);
    PROP(map_uuid_ui32);
    PROP(map_uuid_si32);
    PROP(map_uuid_ui64);
    PROP(map_uuid_si64);
    PROP(map_uuid_uts_s);
    PROP(map_uuid_uts_ms);
    PROP(map_uuid_real_f);
    PROP(map_uuid_real_d);
    PROP(map_uuid_b);
    PROP(map_uuid_uuid);
    PROP(map_uuid_str);
    PROP(map_uuid_blob);
    PROP(map_uuid_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapStrSDesc, GP_MODULE_UUID)

GpReflectDemoMapStrSDesc::GpReflectDemoMapStrSDesc (void) noexcept
{
}

GpReflectDemoMapStrSDesc::GpReflectDemoMapStrSDesc (const GpReflectDemoMapStrSDesc& aDesc):
GpReflectDemoMapUuidSDesc(aDesc),
map_str_ui8(aDesc.map_str_ui8),
map_str_si8(aDesc.map_str_si8),
map_str_ui16(aDesc.map_str_ui16),
map_str_si16(aDesc.map_str_si16),
map_str_ui32(aDesc.map_str_ui32),
map_str_si32(aDesc.map_str_si32),
map_str_ui64(aDesc.map_str_ui64),
map_str_si64(aDesc.map_str_si64),
map_str_uts_s(aDesc.map_str_uts_s),
map_str_uts_ms(aDesc.map_str_uts_ms),
map_str_real_f(aDesc.map_str_real_f),
map_str_real_d(aDesc.map_str_real_d),
map_str_b(aDesc.map_str_b),
map_str_uuid(aDesc.map_str_uuid),
map_str_str(aDesc.map_str_str),
map_str_blob(aDesc.map_str_blob),
map_str_s2(aDesc.map_str_s2)
{
}

GpReflectDemoMapStrSDesc::GpReflectDemoMapStrSDesc (GpReflectDemoMapStrSDesc&& aDesc) noexcept:
GpReflectDemoMapUuidSDesc(std::move(aDesc)),
map_str_ui8(std::move(aDesc.map_str_ui8)),
map_str_si8(std::move(aDesc.map_str_si8)),
map_str_ui16(std::move(aDesc.map_str_ui16)),
map_str_si16(std::move(aDesc.map_str_si16)),
map_str_ui32(std::move(aDesc.map_str_ui32)),
map_str_si32(std::move(aDesc.map_str_si32)),
map_str_ui64(std::move(aDesc.map_str_ui64)),
map_str_si64(std::move(aDesc.map_str_si64)),
map_str_uts_s(std::move(aDesc.map_str_uts_s)),
map_str_uts_ms(std::move(aDesc.map_str_uts_ms)),
map_str_real_f(std::move(aDesc.map_str_real_f)),
map_str_real_d(std::move(aDesc.map_str_real_d)),
map_str_b(std::move(aDesc.map_str_b)),
map_str_uuid(std::move(aDesc.map_str_uuid)),
map_str_str(std::move(aDesc.map_str_str)),
map_str_blob(std::move(aDesc.map_str_blob)),
map_str_s2(std::move(aDesc.map_str_s2))
{
}

GpReflectDemoMapStrSDesc::~GpReflectDemoMapStrSDesc (void) noexcept
{
}

void    GpReflectDemoMapStrSDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_str_ui8);
    PROP(map_str_si8);
    PROP(map_str_ui16);
    PROP(map_str_si16);
    PROP(map_str_ui32);
    PROP(map_str_si32);
    PROP(map_str_ui64);
    PROP(map_str_si64);
    PROP(map_str_uts_s);
    PROP(map_str_uts_ms);
    PROP(map_str_real_f);
    PROP(map_str_real_d);
    PROP(map_str_b);
    PROP(map_str_uuid);
    PROP(map_str_str);
    PROP(map_str_blob);
    PROP(map_str_s2);
}

REFLECT_IMPLEMENT(GpReflectDemoMapBytesDesc, GP_MODULE_UUID)

GpReflectDemoMapBytesDesc::GpReflectDemoMapBytesDesc (void) noexcept
{
}

GpReflectDemoMapBytesDesc::GpReflectDemoMapBytesDesc (const GpReflectDemoMapBytesDesc& aDesc):
GpReflectDemoMapStrSDesc(aDesc),
map_bytes_ui8(aDesc.map_bytes_ui8),
map_bytes_si8(aDesc.map_bytes_si8),
map_bytes_ui16(aDesc.map_bytes_ui16),
map_bytes_si16(aDesc.map_bytes_si16),
map_bytes_ui32(aDesc.map_bytes_ui32),
map_bytes_si32(aDesc.map_bytes_si32),
map_bytes_ui64(aDesc.map_bytes_ui64),
map_bytes_si64(aDesc.map_bytes_si64),
map_bytes_uts_s(aDesc.map_bytes_uts_s),
map_bytes_uts_ms(aDesc.map_bytes_uts_ms),
map_bytes_real_f(aDesc.map_bytes_real_f),
map_bytes_real_d(aDesc.map_bytes_real_d),
map_bytes_b(aDesc.map_bytes_b),
map_bytes_uuid(aDesc.map_bytes_uuid),
map_bytes_str(aDesc.map_bytes_str),
map_bytes_blob(aDesc.map_bytes_blob),
map_bytes_s2(aDesc.map_bytes_s2)
{
}

GpReflectDemoMapBytesDesc::GpReflectDemoMapBytesDesc (GpReflectDemoMapBytesDesc&& aDesc) noexcept:
GpReflectDemoMapStrSDesc(std::move(aDesc)),
map_bytes_ui8(std::move(aDesc.map_bytes_ui8)),
map_bytes_si8(std::move(aDesc.map_bytes_si8)),
map_bytes_ui16(std::move(aDesc.map_bytes_ui16)),
map_bytes_si16(std::move(aDesc.map_bytes_si16)),
map_bytes_ui32(std::move(aDesc.map_bytes_ui32)),
map_bytes_si32(std::move(aDesc.map_bytes_si32)),
map_bytes_ui64(std::move(aDesc.map_bytes_ui64)),
map_bytes_si64(std::move(aDesc.map_bytes_si64)),
map_bytes_uts_s(std::move(aDesc.map_bytes_uts_s)),
map_bytes_uts_ms(std::move(aDesc.map_bytes_uts_ms)),
map_bytes_real_f(std::move(aDesc.map_bytes_real_f)),
map_bytes_real_d(std::move(aDesc.map_bytes_real_d)),
map_bytes_b(std::move(aDesc.map_bytes_b)),
map_bytes_uuid(std::move(aDesc.map_bytes_uuid)),
map_bytes_str(std::move(aDesc.map_bytes_str)),
map_bytes_blob(std::move(aDesc.map_bytes_blob)),
map_bytes_s2(std::move(aDesc.map_bytes_s2))
{
}

GpReflectDemoMapBytesDesc::~GpReflectDemoMapBytesDesc (void) noexcept
{
}

void    GpReflectDemoMapBytesDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(map_bytes_ui8);
    PROP(map_bytes_si8);
    PROP(map_bytes_ui16);
    PROP(map_bytes_si16);
    PROP(map_bytes_ui32);
    PROP(map_bytes_si32);
    PROP(map_bytes_ui64);
    PROP(map_bytes_si64);
    PROP(map_bytes_uts_s);
    PROP(map_bytes_uts_ms);
    PROP(map_bytes_real_f);
    PROP(map_bytes_real_d);
    PROP(map_bytes_b);
    PROP(map_bytes_uuid);
    PROP(map_bytes_str);
    PROP(map_bytes_blob);
    PROP(map_bytes_s2);
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
