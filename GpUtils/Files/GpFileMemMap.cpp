#include <GpCore2/GpUtils/Files/GpFileMemMap.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>

#if defined(GP_USE_FILE_MEMORY_MAP)

#if defined(GP_OS_LINUX) || defined(GP_OS_MACOS)
#   include <sys/mman.h>

#   if defined(GP_OS_LINUX)
#       define memory_map mmap64
#   elif defined(GP_OS_MACOS)
#       define memory_map mmap
#   endif

#endif// #if defined(GP_OS_LINUX) || defined(GP_OS_MACOS)

namespace GPlatform {

GpFileMemMap::GpFileMemMap (void) noexcept
{
}

GpFileMemMap::~GpFileMemMap (void) noexcept
{
    Close();
}

GpSpanByteRW    GpFileMemMap::OpenAndMap
(
    std::string_view    aFileName,
    const GpFileFlags   aFileFlags,
    const size_byte_t   aOffsetToMap,
    const size_byte_t   aSizeToMap
)
{
    Close();

    iFile.Open(aFileName, aFileFlags);
    const size_byte_t   fileSize    = iFile.Size();
    size_byte_t         sizeToMap   = aSizeToMap;

    if (aSizeToMap > 0_byte)
    {
        const size_byte_t expectedFileSize  = aOffsetToMap + aSizeToMap;

        if (fileSize < expectedFileSize)
        {
            iFile.Resize(expectedFileSize);
        }
    } else
    {
        sizeToMap = fileSize;
    }

    if (sizeToMap > 0_byte)
    {
        MemMap(aOffsetToMap, sizeToMap);
    } else
    {
        iMappedData.Clear();
    }

    return iMappedData;
}

GpSpanByteRW    GpFileMemMap::ReopenAndMap
(
    size_byte_t aOffsetToMap,
    size_byte_t aSizeToMap
)
{
    VERIFY
    (
        iFile.IsOpen(),
        "The file must already be open"
    );

    MemUnmap();
    iFile.Resize(aOffsetToMap + aSizeToMap);
    MemMap(aOffsetToMap, aSizeToMap);

    return iMappedData;
}

void    GpFileMemMap::Close (void) noexcept
{
    MemUnmap();

    iFile.Close();
}

void    GpFileMemMap::MemMap
(
    const size_byte_t aOffsetToMap,
    const size_byte_t aSizeToMap
)
{
#if defined(GP_OS_WINDOWS)
    iMappedHandle = CreateFileMappingA
    (
        iFile.Handler(),
        nullptr,
        iFile.Flags().Test(GpFileFlag::WRITE) ? PAGE_READWRITE : PAGE_READONLY,
        0,
        0,
        nullptr
    );

    const std::string_view fileName = iFile.Name();

    VERIFY
    (
        iMappedHandle != nullptr,
        [fileName]()
        {
            return fmt::format
            (
                "Failed to create memory mapping for file '{}'. {}",
                fileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    iMappedData = MapViewOfFileEx
    (
        iMappedHandle,
        iFile.Flags().Test(GpFileFlag::WRITE) ? FILE_MAP_WRITE : FILE_MAP_READ,
        NumOps::SConvert<DWORD>(aOffsetToMap.Value() >> 32),
        NumOps::SConvert<DWORD>(aOffsetToMap.Value() & size_byte_t::value_type{0xFFFFFFFF}),
        NumOps::SConvert<SIZE_T>(aSizeToMap.Value()),
        nullptr
    );

    VERIFY
    (
        iMappedData != nullptr,
        [fileName]()
        {
            return fmt::format
            (
                "Failed to map view for file '{}'. {}",
                fileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );
#elif defined(GP_OS_LINUX) || defined(GP_OS_MACOS)
    int flags = 0;

    if (iFile.Flags().Test(GpFileFlag::WRITE))
    {
        flags |= PROT_WRITE;
    }

    if (iFile.Flags().Test(GpFileFlag::READ))
    {
        flags |= PROT_READ;
    }

    void* mappedData = memory_map
    (
        nullptr,
        NumOps::SConvert<size_t>(aSizeToMap.Value()),
        flags,
        MAP_SHARED,
        iFile.Handler(),
        NumOps::SConvert<s_int_64>(aOffsetToMap.Value())
    );

    const std::string_view fileName = iFile.Name();

    VERIFY
    (
        mappedData != MAP_FAILED,
        [fileName]()
        {
            return fmt::format
            (
                "Failed to map view for file '{}'. {}",
                fileName,
                GpErrno::SGetAndClear()
            );
        }
    );
#else
#   error Unsupported OS
#endif

    iMappedData = GpSpanByteRW
    {
        static_cast<std::byte*>(mappedData),
        aSizeToMap.Value()
    };
}

void    GpFileMemMap::MemUnmap (void) noexcept
{
#if defined(GP_OS_WINDOWS)
    if (iMappedData != nullptr)
    {
        if (UnmapViewOfFile(iMappedData) == 0)
        {
            GpOutUtils::S().Err
            (
                fmt::format
                (
                    "Failed to unmap view for file '{}'. {}",
                    iFile.Name(),
                    GpErrno::SWinGetAndClear()
                )
            );
        }

        iMappedData = nullptr;
    }

    if (iMappedHandle != GpFile::HandlerT{})
    {
        if (CloseHandle(iMappedHandle) == 0)
        {
            GpOutUtils::S().Err
            (
                fmt::format
                (
                    "Failed to close map handle for file '{}'. {}",
                    iFile.Name(),
                    GpErrno::SWinGetAndClear()
                )
            );
        }

        iMappedHandle = nullptr;
    }
#elif defined(GP_OS_LINUX) || defined(GP_OS_MACOS)
    if (iMappedData.NotEmpty())
    {
        const auto res = munmap
        (
            iMappedData.Ptr(),
            iMappedData.SizeInBytes()
        );

        if (res == -1)
        {
            GpOutUtils::S().Err
            (
                fmt::format
                (
                    "Failed to unmap view for file '{}'. {}",
                    iFile.Name(),
                    GpErrno::SGetAndClear()
                )
            );
        }

        iMappedData.Clear();
    }
#else
#   error Unsupported OS
#endif
}

}// namespace GPlatform

#endif// #if defined(GP_USE_FILE_MEMORY_MAP)
