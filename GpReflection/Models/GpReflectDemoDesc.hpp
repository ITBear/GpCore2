#pragma once

#include "../../Config/GpConfig.hpp"

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
    CLASS_DD(GpReflectDemoDesc)
    REFLECT_DECLARE(u8"ecc65c5f-4eb0-4e46-94fc-eeb5dd8e0c9f"_uuid)

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
    std::u8string                   str;
    GpBytesArray                    blob;
    GpReflectDemoEnum               e1;
    GpReflectDemoEnumFlags          ef1;
    GpReflectObject                 s1;
    GpReflectDemoDesc::SP           s2;
};

class GP_REFLECTION_API GpReflectDemoVecDesc: public GpReflectDemoDesc
{
public:
    CLASS_DD(GpReflectDemoVecDesc)
    REFLECT_DECLARE(u8"585dd1e2-ee08-4f74-9a1c-6530ccc69b78"_uuid)

public:
                                        GpReflectDemoVecDesc    (void) noexcept;
    explicit                            GpReflectDemoVecDesc    (const GpReflectDemoVecDesc& aDesc);
    explicit                            GpReflectDemoVecDesc    (GpReflectDemoVecDesc&& aDesc) noexcept;
    virtual                             ~GpReflectDemoVecDesc   (void) noexcept override;

public:
    std::vector<u_int_8>                vec_ui8;
    std::vector<s_int_8>                vec_si8;
    std::vector<u_int_16>               vec_ui16;
    std::vector<s_int_16>               vec_si16;
    std::vector<u_int_32>               vec_ui32;
    std::vector<s_int_32>               vec_si32;
    std::vector<u_int_64>               vec_ui64;
    std::vector<s_int_64>               vec_si64;
    std::vector<unix_ts_s_t>            vec_uts_s;
    std::vector<unix_ts_ms_t>           vec_uts_ms;
    std::vector<float>                  vec_real_f;
    std::vector<double>                 vec_real_d;
    std::vector<bool>                   vec_b;
    std::vector<GpUUID>                 vec_uuid;
    std::vector<std::u8string>          vec_str;
    std::vector<GpBytesArray>           vec_blob;
    std::vector<GpReflectDemoDesc::SP>  vec_s2;
};


