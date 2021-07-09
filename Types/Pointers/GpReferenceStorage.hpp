#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include "GpReferenceCounter.hpp"
#include "../../Memory/GpMemOps.hpp"

namespace GPlatform {

template <typename  T,
          typename  DeleterT>
class GpReferenceStorage final: public GpReferenceCounter
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReferenceStorage)

public:
    using this_type     = GpReferenceStorage<T, DeleterT>;
    using value_type    = T;

public:
    template<typename... Ts>
                            GpReferenceStorage  (Ts&&... aArgs):
                            GpReferenceCounter(iValueStorage.data())
                            {
                                GpMemOps::SEmplace<T>(iValueStorage.data(), std::forward<Ts>(aArgs)...);
                            }

    virtual                 ~GpReferenceStorage (void) noexcept override final
                            {
                                _DeleteValue();
                            }

    /*[[nodiscard]] T&      Value               (void) noexcept;
    [[nodiscard]] const T&  Value               (void) const noexcept;
    [[nodiscard]] T*        Ptr                 (void) noexcept;
    [[nodiscard]] const T*  Ptr                 (void) const noexcept;*/

protected:
    virtual void            DeleteValue         (void) noexcept override final;
    virtual void            DeleteSelf          (void) noexcept override final;

private:
    void                    _DeleteValue        (void) noexcept;

private:
    alignas(T) std::array<u_int_8, sizeof(T)>   iValueStorage;
    //std::atomic_flag                          iIsDeleted = false;
};

/*template <typename    T,
          typename  DeleterT>
T&  GpReferenceStorage<T, DeleterT>::Value (void) noexcept
{
    return *reinterpret_cast<T*>(iValueStorage.data());
}

template <typename  T,
          typename  DeleterT>
const T&    GpReferenceStorage<T, DeleterT>::Value (void) const noexcept
{
    return *reinterpret_cast<const T*>(iValueStorage.data());
}

template <typename  T,
          typename  DeleterT>
T*  GpReferenceStorage<T, DeleterT>::Ptr (void) noexcept
{
    return reinterpret_cast<T*>(iValueStorage.data());
}

template <typename  T,
          typename  DeleterT>
const T*    GpReferenceStorage<T, DeleterT>::Ptr (void) const noexcept
{
    return reinterpret_cast<const T*>(iValueStorage.data());
}*/

template <typename  T,
          typename  DeleterT>
void    GpReferenceStorage<T, DeleterT>::DeleteValue (void) noexcept
{
    _DeleteValue();
}

template <typename  T,
          typename  DeleterT>
void    GpReferenceStorage<T, DeleterT>::DeleteSelf (void) noexcept
{
    DeleterT::SDelete(this);
}

template <typename  T,
          typename  DeleterT>
void    GpReferenceStorage<T, DeleterT>::_DeleteValue (void) noexcept
{
    //if (iIsDeleted.test_and_set(std::memory_order_acquire) == false)

    auto ptr = ValuePtr<T>();

    if (ptr)
    {
        GpMemOps::EmplaceDeleter::SDelete<T>(ptr);
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_SHARED_POINTERS)
