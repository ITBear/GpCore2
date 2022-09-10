#pragma once

#include "../GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectObject.hpp"
#include "../GpReflectUtils.hpp"
#include "../GpReflectManager.hpp"

namespace GPlatform {

GP_ENUM(GP_REFLECTION_API, GpReflectDemoEnum,
    VALUE_A,
    VALUE_B,
    VALUE_C
);

using GpReflectDemoEnumFlags = GpEnumFlagsST<GpReflectDemoEnum>;

class GP_REFLECTION_API GpReflectDemoDesc: public GpReflectObject
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoDesc)
    REFLECT_DECLARE("ecc65c5f-4eb0-4e46-94fc-eeb5dd8e0c9f"_uuid)

public:
                                    GpReflectDemoDesc   (void) noexcept;
    explicit                        GpReflectDemoDesc   (const GpReflectDemoDesc& aDesc);
    explicit                        GpReflectDemoDesc   (GpReflectDemoDesc&& aDesc) noexcept;
    virtual                         ~GpReflectDemoDesc  (void) noexcept override;

public:
    u_int_8                         ui8     = 0;
    s_int_8                         si8     = 0;
    u_int_16                        ui16    = 0;
    s_int_16                        si16    = 0;
    u_int_32                        ui32    = 0;
    s_int_32                        si32    = 0;
    u_int_64                        ui64    = 0;
    s_int_64                        si64    = 0;
    unix_ts_s_t                     uts_s;
    unix_ts_ms_t                    uts_ms;
    float                           real_f  = 0.0f;
    double                          real_d  = 0.0;
    bool                            b       = true;
    GpUUID                          uuid;
    std::string                     str;
    GpBytesArray                    blob;
    GpReflectDemoEnum               e1;
    GpReflectDemoEnumFlags          ef1;
    GpReflectObject                 s1;
    GpReflectDemoDesc::SP           s2;
};

class GP_REFLECTION_API GpReflectDemoVecDesc: public GpReflectDemoDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoVecDesc)
    REFLECT_DECLARE("585dd1e2-ee08-4f74-9a1c-6530ccc69b78"_uuid)

public:
                                        GpReflectDemoVecDesc    (void) noexcept;
    explicit                            GpReflectDemoVecDesc    (const GpReflectDemoVecDesc& aDesc);
    explicit                            GpReflectDemoVecDesc    (GpReflectDemoVecDesc&& aDesc) noexcept;
    virtual                             ~GpReflectDemoVecDesc   (void) noexcept override;

public:
    GpVector<u_int_8>                   vec_ui8;
    GpVector<s_int_8>                   vec_si8;
    GpVector<u_int_16>                  vec_ui16;
    GpVector<s_int_16>                  vec_si16;
    GpVector<u_int_32>                  vec_ui32;
    GpVector<s_int_32>                  vec_si32;
    GpVector<u_int_64>                  vec_ui64;
    GpVector<s_int_64>                  vec_si64;
    GpVector<unix_ts_s_t>               vec_uts_s;
    GpVector<unix_ts_ms_t>              vec_uts_ms;
    GpVector<float>                     vec_real_f;
    GpVector<double>                    vec_real_d;
    GpVector<bool>                      vec_b;
    GpVector<GpUUID>                    vec_uuid;
    GpVector<std::string>               vec_str;
    GpVector<GpBytesArray>              vec_blob;
    GpVector<GpReflectDemoDesc::SP>     vec_s2;
};


class GP_REFLECTION_API GpReflectDemoMapUi8Desc: public GpReflectDemoVecDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapUi8Desc)
    REFLECT_DECLARE("990d3a54-9130-4e31-944a-abdfcc2b4e37"_uuid)

