#include "GpReflectUtils_TotalMemoryUse.hpp"

#include "GpReflectVisitor.hpp"

namespace GPlatform {

namespace TotalMemoryUse {

// ------------------------------------------ GpReflectUtils_VisitCtx -------------------------------------------------
class GpReflectUtils_VisitCtx
{
public:
    using CacheOptRefT = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

public:
                        GpReflectUtils_VisitCtx     (CacheOptRefT&  aCache,
                                                     const void*    aDataPtr) noexcept: iCache(aCache), iDataPtr(aDataPtr) {}

    [[nodiscard]] bool  OnVisitBegin                ([[maybe_unused]] const GpReflectModel& aModel) {return true;}
    void                OnVisitEnd                  ([[maybe_unused]] const GpReflectModel& aModel) {/*NOP*/}

    size_t              PropsDynamicAllocatedSize   (void) const noexcept;
    void                SumSize                     (size_t aSize);

public:
    size_t              iPropsDynamicAllocatedSize  = 0;
    CacheOptRefT&       iCache;
    const void*         iDataPtr                    = nullptr;
};

size_t  GpReflectUtils_VisitCtx::PropsDynamicAllocatedSize (void) const noexcept
{
    return iPropsDynamicAllocatedSize;
}

void    GpReflectUtils_VisitCtx::SumSize (size_t aSize)
{
    iPropsDynamicAllocatedSize = NumOps::SAdd(iPropsDynamicAllocatedSize, aSize);
}

// ------------------------------------- GpReflectUtils_VisitValueCtx ------------------------------------------
class GpReflectUtils_VisitValueCtx
{
public:
                GpReflectUtils_VisitValueCtx    (void) noexcept {}

