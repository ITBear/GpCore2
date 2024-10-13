#include <GpCore2/GpUtils/Files/GpFileMemMap.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>

#if defined(GP_USE_FILE_MEMORY_MAP)

#if defined(GP_OS_LINUX)
#   include <sys/mman.h>
#endif

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
    // Close
    Close();

    // Open file
    iFile.Open(aFileName, aFileFlags);
    size_byte_t fileSize = iFile.Size();

    if (aSizeToMap > 0_byte)
    {
        const size_byte_t actualfileSize    = fileSize;
        const size_byte_t expectedFileSize  = aOffsetToMap + aSizeToMap;

        if (actualfileSize < expectedFileSize)
        {
            iFile.GoToPos(expectedFileSize);
            iFile.TruncateToCurrentPos();
        }

        fileSize    = expectedFileSize;
        iSizeToMap  = aSizeToMap;
    } else
    {
        //fileSize  = fileSize;
        iSizeToMap  = fileSize;
    }

    // Save args
    iOffsetToMap    = aOffsetToMap;

    // Map file
    return GpSpanByteRW
    {
        static_cast<std::byte*>(MemMap()),
        NumOps::SConvert<size_t>(fileSize.Value())
    };
}

void    GpFileMemMap::Close (void) noexcept
{
    MemUnmap();
    iFile.Close();

    iOffsetToMap    = 0_byte;
    iSizeToMap      = 0_byte;
}

void*   GpFileMemMap::MemMap (void)
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

    THROW_COND_GP
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
        NumOps::SConvert<DWORD>(iOffsetToMap.Value() >> 32),
        NumOps::SConvert<DWORD>(iOffsetToMap.Value() & size_byte_t::value_type{0xFFFFFFFF}),
        NumOps::SConvert<SIZE_T>(iSizeToMap.Value()),
        nullptr
    );

    THROW_COND_GP
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
#else
    iMappedData = mmap64
    (
        nullptr,
        NumOps::SConvert<size_t>(iSizeToMap.Value()),
        iFile.Flags().Test(GpFileFlag::WRITE) ? (PROT_READ | PROT_WRITE) : PROT_READ,
        MAP_SHARED,
        iFile.Handler(),
        NumOps::SConvert<__off64_t>(iOffsetToMap.Value())
    );

    const std::string_view fileName = iFile.Name();

    THROW_COND_GP
    (
        iMappedData != MAP_FAILED,
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
#endif

    return iMappedData;
}

void    GpFileMemMap::MemUnmap (void) noexcept
{
#if defined(GP_OS_WINDOWS)
    if (iMappedData != nullptr)
    {
        if (UnmapViewOfFile(iMappedData) == 0)
        {
            GpStringUtils::SCerr
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
            GpStringUtils::SCerr
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
#else
    if (iMappedData != nullptr)
    {
        if (munmap(iMappedData, NumOps::SConvert<size_t>(iSizeToMap.Value())) == -1)
        {
            GpStringUtils::SCerr
            (
                fmt::format
                (
                    "Failed to unmap view for file '{}'. {}",
                    iFile.Name(),
                    GpErrno::SGetAndClear()
                )
            );
        }

        iMappedData = nullptr;
    }
#endif
}

}// namespace GPlatform

#endif// #if defined(GP_USE_FILE_MEMORY_MAP)