public:
                                                GpReflectDemoMapUi8Desc     (void) noexcept;
    explicit                                    GpReflectDemoMapUi8Desc     (const GpReflectDemoMapUi8Desc& aDesc);
    explicit                                    GpReflectDemoMapUi8Desc     (GpReflectDemoMapUi8Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi8Desc    (void) noexcept override;

public:
    GpMap<u_int_8, u_int_8, std::less<>>                map_ui8_ui8;
    GpMap<u_int_8, s_int_8, std::less<>>                map_ui8_si8;
    GpMap<u_int_8, u_int_16, std::less<>>               map_ui8_ui16;
    GpMap<u_int_8, s_int_16, std::less<>>               map_ui8_si16;
    GpMap<u_int_8, u_int_32, std::less<>>               map_ui8_ui32;
    GpMap<u_int_8, s_int_32, std::less<>>               map_ui8_si32;
    GpMap<u_int_8, u_int_64, std::less<>>               map_ui8_ui64;
    GpMap<u_int_8, s_int_64, std::less<>>               map_ui8_si64;
    GpMap<u_int_8, unix_ts_s_t, std::less<>>            map_ui8_uts_s;
    GpMap<u_int_8, unix_ts_ms_t, std::less<>>           map_ui8_uts_ms;
    GpMap<u_int_8, float, std::less<>>                  map_ui8_real_f;
    GpMap<u_int_8, double, std::less<>>                 map_ui8_real_d;
    GpMap<u_int_8, bool, std::less<>>                   map_ui8_b;
    GpMap<u_int_8, GpUUID, std::less<>>                 map_ui8_uuid;
    GpMap<u_int_8, std::string, std::less<>>            map_ui8_str;
    GpMap<u_int_8, GpBytesArray, std::less<>>           map_ui8_blob;
    GpMap<u_int_8, GpReflectDemoDesc::SP, std::less<>>  map_ui8_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi8Desc: public GpReflectDemoMapUi8Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapSi8Desc)
    REFLECT_DECLARE("fdbbf0ab-3921-4708-abbe-7ddf87d8bbc7"_uuid)

public:
                                                GpReflectDemoMapSi8Desc     (void) noexcept;
    explicit                                    GpReflectDemoMapSi8Desc     (const GpReflectDemoMapSi8Desc& aDesc);
    explicit                                    GpReflectDemoMapSi8Desc     (GpReflectDemoMapSi8Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi8Desc    (void) noexcept override;

public:
    GpMap<s_int_8, u_int_8, std::less<>>                map_si8_ui8;
    GpMap<s_int_8, s_int_8, std::less<>>                map_si8_si8;
    GpMap<s_int_8, u_int_16, std::less<>>               map_si8_ui16;
    GpMap<s_int_8, s_int_16, std::less<>>               map_si8_si16;
    GpMap<s_int_8, u_int_32, std::less<>>               map_si8_ui32;
    GpMap<s_int_8, s_int_32, std::less<>>               map_si8_si32;
    GpMap<s_int_8, u_int_64, std::less<>>               map_si8_ui64;
    GpMap<s_int_8, s_int_64, std::less<>>               map_si8_si64;
    GpMap<s_int_8, unix_ts_s_t, std::less<>>            map_si8_uts_s;
    GpMap<s_int_8, unix_ts_ms_t, std::less<>>           map_si8_uts_ms;
    GpMap<s_int_8, float, std::less<>>                  map_si8_real_f;
    GpMap<s_int_8, double, std::less<>>                 map_si8_real_d;
    GpMap<s_int_8, bool, std::less<>>                   map_si8_b;
    GpMap<s_int_8, GpUUID, std::less<>>                 map_si8_uuid;
    GpMap<s_int_8, std::string, std::less<>>            map_si8_str;
    GpMap<s_int_8, GpBytesArray, std::less<>>           map_si8_blob;
    GpMap<s_int_8, GpReflectDemoDesc::SP, std::less<>>  map_si8_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUi16Desc: public GpReflectDemoMapSi8Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapUi16Desc)
    REFLECT_DECLARE("a74ccb30-31a5-452e-9cdd-1d9e17cc91c2"_uuid)

