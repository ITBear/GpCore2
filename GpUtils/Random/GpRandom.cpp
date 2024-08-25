#include <GpCore2/GpUtils/Random/GpRandom.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageFixedSize.hpp>

#if defined(GP_USE_RANDOM_GENERATORS)

#include <functional>

#   if defined(GP_OS_WINDOWS)
#       include "GpRandomDeviceWin.hpp"
#   endif//if defined(GP_OS_WINDOWS)

namespace GPlatform {

template<typename T>
T   GenRndVal
(
    const T         aMin,
    const T         aMax,
    random_mt19937& aRndEngine
) noexcept
{
    using UseInDistT = std::conditional_t<std::is_same_v<T, u_int_8> || std::is_same_v<T, s_int_8>, s_int_16, T>;

    std::uniform_int_distribution<UseInDistT> dist{UseInDistT(aMin), UseInDistT(aMax)};
    return static_cast<T>(dist(aRndEngine));
}

GpRandom::GpRandom (void) noexcept
{
}

GpRandom::~GpRandom (void) noexcept
{
}

void    GpRandom::SetSeedFromRD (void)
{
#if defined(GP_OS_WINDOWS)
    GpRandomDeviceWin rd;
#else
    std::random_device rd;
#endif

    std::array<random_mt19937::result_type, random_mt19937::state_size> seed_data;
    std::generate_n(std::data(seed_data), std::size(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    iEngine.seed(seq);
}

void    GpRandom::SetSeed (const random_mt19937::result_type aSeed)
{
    iEngine.seed(aSeed);
}

s_int_8 GpRandom::SI8 (const s_int_8 aMin, const s_int_8 aMax) noexcept
{
    return GenRndVal<s_int_8>(aMin, aMax, iEngine);
}

u_int_8 GpRandom::UI8 (const u_int_8 aMin, const u_int_8 aMax) noexcept
{
    return GenRndVal<u_int_8>(aMin, aMax, iEngine);
}

s_int_16    GpRandom::SI16 (const s_int_16 aMin, const s_int_16 aMax) noexcept
{
    return GenRndVal<s_int_16>(aMin, aMax, iEngine);
}

u_int_16    GpRandom::UI16 (const u_int_16 aMin, const u_int_16 aMax) noexcept
{
    return GenRndVal<u_int_16>(aMin, aMax, iEngine);
}

s_int_32    GpRandom::SI32 (const s_int_32 aMin, const s_int_32 aMax) noexcept
{
    return GenRndVal<s_int_32>(aMin, aMax, iEngine);
}

u_int_32    GpRandom::UI32 (const u_int_32 aMin, const u_int_32 aMax) noexcept
{
    return GenRndVal<u_int_32>(aMin, aMax, iEngine);
}

s_int_64    GpRandom::SI64 (const s_int_64 aMin, const s_int_64 aMax) noexcept
{
    return GenRndVal<s_int_64>(aMin, aMax, iEngine);
}

u_int_64    GpRandom::UI64 (const u_int_64 aMin, const u_int_64 aMax) noexcept
{
    return GenRndVal<u_int_64>(aMin, aMax, iEngine);
}

ssize_t GpRandom::SSizeT (const ssize_t aMin, const ssize_t aMax) noexcept
{
    return GenRndVal<ssize_t>(aMin, aMax, iEngine);
}

size_t  GpRandom::SizeT (const size_t aMin, const size_t aMax) noexcept
{
    return GenRndVal<size_t>(aMin, aMax, iEngine);
}

bool    GpRandom::Bool (void) noexcept
{
    return GenRndVal<u_int_64>(0, 1, iEngine) == 0;
}

double  GpRandom::Double
(
    const double    aMin,
    const double    aMax
) noexcept
{
    const double v = double(GenRndVal<u_int_64>(NumOps::SMin<u_int_64>(), NumOps::SMax<u_int_64>(), iEngine))
                   / double(NumOps::SMax<u_int_64>());

    return NumOps::SLerp(aMin, aMax, v);
}

float   GpRandom::Float
(
    const float aMin,
    const float aMax
) noexcept
{
    return static_cast<float>(Double(double(aMin), double(aMax)));
}

std::string     GpRandom::String
(
    const GpRandomStrMode::EnumT    aMode,
    const size_t                    aSize
)
{
    std::string_view    srcStr      = GpRandomIf::sStrs.at(size_t(aMode));
    const u_int_8       srcStrMaxId = NumOps::SConvert<u_int_8>(std::size(srcStr)) - 1;
    std::string         res;

    res.resize(aSize);
    char* strData = std::data(res);

    for (size_t id = 0; id < aSize; ++id)
    {
        *strData++ = srcStr.at(UI8(0, srcStrMaxId));
    }

    return res;
}

GpBytesArray    GpRandom::BytesArray (const size_t aSize)
{
    GpBytesArray res;
    res.resize(aSize);

    GpByteWriterStorageFixedSize    writerStorage{res};
    GpByteWriter                    writer{writerStorage};

    BytesArray(writer, aSize);

    return res;
}

void    GpRandom::BytesArray (GpSpanByteRW aSpanByteRW)
{
    GpByteWriterStorageFixedSize    writerStorage{aSpanByteRW};
    GpByteWriter                    writer{writerStorage};

    BytesArray(writer, aSpanByteRW.SizeInBytes());
}

void    GpRandom::BytesArray
(
    GpByteWriter&   aDataWriter,
    const size_t    aSize
)
{
    GpSpanByteRW    dataSpan    = aDataWriter.SubspanThenOffsetAdd(aSize);
    std::byte* _R_  dataPtr     = reinterpret_cast<std::byte*>(dataSpan.Ptr());

    // 8 byte parts
    const size_t ui64PartsCount = aSize / sizeof(u_int_64);

    for (size_t id = 0; id < ui64PartsCount; id++)
    {
        const u_int_64 v = UI64();
        std::memcpy(dataPtr, &v, sizeof(v));
        dataPtr += sizeof(v);
    }

    // 1 byte parts
    const size_t ui8PartsCount = aSize - (ui64PartsCount * sizeof(u_int_64));

    for (size_t id = 0; id < ui8PartsCount; id++)
    {
        *dataPtr++ = std::byte{UI8()};
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_RANDOM_GENERATORS)
