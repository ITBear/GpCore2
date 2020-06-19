#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include "GpReferenceCounter.hpp"

namespace GPlatform {

template <typename	T,
          typename	DeleterT>
class GpReferenceStorage final: public GpReferenceCounter
{
public:
    using this_type		= GpReferenceStorage<T, DeleterT>;
    using value_type	= T;

public:
    virtual					~GpReferenceStorage	(void) noexcept override final = default;

    template<typename... Ts>
                            GpReferenceStorage	(Ts&&... aArgs):
                            GpReferenceCounter(1_cnt, &iValue),
                            iValue(std::forward<Ts>(aArgs)...)
                            {
                            }

    [[nodiscard]] T&		Value				(void) noexcept;
    [[nodiscard]] const T&	Value				(void) const noexcept;
    [[nodiscard]] T*		Ptr					(void) noexcept;
    [[nodiscard]] const T*	Ptr					(void) const noexcept;

protected:
    virtual void			Clear				(void) noexcept override;

private:
    T						iValue;
};

template <typename	T,
          typename	DeleterT>
T&	GpReferenceStorage<T, DeleterT>::Value (void) noexcept
{
    return iValue;
}

template <typename	T,
          typename	DeleterT>
const T&	GpReferenceStorage<T, DeleterT>::Value (void) const noexcept
{
    return iValue;
}

template <typename	T,
          typename	DeleterT>
T*	GpReferenceStorage<T, DeleterT>::Ptr (void) noexcept
{
    return &iValue;
}

template <typename	T,
          typename	DeleterT>
const T*	GpReferenceStorage<T, DeleterT>::Ptr (void) const noexcept
{
    return &iValue;
}

template <typename	T,
          typename	DeleterT>
void	GpReferenceStorage<T, DeleterT>::Clear (void) noexcept
{
    DeleterT::SDelete(this);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_SHARED_POINTERS)