public:
                                                GpReflectDemoMapUi16Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapUi16Desc    (const GpReflectDemoMapUi16Desc& aDesc);
    explicit                                    GpReflectDemoMapUi16Desc    (GpReflectDemoMapUi16Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi16Desc   (void) noexcept override;

public:
    GpMap<u_int_16, u_int_8, std::less<>>               map_ui16_ui8;
    GpMap<u_int_16, s_int_8, std::less<>>               map_ui16_si8;
    GpMap<u_int_16, u_int_16, std::less<>>              map_ui16_ui16;
    GpMap<u_int_16, s_int_16, std::less<>>              map_ui16_si16;
    GpMap<u_int_16, u_int_32, std::less<>>              map_ui16_ui32;
    GpMap<u_int_16, s_int_32, std::less<>>              map_ui16_si32;
    GpMap<u_int_16, u_int_64, std::less<>>              map_ui16_ui64;
    GpMap<u_int_16, s_int_64, std::less<>>              map_ui16_si64;
    GpMap<u_int_16, unix_ts_s_t, std::less<>>           map_ui16_uts_s;
    GpMap<u_int_16, unix_ts_ms_t, std::less<>>          map_ui16_uts_ms;
    GpMap<u_int_16, float, std::less<>>                 map_ui16_real_f;
    GpMap<u_int_16, double, std::less<>>                map_ui16_real_d;
    GpMap<u_int_16, bool, std::less<>>                  map_ui16_b;
    GpMap<u_int_16, GpUUID, std::less<>>                map_ui16_uuid;
    GpMap<u_int_16, std::string, std::less<>>           map_ui16_str;
    GpMap<u_int_16, GpBytesArray, std::less<>>          map_ui16_blob;
    GpMap<u_int_16, GpReflectDemoDesc::SP, std::less<>> map_ui16_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi16Desc: public GpReflectDemoMapUi16Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapSi16Desc)
    REFLECT_DECLARE("3ad98875-3cfa-4491-a96e-27a50d8e5520"_uuid)

public:
                                                GpReflectDemoMapSi16Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapSi16Desc    (const GpReflectDemoMapSi16Desc& aDesc);
    explicit                                    GpReflectDemoMapSi16Desc    (GpReflectDemoMapSi16Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi16Desc   (void) noexcept override;

public:
    GpMap<s_int_16, u_int_8, std::less<>>               map_si16_ui8;
    GpMap<s_int_16, s_int_8, std::less<>>               map_si16_si8;
    GpMap<s_int_16, u_int_16, std::less<>>              map_si16_ui16;
    GpMap<s_int_16, s_int_16, std::less<>>              map_si16_si16;
    GpMap<s_int_16, u_int_32, std::less<>>              map_si16_ui32;
    GpMap<s_int_16, s_int_32, std::less<>>              map_si16_si32;
    GpMap<s_int_16, u_int_64, std::less<>>              map_si16_ui64;
    GpMap<s_int_16, s_int_64, std::less<>>              map_si16_si64;
    GpMap<s_int_16, unix_ts_s_t, std::less<>>           map_si16_uts_s;
    GpMap<s_int_16, unix_ts_ms_t, std::less<>>          map_si16_uts_ms;
    GpMap<s_int_16, float, std::less<>>                 map_si16_real_f;
    GpMap<s_int_16, double, std::less<>>                map_si16_real_d;
    GpMap<s_int_16, bool, std::less<>>                  map_si16_b;
    GpMap<s_int_16, GpUUID, std::less<>>                map_si16_uuid;
    GpMap<s_int_16, std::string, std::less<>>           map_si16_str;
    GpMap<s_int_16, GpBytesArray, std::less<>>          map_si16_blob;
    GpMap<s_int_16, GpReflectDemoDesc::SP, std::less<>> map_si16_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUi32Desc: public GpReflectDemoMapSi16Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapUi32Desc)
    REFLECT_DECLARE("ccc6bd0b-b1c4-4874-9483-d2cadea615db"_uuid)

public:
                                                GpReflectDemoMapUi32Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapUi32Desc    (const GpReflectDemoMapUi32Desc& aDesc);
    explicit                                    GpReflectDemoMapUi32Desc    (GpReflectDemoMapUi32Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi32Desc   (void) noexcept override;

public:
    GpMap<u_int_32, u_int_8, std::less<>>               map_ui32_ui8;
    GpMap<u_int_32, s_int_8, std::less<>>               map_ui32_si8;
    GpMap<u_int_32, u_int_16, std::less<>>              map_ui32_ui16;
    GpMap<u_int_32, s_int_16, std::less<>>              map_ui32_si16;
    GpMap<u_int_32, u_int_32, std::less<>>              map_ui32_ui32;
    GpMap<u_int_32, s_int_32, std::less<>>              map_ui32_si32;
    GpMap<u_int_32, u_int_64, std::less<>>              map_ui32_ui64;
    GpMap<u_int_32, s_int_64, std::less<>>              map_ui32_si64;
    GpMap<u_int_32, unix_ts_s_t, std::less<>>           map_ui32_uts_s;
    GpMap<u_int_32, unix_ts_ms_t, std::less<>>          map_ui32_uts_ms;
    GpMap<u_int_32, float, std::less<>>                 map_ui32_real_f;
    GpMap<u_int_32, double, std::less<>>                map_ui32_real_d;
    GpMap<u_int_32, bool, std::less<>>                  map_ui32_b;
    GpMap<u_int_32, GpUUID, std::less<>>                map_ui32_uuid;
    GpMap<u_int_32, std::string, std::less<>>           map_ui32_str;
    GpMap<u_int_32, GpBytesArray, std::less<>>          map_ui32_blob;
    GpMap<u_int_32, GpReflectDemoDesc::SP, std::less<>> map_ui32_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi32Desc: public GpReflectDemoMapUi32Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapSi32Desc)
    REFLECT_DECLARE("adca4efa-2374-4a67-ad73-070d502673e6"_uuid)