class GP_REFLECTION_API GpReflectDemoMapUi8Desc: public GpReflectDemoVecDesc
{
public:
    CLASS_DD(GpReflectDemoMapUi8Desc)
    REFLECT_DECLARE(u8"990d3a54-9130-4e31-944a-abdfcc2b4e37"_uuid)

public:
                                                GpReflectDemoMapUi8Desc     (void) noexcept;
    explicit                                    GpReflectDemoMapUi8Desc     (const GpReflectDemoMapUi8Desc& aDesc);
    explicit                                    GpReflectDemoMapUi8Desc     (GpReflectDemoMapUi8Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi8Desc    (void) noexcept override;

public:
    std::map<u_int_8, u_int_8, std::less<>>                 map_ui8_ui8;
    std::map<u_int_8, s_int_8, std::less<>>                 map_ui8_si8;
    std::map<u_int_8, u_int_16, std::less<>>                map_ui8_ui16;
    std::map<u_int_8, s_int_16, std::less<>>                map_ui8_si16;
    std::map<u_int_8, u_int_32, std::less<>>                map_ui8_ui32;
    std::map<u_int_8, s_int_32, std::less<>>                map_ui8_si32;
    std::map<u_int_8, u_int_64, std::less<>>                map_ui8_ui64;
    std::map<u_int_8, s_int_64, std::less<>>                map_ui8_si64;
    std::map<u_int_8, unix_ts_s_t, std::less<>>             map_ui8_uts_s;
    std::map<u_int_8, unix_ts_ms_t, std::less<>>            map_ui8_uts_ms;
    std::map<u_int_8, float, std::less<>>                   map_ui8_real_f;
    std::map<u_int_8, double, std::less<>>                  map_ui8_real_d;
    std::map<u_int_8, bool, std::less<>>                    map_ui8_b;
    std::map<u_int_8, GpUUID, std::less<>>                  map_ui8_uuid;
    std::map<u_int_8, std::u8string, std::less<>>               map_ui8_str;
    std::map<u_int_8, GpBytesArray, std::less<>>            map_ui8_blob;
    std::map<u_int_8, GpReflectDemoDesc::SP, std::less<>>   map_ui8_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi8Desc: public GpReflectDemoMapUi8Desc
{
public:
    CLASS_DD(GpReflectDemoMapSi8Desc)
    REFLECT_DECLARE(u8"fdbbf0ab-3921-4708-abbe-7ddf87d8bbc7"_uuid)

public:
                                                GpReflectDemoMapSi8Desc     (void) noexcept;
    explicit                                    GpReflectDemoMapSi8Desc     (const GpReflectDemoMapSi8Desc& aDesc);
    explicit                                    GpReflectDemoMapSi8Desc     (GpReflectDemoMapSi8Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi8Desc    (void) noexcept override;

public:
    std::map<s_int_8, u_int_8, std::less<>>             map_si8_ui8;
    std::map<s_int_8, s_int_8, std::less<>>             map_si8_si8;
    std::map<s_int_8, u_int_16, std::less<>>                map_si8_ui16;
    std::map<s_int_8, s_int_16, std::less<>>                map_si8_si16;
    std::map<s_int_8, u_int_32, std::less<>>                map_si8_ui32;
    std::map<s_int_8, s_int_32, std::less<>>                map_si8_si32;
    std::map<s_int_8, u_int_64, std::less<>>                map_si8_ui64;
    std::map<s_int_8, s_int_64, std::less<>>                map_si8_si64;
    std::map<s_int_8, unix_ts_s_t, std::less<>>         map_si8_uts_s;
    std::map<s_int_8, unix_ts_ms_t, std::less<>>            map_si8_uts_ms;
    std::map<s_int_8, float, std::less<>>                   map_si8_real_f;
    std::map<s_int_8, double, std::less<>>                  map_si8_real_d;
    std::map<s_int_8, bool, std::less<>>                    map_si8_b;
    std::map<s_int_8, GpUUID, std::less<>>                  map_si8_uuid;
    std::map<s_int_8, std::u8string, std::less<>>           map_si8_str;
    std::map<s_int_8, GpBytesArray, std::less<>>            map_si8_blob;
    std::map<s_int_8, GpReflectDemoDesc::SP, std::less<>>   map_si8_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUi16Desc: public GpReflectDemoMapSi8Desc
{
public:
    CLASS_DD(GpReflectDemoMapUi16Desc)
    REFLECT_DECLARE(u8"a74ccb30-31a5-452e-9cdd-1d9e17cc91c2"_uuid)

public:
                                                GpReflectDemoMapUi16Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapUi16Desc    (const GpReflectDemoMapUi16Desc& aDesc);
    explicit                                    GpReflectDemoMapUi16Desc    (GpReflectDemoMapUi16Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi16Desc   (void) noexcept override;

public:
    std::map<u_int_16, u_int_8, std::less<>>                map_ui16_ui8;
    std::map<u_int_16, s_int_8, std::less<>>                map_ui16_si8;
    std::map<u_int_16, u_int_16, std::less<>>               map_ui16_ui16;
    std::map<u_int_16, s_int_16, std::less<>>               map_ui16_si16;
    std::map<u_int_16, u_int_32, std::less<>>               map_ui16_ui32;
    std::map<u_int_16, s_int_32, std::less<>>               map_ui16_si32;
    std::map<u_int_16, u_int_64, std::less<>>               map_ui16_ui64;
    std::map<u_int_16, s_int_64, std::less<>>               map_ui16_si64;
    std::map<u_int_16, unix_ts_s_t, std::less<>>            map_ui16_uts_s;
    std::map<u_int_16, unix_ts_ms_t, std::less<>>           map_ui16_uts_ms;
    std::map<u_int_16, float, std::less<>>                  map_ui16_real_f;
    std::map<u_int_16, double, std::less<>>             map_ui16_real_d;
    std::map<u_int_16, bool, std::less<>>                   map_ui16_b;
    std::map<u_int_16, GpUUID, std::less<>>             map_ui16_uuid;
    std::map<u_int_16, std::u8string, std::less<>>          map_ui16_str;
    std::map<u_int_16, GpBytesArray, std::less<>>           map_ui16_blob;
    std::map<u_int_16, GpReflectDemoDesc::SP, std::less<>>  map_ui16_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi16Desc: public GpReflectDemoMapUi16Desc
{
public:
    CLASS_DD(GpReflectDemoMapSi16Desc)
    REFLECT_DECLARE(u8"3ad98875-3cfa-4491-a96e-27a50d8e5520"_uuid)

public:
                                                GpReflectDemoMapSi16Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapSi16Desc    (const GpReflectDemoMapSi16Desc& aDesc);
    explicit                                    GpReflectDemoMapSi16Desc    (GpReflectDemoMapSi16Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi16Desc   (void) noexcept override;

public:
    std::map<s_int_16, u_int_8, std::less<>>                map_si16_ui8;
    std::map<s_int_16, s_int_8, std::less<>>                map_si16_si8;
    std::map<s_int_16, u_int_16, std::less<>>               map_si16_ui16;
    std::map<s_int_16, s_int_16, std::less<>>               map_si16_si16;
    std::map<s_int_16, u_int_32, std::less<>>               map_si16_ui32;
    std::map<s_int_16, s_int_32, std::less<>>               map_si16_si32;
    std::map<s_int_16, u_int_64, std::less<>>               map_si16_ui64;
    std::map<s_int_16, s_int_64, std::less<>>               map_si16_si64;
    std::map<s_int_16, unix_ts_s_t, std::less<>>            map_si16_uts_s;
    std::map<s_int_16, unix_ts_ms_t, std::less<>>           map_si16_uts_ms;
    std::map<s_int_16, float, std::less<>>                  map_si16_real_f;
    std::map<s_int_16, double, std::less<>>             map_si16_real_d;
    std::map<s_int_16, bool, std::less<>>                   map_si16_b;
    std::map<s_int_16, GpUUID, std::less<>>             map_si16_uuid;
    std::map<s_int_16, std::u8string, std::less<>>          map_si16_str;
    std::map<s_int_16, GpBytesArray, std::less<>>           map_si16_blob;
    std::map<s_int_16, GpReflectDemoDesc::SP, std::less<>>  map_si16_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUi32Desc: public GpReflectDemoMapSi16Desc
{
public:
    CLASS_DD(GpReflectDemoMapUi32Desc)
    REFLECT_DECLARE(u8"ccc6bd0b-b1c4-4874-9483-d2cadea615db"_uuid)

public:
                                                GpReflectDemoMapUi32Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapUi32Desc    (const GpReflectDemoMapUi32Desc& aDesc);
    explicit                                    GpReflectDemoMapUi32Desc    (GpReflectDemoMapUi32Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi32Desc   (void) noexcept override;

public:
    std::map<u_int_32, u_int_8, std::less<>>                map_ui32_ui8;
    std::map<u_int_32, s_int_8, std::less<>>                map_ui32_si8;
    std::map<u_int_32, u_int_16, std::less<>>               map_ui32_ui16;
    std::map<u_int_32, s_int_16, std::less<>>               map_ui32_si16;
    std::map<u_int_32, u_int_32, std::less<>>               map_ui32_ui32;
    std::map<u_int_32, s_int_32, std::less<>>               map_ui32_si32;
    std::map<u_int_32, u_int_64, std::less<>>               map_ui32_ui64;
    std::map<u_int_32, s_int_64, std::less<>>               map_ui32_si64;
    std::map<u_int_32, unix_ts_s_t, std::less<>>            map_ui32_uts_s;
    std::map<u_int_32, unix_ts_ms_t, std::less<>>           map_ui32_uts_ms;
    std::map<u_int_32, float, std::less<>>                  map_ui32_real_f;
    std::map<u_int_32, double, std::less<>>             map_ui32_real_d;
    std::map<u_int_32, bool, std::less<>>                   map_ui32_b;
    std::map<u_int_32, GpUUID, std::less<>>             map_ui32_uuid;
    std::map<u_int_32, std::u8string, std::less<>>          map_ui32_str;
    std::map<u_int_32, GpBytesArray, std::less<>>           map_ui32_blob;
    std::map<u_int_32, GpReflectDemoDesc::SP, std::less<>>  map_ui32_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi32Desc: public GpReflectDemoMapUi32Desc
{
public:
    CLASS_DD(GpReflectDemoMapSi32Desc)
    REFLECT_DECLARE(u8"adca4efa-2374-4a67-ad73-070d502673e6"_uuid)

public:
                                                GpReflectDemoMapSi32Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapSi32Desc    (const GpReflectDemoMapSi32Desc& aDesc);
    explicit                                    GpReflectDemoMapSi32Desc    (GpReflectDemoMapSi32Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi32Desc   (void) noexcept override;

public:
    std::map<s_int_32, u_int_8, std::less<>>                map_si32_ui8;
    std::map<s_int_32, s_int_8, std::less<>>                map_si32_si8;
    std::map<s_int_32, u_int_16, std::less<>>               map_si32_ui16;
    std::map<s_int_32, s_int_16, std::less<>>               map_si32_si16;
    std::map<s_int_32, u_int_32, std::less<>>               map_si32_ui32;
    std::map<s_int_32, s_int_32, std::less<>>               map_si32_si32;
    std::map<s_int_32, u_int_64, std::less<>>               map_si32_ui64;
    std::map<s_int_32, s_int_64, std::less<>>               map_si32_si64;
    std::map<s_int_32, unix_ts_s_t, std::less<>>            map_si32_uts_s;
    std::map<s_int_32, unix_ts_ms_t, std::less<>>           map_si32_uts_ms;
    std::map<s_int_32, float, std::less<>>                  map_si32_real_f;
    std::map<s_int_32, double, std::less<>>             map_si32_real_d;
    std::map<s_int_32, bool, std::less<>>                   map_si32_b;
    std::map<s_int_32, GpUUID, std::less<>>             map_si32_uuid;
    std::map<s_int_32, std::u8string, std::less<>>          map_si32_str;
    std::map<s_int_32, GpBytesArray, std::less<>>           map_si32_blob;
    std::map<s_int_32, GpReflectDemoDesc::SP, std::less<>>  map_si32_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUi64Desc: public GpReflectDemoMapSi32Desc
{
public:
    CLASS_DD(GpReflectDemoMapUi64Desc)
    REFLECT_DECLARE(u8"d63a6bde-e930-46f6-9dcd-fa7611eb95cf"_uuid)

public:
                                                GpReflectDemoMapUi64Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapUi64Desc    (const GpReflectDemoMapUi64Desc& aDesc);
    explicit                                    GpReflectDemoMapUi64Desc    (GpReflectDemoMapUi64Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUi64Desc   (void) noexcept override;

public:
    std::map<u_int_64, u_int_8, std::less<>>                map_ui64_ui8;
    std::map<u_int_64, s_int_8, std::less<>>                map_ui64_si8;
    std::map<u_int_64, u_int_16, std::less<>>               map_ui64_ui16;
    std::map<u_int_64, s_int_16, std::less<>>               map_ui64_si16;
    std::map<u_int_64, u_int_32, std::less<>>               map_ui64_ui32;
    std::map<u_int_64, s_int_32, std::less<>>               map_ui64_si32;
    std::map<u_int_64, u_int_64, std::less<>>               map_ui64_ui64;
    std::map<u_int_64, s_int_64, std::less<>>               map_ui64_si64;
    std::map<u_int_64, unix_ts_s_t, std::less<>>            map_ui64_uts_s;
    std::map<u_int_64, unix_ts_ms_t, std::less<>>           map_ui64_uts_ms;
    std::map<u_int_64, float, std::less<>>                  map_ui64_real_f;
    std::map<u_int_64, double, std::less<>>             map_ui64_real_d;
    std::map<u_int_64, bool, std::less<>>                   map_ui64_b;
    std::map<u_int_64, GpUUID, std::less<>>             map_ui64_uuid;
    std::map<u_int_64, std::u8string, std::less<>>          map_ui64_str;
    std::map<u_int_64, GpBytesArray, std::less<>>           map_ui64_blob;
    std::map<u_int_64, GpReflectDemoDesc::SP, std::less<>>  map_ui64_s2;
};

class GP_REFLECTION_API GpReflectDemoMapSi64Desc: public GpReflectDemoMapUi64Desc
{
public:
    CLASS_DD(GpReflectDemoMapSi64Desc)
    REFLECT_DECLARE(u8"0bfe8089-51db-41ec-8a89-3c5aaf7d6e5e"_uuid)

public:
                                                GpReflectDemoMapSi64Desc    (void) noexcept;
    explicit                                    GpReflectDemoMapSi64Desc    (const GpReflectDemoMapSi64Desc& aDesc);
    explicit                                    GpReflectDemoMapSi64Desc    (GpReflectDemoMapSi64Desc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapSi64Desc   (void) noexcept override;

public:
    std::map<s_int_64, u_int_8, std::less<>>                map_si64_ui8;
    std::map<s_int_64, s_int_8, std::less<>>                map_si64_si8;
    std::map<s_int_64, u_int_16, std::less<>>               map_si64_ui16;
    std::map<s_int_64, s_int_16, std::less<>>               map_si64_si16;
    std::map<s_int_64, u_int_32, std::less<>>               map_si64_ui32;
    std::map<s_int_64, s_int_32, std::less<>>               map_si64_si32;
    std::map<s_int_64, u_int_64, std::less<>>               map_si64_ui64;
    std::map<s_int_64, s_int_64, std::less<>>               map_si64_si64;
    std::map<s_int_64, unix_ts_s_t, std::less<>>            map_si64_uts_s;
    std::map<s_int_64, unix_ts_ms_t, std::less<>>           map_si64_uts_ms;
    std::map<s_int_64, float, std::less<>>                  map_si64_real_f;
    std::map<s_int_64, double, std::less<>>             map_si64_real_d;
    std::map<s_int_64, bool, std::less<>>                   map_si64_b;
    std::map<s_int_64, GpUUID, std::less<>>             map_si64_uuid;
    std::map<s_int_64, std::u8string, std::less<>>          map_si64_str;
    std::map<s_int_64, GpBytesArray, std::less<>>           map_si64_blob;
    std::map<s_int_64, GpReflectDemoDesc::SP, std::less<>>  map_si64_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUtsSDesc: public GpReflectDemoMapSi64Desc
{
public:
    CLASS_DD(GpReflectDemoMapUtsSDesc)
    REFLECT_DECLARE(u8"b01305f3-84e6-4cf4-b23e-fddc73c5a2bb"_uuid)

public:
                                                GpReflectDemoMapUtsSDesc    (void) noexcept;
    explicit                                    GpReflectDemoMapUtsSDesc    (const GpReflectDemoMapUtsSDesc& aDesc);
    explicit                                    GpReflectDemoMapUtsSDesc    (GpReflectDemoMapUtsSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUtsSDesc   (void) noexcept override;

public:
    std::map<unix_ts_s_t, u_int_8, std::less<>>             map_utss_ui8;
    std::map<unix_ts_s_t, s_int_8, std::less<>>             map_utss_si8;
    std::map<unix_ts_s_t, u_int_16, std::less<>>                map_utss_ui16;
    std::map<unix_ts_s_t, s_int_16, std::less<>>                map_utss_si16;
    std::map<unix_ts_s_t, u_int_32, std::less<>>                map_utss_ui32;
    std::map<unix_ts_s_t, s_int_32, std::less<>>                map_utss_si32;
    std::map<unix_ts_s_t, u_int_64, std::less<>>                map_utss_ui64;
    std::map<unix_ts_s_t, s_int_64, std::less<>>                map_utss_si64;
    std::map<unix_ts_s_t, unix_ts_s_t, std::less<>>         map_utss_uts_s;
    std::map<unix_ts_s_t, unix_ts_ms_t, std::less<>>            map_utss_uts_ms;
    std::map<unix_ts_s_t, float, std::less<>>                   map_utss_real_f;
    std::map<unix_ts_s_t, double, std::less<>>                  map_utss_real_d;
    std::map<unix_ts_s_t, bool, std::less<>>                    map_utss_b;
    std::map<unix_ts_s_t, GpUUID, std::less<>>                  map_utss_uuid;
    std::map<unix_ts_s_t, std::u8string, std::less<>>           map_utss_str;
    std::map<unix_ts_s_t, GpBytesArray, std::less<>>            map_utss_blob;
    std::map<unix_ts_s_t, GpReflectDemoDesc::SP, std::less<>>   map_utss_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUtmsSDesc: public GpReflectDemoMapUtsSDesc
{
public:
    CLASS_DD(GpReflectDemoMapUtmsSDesc)
    REFLECT_DECLARE(u8"4079fc31-aa72-4551-aad9-cfc892ee8c91"_uuid)

public:
                                                GpReflectDemoMapUtmsSDesc   (void) noexcept;
    explicit                                    GpReflectDemoMapUtmsSDesc   (const GpReflectDemoMapUtmsSDesc& aDesc);
    explicit                                    GpReflectDemoMapUtmsSDesc   (GpReflectDemoMapUtmsSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUtmsSDesc  (void) noexcept override;

public:
    std::map<unix_ts_ms_t, u_int_8, std::less<>>                map_utsms_ui8;
    std::map<unix_ts_ms_t, s_int_8, std::less<>>                map_utsms_si8;
    std::map<unix_ts_ms_t, u_int_16, std::less<>>               map_utsms_ui16;
    std::map<unix_ts_ms_t, s_int_16, std::less<>>               map_utsms_si16;
    std::map<unix_ts_ms_t, u_int_32, std::less<>>               map_utsms_ui32;
    std::map<unix_ts_ms_t, s_int_32, std::less<>>               map_utsms_si32;
    std::map<unix_ts_ms_t, u_int_64, std::less<>>               map_utsms_ui64;
    std::map<unix_ts_ms_t, s_int_64, std::less<>>               map_utsms_si64;
    std::map<unix_ts_ms_t, unix_ts_s_t, std::less<>>            map_utsms_uts_s;
    std::map<unix_ts_ms_t, unix_ts_ms_t, std::less<>>           map_utsms_uts_ms;
    std::map<unix_ts_ms_t, float, std::less<>>                  map_utsms_real_f;
    std::map<unix_ts_ms_t, double, std::less<>>             map_utsms_real_d;
    std::map<unix_ts_ms_t, bool, std::less<>>                   map_utsms_b;
    std::map<unix_ts_ms_t, GpUUID, std::less<>>             map_utsms_uuid;
    std::map<unix_ts_ms_t, std::u8string, std::less<>>          map_utsms_str;
    std::map<unix_ts_ms_t, GpBytesArray, std::less<>>           map_utsms_blob;
    std::map<unix_ts_ms_t, GpReflectDemoDesc::SP, std::less<>>  map_utsms_s2;
};

class GP_REFLECTION_API GpReflectDemoMapFloatSDesc: public GpReflectDemoMapUtmsSDesc
{
public:
    CLASS_DD(GpReflectDemoMapFloatSDesc)
    REFLECT_DECLARE(u8"24d7e9d7-24f8-4a58-8227-ebc2fe3ada65"_uuid)

public:
                                                GpReflectDemoMapFloatSDesc  (void) noexcept;
    explicit                                    GpReflectDemoMapFloatSDesc  (const GpReflectDemoMapFloatSDesc& aDesc);
    explicit                                    GpReflectDemoMapFloatSDesc  (GpReflectDemoMapFloatSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapFloatSDesc (void) noexcept override;

public:
    std::map<float, u_int_8, std::less<>>                   map_float_ui8;
    std::map<float, s_int_8, std::less<>>                   map_float_si8;
    std::map<float, u_int_16, std::less<>>                  map_float_ui16;
    std::map<float, s_int_16, std::less<>>                  map_float_si16;
    std::map<float, u_int_32, std::less<>>                  map_float_ui32;
    std::map<float, s_int_32, std::less<>>                  map_float_si32;
    std::map<float, u_int_64, std::less<>>                  map_float_ui64;
    std::map<float, s_int_64, std::less<>>                  map_float_si64;
    std::map<float, unix_ts_s_t, std::less<>>               map_float_uts_s;
    std::map<float, unix_ts_ms_t, std::less<>>              map_float_uts_ms;
    std::map<float, float, std::less<>>                 map_float_real_f;
    std::map<float, double, std::less<>>                    map_float_real_d;
    std::map<float, bool, std::less<>>                      map_float_b;
    std::map<float, GpUUID, std::less<>>                    map_float_uuid;
    std::map<float, std::u8string, std::less<>>             map_float_str;
    std::map<float, GpBytesArray, std::less<>>              map_float_blob;
    std::map<float, GpReflectDemoDesc::SP, std::less<>> map_float_s2;
};

class GP_REFLECTION_API GpReflectDemoMapDoubleSDesc: public GpReflectDemoMapFloatSDesc
{
public:
    CLASS_DD(GpReflectDemoMapDoubleSDesc)
    REFLECT_DECLARE(u8"67bfbbfc-e02c-43d7-8e3f-9110604c6418"_uuid)

public:
                                                GpReflectDemoMapDoubleSDesc (void) noexcept;
    explicit                                    GpReflectDemoMapDoubleSDesc (const GpReflectDemoMapDoubleSDesc& aDesc);
    explicit                                    GpReflectDemoMapDoubleSDesc (GpReflectDemoMapDoubleSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapDoubleSDesc(void) noexcept override;

public:
    std::map<double, u_int_8, std::less<>>                  map_double_ui8;
    std::map<double, s_int_8, std::less<>>                  map_double_si8;
    std::map<double, u_int_16, std::less<>>             map_double_ui16;
    std::map<double, s_int_16, std::less<>>             map_double_si16;
    std::map<double, u_int_32, std::less<>>             map_double_ui32;
    std::map<double, s_int_32, std::less<>>             map_double_si32;
    std::map<double, u_int_64, std::less<>>             map_double_ui64;
    std::map<double, s_int_64, std::less<>>             map_double_si64;
    std::map<double, unix_ts_s_t, std::less<>>              map_double_uts_s;
    std::map<double, unix_ts_ms_t, std::less<>>         map_double_uts_ms;
    std::map<double, float, std::less<>>                    map_double_real_f;
    std::map<double, double, std::less<>>                   map_double_real_d;
    std::map<double, bool, std::less<>>                 map_double_b;
    std::map<double, GpUUID, std::less<>>                   map_double_uuid;
    std::map<double, std::u8string, std::less<>>                map_double_str;
    std::map<double, GpBytesArray, std::less<>>         map_double_blob;
    std::map<double, GpReflectDemoDesc::SP, std::less<>>    map_double_s2;
};

class GP_REFLECTION_API GpReflectDemoMapUuidSDesc: public GpReflectDemoMapDoubleSDesc
{
public:
    CLASS_DD(GpReflectDemoMapUuidSDesc)
    REFLECT_DECLARE(u8"d0d7bbb0-b28a-48cc-99a5-71b7d1d015ea"_uuid)

public:
                                                GpReflectDemoMapUuidSDesc   (void) noexcept;
    explicit                                    GpReflectDemoMapUuidSDesc   (const GpReflectDemoMapUuidSDesc& aDesc);
    explicit                                    GpReflectDemoMapUuidSDesc   (GpReflectDemoMapUuidSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapUuidSDesc  (void) noexcept override;

public:
    std::map<GpUUID, u_int_8, std::less<>>                  map_uuid_ui8;
    std::map<GpUUID, s_int_8, std::less<>>                  map_uuid_si8;
    std::map<GpUUID, u_int_16, std::less<>>             map_uuid_ui16;
    std::map<GpUUID, s_int_16, std::less<>>             map_uuid_si16;
    std::map<GpUUID, u_int_32, std::less<>>             map_uuid_ui32;
    std::map<GpUUID, s_int_32, std::less<>>             map_uuid_si32;
    std::map<GpUUID, u_int_64, std::less<>>             map_uuid_ui64;
    std::map<GpUUID, s_int_64, std::less<>>             map_uuid_si64;
    std::map<GpUUID, unix_ts_s_t, std::less<>>              map_uuid_uts_s;
    std::map<GpUUID, unix_ts_ms_t, std::less<>>         map_uuid_uts_ms;
    std::map<GpUUID, float, std::less<>>                    map_uuid_real_f;
    std::map<GpUUID, double, std::less<>>                   map_uuid_real_d;
    std::map<GpUUID, bool, std::less<>>                 map_uuid_b;
    std::map<GpUUID, GpUUID, std::less<>>                   map_uuid_uuid;
    std::map<GpUUID, std::u8string, std::less<>>                map_uuid_str;
    std::map<GpUUID, GpBytesArray, std::less<>>         map_uuid_blob;
    std::map<GpUUID, GpReflectDemoDesc::SP, std::less<>>    map_uuid_s2;
};

class GP_REFLECTION_API GpReflectDemoMapStrSDesc: public GpReflectDemoMapUuidSDesc
{
public:
    CLASS_DD(GpReflectDemoMapStrSDesc)
    REFLECT_DECLARE(u8"92dbbbad-0e3b-4f09-8922-4335b178a6e6"_uuid)

public:
                                                GpReflectDemoMapStrSDesc    (void) noexcept;
    explicit                                    GpReflectDemoMapStrSDesc    (const GpReflectDemoMapStrSDesc& aDesc);
    explicit                                    GpReflectDemoMapStrSDesc    (GpReflectDemoMapStrSDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapStrSDesc   (void) noexcept override;

public:
    std::map<std::u8string, u_int_8, std::less<>>               map_str_ui8;
    std::map<std::u8string, s_int_8, std::less<>>               map_str_si8;
    std::map<std::u8string, u_int_16, std::less<>>              map_str_ui16;
    std::map<std::u8string, s_int_16, std::less<>>              map_str_si16;
    std::map<std::u8string, u_int_32, std::less<>>              map_str_ui32;
    std::map<std::u8string, s_int_32, std::less<>>              map_str_si32;
    std::map<std::u8string, u_int_64, std::less<>>              map_str_ui64;
    std::map<std::u8string, s_int_64, std::less<>>              map_str_si64;
    std::map<std::u8string, unix_ts_s_t, std::less<>>           map_str_uts_s;
    std::map<std::u8string, unix_ts_ms_t, std::less<>>          map_str_uts_ms;
    std::map<std::u8string, float, std::less<>>                 map_str_real_f;
    std::map<std::u8string, double, std::less<>>                    map_str_real_d;
    std::map<std::u8string, bool, std::less<>>                  map_str_b;
    std::map<std::u8string, GpUUID, std::less<>>                    map_str_uuid;
    std::map<std::u8string, std::u8string, std::less<>>         map_str_str;
    std::map<std::u8string, GpBytesArray, std::less<>>          map_str_blob;
    std::map<std::u8string, GpReflectDemoDesc::SP, std::less<>> map_str_s2;
};

class GP_REFLECTION_API GpReflectDemoMapBytesDesc: public GpReflectDemoMapStrSDesc
{
public:
    CLASS_DD(GpReflectDemoMapBytesDesc)
    REFLECT_DECLARE(u8"e3c574d3-1ffe-4e60-a809-b0903f2a7beb"_uuid)

public:
                                                GpReflectDemoMapBytesDesc   (void) noexcept;
    explicit                                    GpReflectDemoMapBytesDesc   (const GpReflectDemoMapBytesDesc& aDesc);
    explicit                                    GpReflectDemoMapBytesDesc   (GpReflectDemoMapBytesDesc&& aDesc) noexcept;
    virtual                                     ~GpReflectDemoMapBytesDesc  (void) noexcept override;

public:
    std::map<GpBytesArray, u_int_8, std::less<>>                map_bytes_ui8;
    std::map<GpBytesArray, s_int_8, std::less<>>                map_bytes_si8;
    std::map<GpBytesArray, u_int_16, std::less<>>               map_bytes_ui16;
    std::map<GpBytesArray, s_int_16, std::less<>>               map_bytes_si16;
    std::map<GpBytesArray, u_int_32, std::less<>>               map_bytes_ui32;
    std::map<GpBytesArray, s_int_32, std::less<>>               map_bytes_si32;
    std::map<GpBytesArray, u_int_64, std::less<>>               map_bytes_ui64;
    std::map<GpBytesArray, s_int_64, std::less<>>               map_bytes_si64;
    std::map<GpBytesArray, unix_ts_s_t, std::less<>>            map_bytes_uts_s;
    std::map<GpBytesArray, unix_ts_ms_t, std::less<>>           map_bytes_uts_ms;
    std::map<GpBytesArray, float, std::less<>>                  map_bytes_real_f;
    std::map<GpBytesArray, double, std::less<>>             map_bytes_real_d;
    std::map<GpBytesArray, bool, std::less<>>                   map_bytes_b;
    std::map<GpBytesArray, GpUUID, std::less<>>             map_bytes_uuid;
    std::map<GpBytesArray, std::u8string, std::less<>>          map_bytes_str;
    std::map<GpBytesArray, GpBytesArray, std::less<>>           map_bytes_blob;
    std::map<GpBytesArray, GpReflectDemoDesc::SP, std::less<>>  map_bytes_s2;
};

}//namespace GPlatform

#endif//GP_USE_REFLECTION