    bool        OnVisitBegin    (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        OnVisitEnd      (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI8             (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI8             (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI16            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI16            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI32            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI32            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI64            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI64            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Double          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Float           (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Bool            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UUID            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        String          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        BLOB            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Object          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        ObjectSP        (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Enum            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        EnumFlags       (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
};

bool    GpReflectUtils_VisitValueCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectUtils_VisitValueCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::UI8
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::SI8
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::UI16
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::SI16
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::UI32
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::SI32
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::UI64
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::SI64
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::Double
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::Float
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::Bool
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::UUID
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::String
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& val = aProp.Value_String(aCtx.iDataPtr);
    aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(val, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::BLOB
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& val = aProp.Value_BLOB(aCtx.iDataPtr);
    aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(val, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::Object
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    const GpReflectObject& val = aProp.Value_Object(aCtx.iDataPtr);
    aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(val, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::ObjectSP
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const GpReflectObject::SP& valSP = aProp.Value_ObjectSP(aCtx.iDataPtr);
    aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(valSP, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::Enum
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::EnumFlags
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

// ------------------------------------- GpReflectUtils_VisitVecCtx ------------------------------------------
class GpReflectUtils_VisitVecCtx
{
public:
                GpReflectUtils_VisitVecCtx  (void) noexcept {}

    bool        OnVisitBegin(const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);
    void        OnVisitEnd  (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI8         (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI8         (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI16        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI16        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI32        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI32        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI64        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI64        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        Double      (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        Float       (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UUID        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        String      (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        BLOB        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        Object      (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        ObjectSP    (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);
};

bool    GpReflectUtils_VisitVecCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectUtils_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::UI8(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(u_int_8));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::SI8(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(s_int_8));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::UI16(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(u_int_16));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::SI16(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(s_int_16));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::UI32(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(u_int_32));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::SI32(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(s_int_32));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::UI64(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(u_int_64));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::SI64(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(s_int_64));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::Double
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::Double(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(double));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::Float
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::Float(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(float));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UUID
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::UUID(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(GpUUID));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::String
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::String(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(std::string));

    for (const auto& element: container)
    {
        aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(element, aCtx.iCache));
    }
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::BLOB
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::BLOB(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(GpBytesArray));

    for (const auto& element: container)
    {
        aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(element, aCtx.iCache));
    }
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::Object
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::Object(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * container.stride());

    const size_t count = std::size(container);

    for (size_t id = 0; id < count; id++)
    {
        aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(container[id], aCtx.iCache));
    }
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::ObjectSP
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT::ObjectSP(aCtx.iDataPtr, aProp);
    aCtx.SumSize(std::size(container) * sizeof(GpReflectObject::SP));

    const size_t count = std::size(container);

    for (size_t id = 0; id < count; id++)
    {
        aCtx.SumSize(GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(container[id], aCtx.iCache));
    }
}

// ---------------------------------------------- GpReflectUtils_VisitMapCtx ------------------------------------------------------

class GpReflectUtils_VisitMapCtx
{
public:
                GpReflectUtils_VisitMapCtx  (void) noexcept {}

    bool        OnVisitBegin    (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        OnVisitEnd      (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI8           (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI8           (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI16          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI16          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI32          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI32          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI64          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI64          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_Double        (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_Float         (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UUID          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_String        (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_BLOB          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_ObjectSP      (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename CT>
    void        Process         (const CT&                  aContainer,
                                 GpReflectUtils_VisitCtx&   aCtx);
};

bool    GpReflectUtils_VisitMapCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectUtils_VisitMapCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UI8(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SI8(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UI16(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SI16(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UI32(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SI32(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UI64(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SI64(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_Double
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::Double(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_Float
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::Float(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UUID
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UUID(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_String
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::String(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_BLOB
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::BLOB(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_ObjectSP
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& container = ValGetterT<KeyT>::ObjectSP(aCtx.iDataPtr, aProp);
    Process(container, aCtx);
}

template<typename CT>
void    GpReflectUtils_VisitMapCtx::Process
(
    const CT&                   aContainer,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    size_t allocatedSize = 0;

    constexpr const auto types      = GpReflectUtils::SDetectTypes<std::remove_cvref_t<CT>>();
    constexpr const auto valueType  = std::get<0>(types);
    constexpr const auto keyType    = std::get<1>(types);

    // ----------------------------- key -----------------------------
    if constexpr(keyType == GpReflectType::U_INT_8)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_8);
    } else if constexpr(keyType == GpReflectType::S_INT_8)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_8);
    } else if constexpr(keyType == GpReflectType::U_INT_16)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_16);
    } else if constexpr(keyType == GpReflectType::S_INT_16)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_16);
    } else if constexpr(keyType == GpReflectType::U_INT_32)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_32);
    } else if constexpr(keyType == GpReflectType::S_INT_32)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_32);
    } else if constexpr(keyType == GpReflectType::U_INT_64)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_64);
    } else if constexpr(keyType == GpReflectType::S_INT_64)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_64);
    } else if constexpr(keyType == GpReflectType::DOUBLE)
    {
        allocatedSize += std::size(aContainer) * sizeof(double);
    } else if constexpr(keyType == GpReflectType::FLOAT)
    {
        allocatedSize += std::size(aContainer) * sizeof(float);
    } else if constexpr(keyType == GpReflectType::UUID)
    {
        allocatedSize += std::size(aContainer) * sizeof(GpUUID);
    } else if constexpr(keyType == GpReflectType::STRING)
    {
        allocatedSize += std::size(aContainer) * sizeof(std::string);

        for (const auto&[key, val]: aContainer)
        {
            allocatedSize += GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(key, aCtx.iCache);
        }
    } else if constexpr(keyType == GpReflectType::BLOB)
    {
        allocatedSize += std::size(aContainer) * sizeof(GpBytesArray);

        for (const auto&[key, val]: aContainer)
        {
            allocatedSize += GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(key, aCtx.iCache);
        }
    } else
    {
        GpThrowCe<GpException>(fmt::format("Unsupported key type {}", GpReflectType{keyType}));
    }

    // ----------------------------- value -----------------------------
    if constexpr(valueType == GpReflectType::U_INT_8)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_8);
    } else if constexpr(valueType == GpReflectType::S_INT_8)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_8);
    } else if constexpr(valueType == GpReflectType::U_INT_16)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_16);
    } else if constexpr(valueType == GpReflectType::S_INT_16)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_16);
    } else if constexpr(valueType == GpReflectType::U_INT_32)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_32);
    } else if constexpr(valueType == GpReflectType::S_INT_32)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_32);
    } else if constexpr(valueType == GpReflectType::U_INT_64)
    {
        allocatedSize += std::size(aContainer) * sizeof(u_int_64);
    } else if constexpr(valueType == GpReflectType::S_INT_64)
    {
        allocatedSize += std::size(aContainer) * sizeof(s_int_64);
    } else if constexpr(valueType == GpReflectType::DOUBLE)
    {
        allocatedSize += std::size(aContainer) * sizeof(double);
    } else if constexpr(valueType == GpReflectType::FLOAT)
    {
        allocatedSize += std::size(aContainer) * sizeof(float);
    } else if constexpr(valueType == GpReflectType::UUID)
    {
        allocatedSize += std::size(aContainer) * sizeof(GpUUID);
    } else if constexpr(valueType == GpReflectType::STRING)
    {
        allocatedSize += std::size(aContainer) * sizeof(std::string);

        for (const auto&[key, val]: aContainer)
        {
            allocatedSize += GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(val, aCtx.iCache);
        }
    } else if constexpr(valueType == GpReflectType::BLOB)
    {
        allocatedSize += std::size(aContainer) * sizeof(GpBytesArray);

        for (const auto&[key, val]: aContainer)
        {
            allocatedSize += GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(val, aCtx.iCache);
        }
    } else if constexpr(valueType == GpReflectType::OBJECT_SP)
    {
        allocatedSize += std::size(aContainer) * sizeof(GpReflectObject::SP);

        for (const auto&[key, val]: aContainer)
        {
            allocatedSize += GpReflectUtils_TotalMemoryUse::SGetAllocatedSize(val, aCtx.iCache);
        }
    } else
    {
        GpThrowCe<GpException>(fmt::format("Unsupported value type {}", GpReflectType{valueType}));
    }

    aCtx.SumSize(allocatedSize);
}

// ------------------------------------------ GpReflectUtils_Visitor -------------------------------------------------

class GpReflectUtils_Visitor
{
public:
    using VisitCtx      = GpReflectUtils_VisitCtx;
    using VisitValueCtx = GpReflectUtils_VisitValueCtx;
    using VisitVecCtx   = GpReflectUtils_VisitVecCtx;
    using VisitMapCtx   = GpReflectUtils_VisitMapCtx;
};

}// namespace TotalMemoryUse

// ------------------------------------------ GpReflectUtils_TotalMemoryUse -------------------------------------------------

size_t  GpReflectUtils_TotalMemoryUse::SDo
(
    const GpReflectObject&  aObject,
    CacheRefOptT            aCache,
    Mode                    aMode
)
{
    // Try to find in cache
    if (aCache.has_value())
    {
        auto& cache = aCache.value().get();
        auto iter   = cache.find(&aObject);

        if (iter != std::end(cache))// found in cache
        {
            // return result from cache
            return iter->second;
        }
    }

    GpReflectVisitor<TotalMemoryUse::GpReflectUtils_Visitor>    visitor;
    TotalMemoryUse::GpReflectUtils_VisitCtx                     ctx(aCache, aObject.ReflectDataPtr());
    GpReflectModel::CSP                                         modelCSP    = aObject.ReflectModel();
    const GpReflectModel&                                       model       = modelCSP.Vn();

    visitor.Visit(model, ctx);

    size_t reflectObjectSize = 0;

    if (aMode == Mode::COUNT_SELF_SIZE)
    {
        reflectObjectSize += model.Size();
    }

    reflectObjectSize += ctx.PropsDynamicAllocatedSize();

    // Update cache
    if (aCache.has_value())
    {
        auto& cache = aCache.value().get();
        cache.emplace(&aObject, reflectObjectSize);
    }

    return reflectObjectSize;
}

}// namespace GPlatform