public:
                                                GpReflectDemoMapSi32Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapSi32Desc    (const GpReflectDemoMapSi32Desc& aDesc);
    explicit                                    GpReflectDemoMapSi32Desc    (GpReflectDemoMapSi32Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi32Desc   (void) noexcept override;

public:
    GpMap<s_int_32, u_int_8, std::less<>>               map_si32_ui8;
    GpMap<s_int_32, s_int_8, std::less<>>               map_si32_si8;
    GpMap<s_int_32, u_int_16, std::less<>>              map_si32_ui16;
    GpMap<s_int_32, s_int_16, std::less<>>              map_si32_si16;
    GpMap<s_int_32, u_int_32, std::less<>>              map_si32_ui32;
    GpMap<s_int_32, s_int_32, std::less<>>              map_si32_si32;
    GpMap<s_int_32, u_int_64, std::less<>>              map_si32_ui64;
    GpMap<s_int_32, s_int_64, std::less<>>              map_si32_si64;
    GpMap<s_int_32, unix_ts_s_t, std::less<>>           map_si32_uts_s;
    GpMap<s_int_32, unix_ts_ms_t, std::less<>>          map_si32_uts_ms;
    GpMap<s_int_32, float, std::less<>>                 map_si32_real_f;
    GpMap<s_int_32, double, std::less<>>                map_si32_real_d;
    GpMap<s_int_32, bool, std::less<>>                  map_si32_b;
    GpMap<s_int_32, GpUUID, std::less<>>                map_si32_uuid;
    GpMap<s_int_32, std::string, std::less<>>           map_si32_str;
    GpMap<s_int_32, GpBytesArray, std::less<>>          map_si32_blob;
    GpMap<s_int_32, GpReflectDemoDesc::SP, std::less<>> map_si32_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUi64Desc: public GpReflectDemoMapSi32Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapUi64Desc)
    REFLECT_DECLARE("d63a6bde-e930-46f6-9dcd-fa7611eb95cf"_uuid)

public:
                                                GpReflectDemoMapUi64Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapUi64Desc    (const GpReflectDemoMapUi64Desc& aDesc);
    explicit                                    GpReflectDemoMapUi64Desc    (GpReflectDemoMapUi64Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi64Desc   (void) noexcept override;

public:
    GpMap<u_int_64, u_int_8, std::less<>>               map_ui64_ui8;
    GpMap<u_int_64, s_int_8, std::less<>>               map_ui64_si8;
    GpMap<u_int_64, u_int_16, std::less<>>              map_ui64_ui16;
    GpMap<u_int_64, s_int_16, std::less<>>              map_ui64_si16;
    GpMap<u_int_64, u_int_32, std::less<>>              map_ui64_ui32;
    GpMap<u_int_64, s_int_32, std::less<>>              map_ui64_si32;
    GpMap<u_int_64, u_int_64, std::less<>>              map_ui64_ui64;
    GpMap<u_int_64, s_int_64, std::less<>>              map_ui64_si64;
    GpMap<u_int_64, unix_ts_s_t, std::less<>>           map_ui64_uts_s;
    GpMap<u_int_64, unix_ts_ms_t, std::less<>>          map_ui64_uts_ms;
    GpMap<u_int_64, float, std::less<>>                 map_ui64_real_f;
    GpMap<u_int_64, double, std::less<>>                map_ui64_real_d;
    GpMap<u_int_64, bool, std::less<>>                  map_ui64_b;
    GpMap<u_int_64, GpUUID, std::less<>>                map_ui64_uuid;
    GpMap<u_int_64, std::string, std::less<>>           map_ui64_str;
    GpMap<u_int_64, GpBytesArray, std::less<>>          map_ui64_blob;
    GpMap<u_int_64, GpReflectDemoDesc::SP, std::less<>> map_ui64_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi64Desc: public GpReflectDemoMapUi64Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapSi64Desc)
    REFLECT_DECLARE("0bfe8089-51db-41ec-8a89-3c5aaf7d6e5e"_uuid)

public:
                                                GpReflectDemoMapSi64Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapSi64Desc    (const GpReflectDemoMapSi64Desc& aDesc);
    explicit                                    GpReflectDemoMapSi64Desc    (GpReflectDemoMapSi64Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi64Desc   (void) noexcept override;

public:
    GpMap<s_int_64, u_int_8, std::less<>>               map_si64_ui8;
    GpMap<s_int_64, s_int_8, std::less<>>               map_si64_si8;
    GpMap<s_int_64, u_int_16, std::less<>>              map_si64_ui16;
    GpMap<s_int_64, s_int_16, std::less<>>              map_si64_si16;
    GpMap<s_int_64, u_int_32, std::less<>>              map_si64_ui32;
    GpMap<s_int_64, s_int_32, std::less<>>              map_si64_si32;
    GpMap<s_int_64, u_int_64, std::less<>>              map_si64_ui64;
    GpMap<s_int_64, s_int_64, std::less<>>              map_si64_si64;
    GpMap<s_int_64, unix_ts_s_t, std::less<>>           map_si64_uts_s;
    GpMap<s_int_64, unix_ts_ms_t, std::less<>>          map_si64_uts_ms;
    GpMap<s_int_64, float, std::less<>>                 map_si64_real_f;
    GpMap<s_int_64, double, std::less<>>                map_si64_real_d;
    GpMap<s_int_64, bool, std::less<>>                  map_si64_b;
    GpMap<s_int_64, GpUUID, std::less<>>                map_si64_uuid;
    GpMap<s_int_64, std::string, std::less<>>           map_si64_str;
    GpMap<s_int_64, GpBytesArray, std::less<>>          map_si64_blob;
    GpMap<s_int_64, GpReflectDemoDesc::SP, std::less<>> map_si64_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUtsSDesc: public GpReflectDemoMapSi64Desc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapUtsSDesc)
    REFLECT_DECLARE("b01305f3-84e6-4cf4-b23e-fddc73c5a2bb"_uuid)

public:
                                                GpReflectDemoMapUtsSDesc    (void) noexcept;
    explicit                                    GpReflectDemoMapUtsSDesc    (const GpReflectDemoMapUtsSDesc& aDesc);
    explicit                                    GpReflectDemoMapUtsSDesc    (GpReflectDemoMapUtsSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUtsSDesc   (void) noexcept override;

public:
    GpMap<unix_ts_s_t, u_int_8, std::less<>>                map_utss_ui8;
    GpMap<unix_ts_s_t, s_int_8, std::less<>>                map_utss_si8;
    GpMap<unix_ts_s_t, u_int_16, std::less<>>               map_utss_ui16;
    GpMap<unix_ts_s_t, s_int_16, std::less<>>               map_utss_si16;
    GpMap<unix_ts_s_t, u_int_32, std::less<>>               map_utss_ui32;
    GpMap<unix_ts_s_t, s_int_32, std::less<>>               map_utss_si32;
    GpMap<unix_ts_s_t, u_int_64, std::less<>>               map_utss_ui64;
    GpMap<unix_ts_s_t, s_int_64, std::less<>>               map_utss_si64;
    GpMap<unix_ts_s_t, unix_ts_s_t, std::less<>>            map_utss_uts_s;
    GpMap<unix_ts_s_t, unix_ts_ms_t, std::less<>>           map_utss_uts_ms;
    GpMap<unix_ts_s_t, float, std::less<>>                  map_utss_real_f;
    GpMap<unix_ts_s_t, double, std::less<>>                 map_utss_real_d;
    GpMap<unix_ts_s_t, bool, std::less<>>                   map_utss_b;
    GpMap<unix_ts_s_t, GpUUID, std::less<>>                 map_utss_uuid;
    GpMap<unix_ts_s_t, std::string, std::less<>>            map_utss_str;
    GpMap<unix_ts_s_t, GpBytesArray, std::less<>>           map_utss_blob;
    GpMap<unix_ts_s_t, GpReflectDemoDesc::SP, std::less<>>  map_utss_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUtmsSDesc: public GpReflectDemoMapUtsSDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapUtmsSDesc)
    REFLECT_DECLARE("4079fc31-aa72-4551-aad9-cfc892ee8c91"_uuid)

public:
                                                GpReflectDemoMapUtmsSDesc   (void) noexcept;
    explicit                                    GpReflectDemoMapUtmsSDesc   (const GpReflectDemoMapUtmsSDesc& aDesc);
    explicit                                    GpReflectDemoMapUtmsSDesc   (GpReflectDemoMapUtmsSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUtmsSDesc  (void) noexcept override;

public:
    GpMap<unix_ts_ms_t, u_int_8, std::less<>>               map_utsms_ui8;
    GpMap<unix_ts_ms_t, s_int_8, std::less<>>               map_utsms_si8;
    GpMap<unix_ts_ms_t, u_int_16, std::less<>>              map_utsms_ui16;
    GpMap<unix_ts_ms_t, s_int_16, std::less<>>              map_utsms_si16;
    GpMap<unix_ts_ms_t, u_int_32, std::less<>>              map_utsms_ui32;
    GpMap<unix_ts_ms_t, s_int_32, std::less<>>              map_utsms_si32;
    GpMap<unix_ts_ms_t, u_int_64, std::less<>>              map_utsms_ui64;
    GpMap<unix_ts_ms_t, s_int_64, std::less<>>              map_utsms_si64;
    GpMap<unix_ts_ms_t, unix_ts_s_t, std::less<>>           map_utsms_uts_s;
    GpMap<unix_ts_ms_t, unix_ts_ms_t, std::less<>>          map_utsms_uts_ms;
    GpMap<unix_ts_ms_t, float, std::less<>>                 map_utsms_real_f;
    GpMap<unix_ts_ms_t, double, std::less<>>                map_utsms_real_d;
    GpMap<unix_ts_ms_t, bool, std::less<>>                  map_utsms_b;
    GpMap<unix_ts_ms_t, GpUUID, std::less<>>                map_utsms_uuid;
    GpMap<unix_ts_ms_t, std::string, std::less<>>           map_utsms_str;
    GpMap<unix_ts_ms_t, GpBytesArray, std::less<>>          map_utsms_blob;
    GpMap<unix_ts_ms_t, GpReflectDemoDesc::SP, std::less<>> map_utsms_s2;
};

class GP_REFLECTION_API GpReflectDemoMapFloatSDesc: public GpReflectDemoMapUtmsSDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapFloatSDesc)
    REFLECT_DECLARE("24d7e9d7-24f8-4a58-8227-ebc2fe3ada65"_uuid)

public:
                                                GpReflectDemoMapFloatSDesc  (void) noexcept;
    explicit                                    GpReflectDemoMapFloatSDesc  (const GpReflectDemoMapFloatSDesc& aDesc);
    explicit                                    GpReflectDemoMapFloatSDesc  (GpReflectDemoMapFloatSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapFloatSDesc (void) noexcept override;

public:
    GpMap<float, u_int_8, std::less<>>                  map_float_ui8;
    GpMap<float, s_int_8, std::less<>>                  map_float_si8;
    GpMap<float, u_int_16, std::less<>>                 map_float_ui16;
    GpMap<float, s_int_16, std::less<>>                 map_float_si16;
    GpMap<float, u_int_32, std::less<>>                 map_float_ui32;
    GpMap<float, s_int_32, std::less<>>                 map_float_si32;
    GpMap<float, u_int_64, std::less<>>                 map_float_ui64;
    GpMap<float, s_int_64, std::less<>>                 map_float_si64;
    GpMap<float, unix_ts_s_t, std::less<>>              map_float_uts_s;
    GpMap<float, unix_ts_ms_t, std::less<>>             map_float_uts_ms;
    GpMap<float, float, std::less<>>                    map_float_real_f;
    GpMap<float, double, std::less<>>                   map_float_real_d;
    GpMap<float, bool, std::less<>>                     map_float_b;
    GpMap<float, GpUUID, std::less<>>                   map_float_uuid;
    GpMap<float, std::string, std::less<>>              map_float_str;
    GpMap<float, GpBytesArray, std::less<>>             map_float_blob;
    GpMap<float, GpReflectDemoDesc::SP, std::less<>>    map_float_s2;
};

class GP_REFLECTION_API GpReflectDemoMapDoubleSDesc: public GpReflectDemoMapFloatSDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapDoubleSDesc)
    REFLECT_DECLARE("67bfbbfc-e02c-43d7-8e3f-9110604c6418"_uuid)

public:
                                                GpReflectDemoMapDoubleSDesc (void) noexcept;
    explicit                                    GpReflectDemoMapDoubleSDesc (const GpReflectDemoMapDoubleSDesc& aDesc);
    explicit                                    GpReflectDemoMapDoubleSDesc (GpReflectDemoMapDoubleSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapDoubleSDesc(void) noexcept override;

public:
    GpMap<double, u_int_8, std::less<>>                 map_double_ui8;
    GpMap<double, s_int_8, std::less<>>                 map_double_si8;
    GpMap<double, u_int_16, std::less<>>                map_double_ui16;
    GpMap<double, s_int_16, std::less<>>                map_double_si16;
    GpMap<double, u_int_32, std::less<>>                map_double_ui32;
    GpMap<double, s_int_32, std::less<>>                map_double_si32;
    GpMap<double, u_int_64, std::less<>>                map_double_ui64;
    GpMap<double, s_int_64, std::less<>>                map_double_si64;
    GpMap<double, unix_ts_s_t, std::less<>>             map_double_uts_s;
    GpMap<double, unix_ts_ms_t, std::less<>>            map_double_uts_ms;
    GpMap<double, float, std::less<>>                   map_double_real_f;
    GpMap<double, double, std::less<>>                  map_double_real_d;
    GpMap<double, bool, std::less<>>                    map_double_b;
    GpMap<double, GpUUID, std::less<>>                  map_double_uuid;
    GpMap<double, std::string, std::less<>>             map_double_str;
    GpMap<double, GpBytesArray, std::less<>>            map_double_blob;
    GpMap<double, GpReflectDemoDesc::SP, std::less<>>   map_double_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUuidSDesc: public GpReflectDemoMapDoubleSDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapUuidSDesc)
    REFLECT_DECLARE("d0d7bbb0-b28a-48cc-99a5-71b7d1d015ea"_uuid)

public:
                                                GpReflectDemoMapUuidSDesc   (void) noexcept;
    explicit                                    GpReflectDemoMapUuidSDesc   (const GpReflectDemoMapUuidSDesc& aDesc);
    explicit                                    GpReflectDemoMapUuidSDesc   (GpReflectDemoMapUuidSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUuidSDesc  (void) noexcept override;

public:
    GpMap<GpUUID, u_int_8, std::less<>>                 map_uuid_ui8;
    GpMap<GpUUID, s_int_8, std::less<>>                 map_uuid_si8;
    GpMap<GpUUID, u_int_16, std::less<>>                map_uuid_ui16;
    GpMap<GpUUID, s_int_16, std::less<>>                map_uuid_si16;
    GpMap<GpUUID, u_int_32, std::less<>>                map_uuid_ui32;
    GpMap<GpUUID, s_int_32, std::less<>>                map_uuid_si32;
    GpMap<GpUUID, u_int_64, std::less<>>                map_uuid_ui64;
    GpMap<GpUUID, s_int_64, std::less<>>                map_uuid_si64;
    GpMap<GpUUID, unix_ts_s_t, std::less<>>             map_uuid_uts_s;
    GpMap<GpUUID, unix_ts_ms_t, std::less<>>            map_uuid_uts_ms;
    GpMap<GpUUID, float, std::less<>>                   map_uuid_real_f;
    GpMap<GpUUID, double, std::less<>>                  map_uuid_real_d;
    GpMap<GpUUID, bool, std::less<>>                    map_uuid_b;
    GpMap<GpUUID, GpUUID, std::less<>>                  map_uuid_uuid;
    GpMap<GpUUID, std::string, std::less<>>             map_uuid_str;
    GpMap<GpUUID, GpBytesArray, std::less<>>            map_uuid_blob;
    GpMap<GpUUID, GpReflectDemoDesc::SP, std::less<>>   map_uuid_s2;
};

class GP_REFLECTION_API GpReflectDemoMapStrSDesc: public GpReflectDemoMapUuidSDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapStrSDesc)
    REFLECT_DECLARE("92dbbbad-0e3b-4f09-8922-4335b178a6e6"_uuid)

public:
                                                GpReflectDemoMapStrSDesc    (void) noexcept;
    explicit                                    GpReflectDemoMapStrSDesc    (const GpReflectDemoMapStrSDesc& aDesc);
    explicit                                    GpReflectDemoMapStrSDesc    (GpReflectDemoMapStrSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapStrSDesc   (void) noexcept override;

public:
    GpMap<std::string, u_int_8, std::less<>>                map_str_ui8;
    GpMap<std::string, s_int_8, std::less<>>                map_str_si8;
    GpMap<std::string, u_int_16, std::less<>>               map_str_ui16;
    GpMap<std::string, s_int_16, std::less<>>               map_str_si16;
    GpMap<std::string, u_int_32, std::less<>>               map_str_ui32;
    GpMap<std::string, s_int_32, std::less<>>               map_str_si32;
    GpMap<std::string, u_int_64, std::less<>>               map_str_ui64;
    GpMap<std::string, s_int_64, std::less<>>               map_str_si64;
    GpMap<std::string, unix_ts_s_t, std::less<>>            map_str_uts_s;
    GpMap<std::string, unix_ts_ms_t, std::less<>>           map_str_uts_ms;
    GpMap<std::string, float, std::less<>>                  map_str_real_f;
    GpMap<std::string, double, std::less<>>                 map_str_real_d;
    GpMap<std::string, bool, std::less<>>                   map_str_b;
    GpMap<std::string, GpUUID, std::less<>>                 map_str_uuid;
    GpMap<std::string, std::string, std::less<>>            map_str_str;
    GpMap<std::string, GpBytesArray, std::less<>>           map_str_blob;
    GpMap<std::string, GpReflectDemoDesc::SP, std::less<>>  map_str_s2;
};

class GP_REFLECTION_API GpReflectDemoMapBytesDesc: public GpReflectDemoMapStrSDesc
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectDemoMapBytesDesc)
    REFLECT_DECLARE("e3c574d3-1ffe-4e60-a809-b0903f2a7beb"_uuid)

public:
                                                GpReflectDemoMapBytesDesc   (void) noexcept;
    explicit                                    GpReflectDemoMapBytesDesc   (const GpReflectDemoMapBytesDesc& aDesc);
    explicit                                    GpReflectDemoMapBytesDesc   (GpReflectDemoMapBytesDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapBytesDesc  (void) noexcept override;

public:
    GpMap<GpBytesArray, u_int_8, std::less<>>               map_bytes_ui8;
    GpMap<GpBytesArray, s_int_8, std::less<>>               map_bytes_si8;
    GpMap<GpBytesArray, u_int_16, std::less<>>              map_bytes_ui16;
    GpMap<GpBytesArray, s_int_16, std::less<>>              map_bytes_si16;
    GpMap<GpBytesArray, u_int_32, std::less<>>              map_bytes_ui32;
    GpMap<GpBytesArray, s_int_32, std::less<>>              map_bytes_si32;
    GpMap<GpBytesArray, u_int_64, std::less<>>              map_bytes_ui64;
    GpMap<GpBytesArray, s_int_64, std::less<>>              map_bytes_si64;
    GpMap<GpBytesArray, unix_ts_s_t, std::less<>>           map_bytes_uts_s;
    GpMap<GpBytesArray, unix_ts_ms_t, std::less<>>          map_bytes_uts_ms;
    GpMap<GpBytesArray, float, std::less<>>                 map_bytes_real_f;
    GpMap<GpBytesArray, double, std::less<>>                map_bytes_real_d;
    GpMap<GpBytesArray, bool, std::less<>>                  map_bytes_b;
    GpMap<GpBytesArray, GpUUID, std::less<>>                map_bytes_uuid;
    GpMap<GpBytesArray, std::string, std::less<>>           map_bytes_str;
    GpMap<GpBytesArray, GpBytesArray, std::less<>>          map_bytes_blob;
    GpMap<GpBytesArray, GpReflectDemoDesc::SP, std::less<>> map_bytes_s2;
};

}//namespace GPlatform

#endif//GP_USE_REFLECTION
