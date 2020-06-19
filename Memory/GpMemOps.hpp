#pragma once

#include "../Config/GpConfig.hpp"
#include "../Types/Units/Other/count_t.hpp"

#if defined(GP_USE_MEMORY_MNG)

#include "../Types/Classes/GpClassesDefines.hpp"

#include <cstring>
#include <utility>
#include <sys/types.h>

namespace GPlatform {

class GpMemOps
{
	CLASS_REMOVE_CTRS(GpMemOps);

public:
	struct VoidDeleter
	{
		template<typename T>
		static void SDelete (T*)
		{
			//NOP
		}
	};

	struct NewDeleter
	{
		template<typename T>
		static void SDelete (T* aPtr)
		{
			GpMemOps::SDelete<T>(aPtr);
		}
	};

	struct EmplaceDeleter
	{
		template<typename T>
		static void SDelete (T* aPtr)
		{
			GpMemOps::SDestruct<T>(aPtr, 1);
		}
	};

public:
	//------------------------ std new/delete ----------------------------
	template<typename T, typename... Ts>
	[[nodiscard]] static
	T*		SNew (Ts&&... aArgs)
	{
		return new T(std::forward<Ts>(aArgs)...);
	}

	template<typename T, typename... Ts>
	[[nodiscard]] static
	T*		SEmplace (void* aPtrToPlace, Ts&&... aArgs)
	{
		return new(aPtrToPlace) T(std::forward<Ts>(aArgs)...);
	}

	template<typename T>
	static
	void	SDelete (T* aObject)
	{
		delete aObject;
	}

	//------------------------ std new/delete array ----------------------------
	template<typename T>
	static
	void	SDeleteArray (T* aObject)
	{
		delete[] aObject;
	}

	//------------------------------------------------------------------------------
	template<typename		T,
			 typename...	Ts>
	static void	SConstruct (T*				aElements,
							const count_t	aCount,
							Ts&&...			aArgs)
	{
		if constexpr (std::is_scalar<T>::value)
		{
			SConstructScalar(aElements, aCount);
		} else
		{
			SConstructNotScalar(aElements, aCount, std::forward<Ts>(aArgs)...);
		}
	}

	template<typename T>
	static void	SDestruct (T*				aElements,
						   const count_t	aCount) noexcept
	{
		if constexpr (std::is_scalar<T>::value)
		{
			SDestructScalar(aElements, aCount);
		} else
		{
			SDestructNotScalar(aElements, aCount);
		}
	}

	template<typename T>
	static constexpr void	SConstructAndMove (T& aElementsDst,
											   T&& aElementsSrc)
	{
		SConstructAndMove(&aElementsDst, &aElementsSrc, 1_cnt);
	}

	template<typename T>
	static constexpr void	SConstructAndMove (T* aElementsDst,
											   T* aElementsSrc)
	{
		SConstructAndMove(aElementsDst, aElementsSrc, 1_cnt);
	}

	template<typename T>
	static constexpr void	SConstructAndMove (T*				aElementsDst,
											   T*				aElementsSrc,
											   const count_t	aCount)
	{
		if constexpr (   std::is_scalar<T>::value
					  || (std::is_trivially_copyable_v<T> && std::is_trivial_v<T>))
		{
			SConstructAndMoveScalar(aElementsDst,
									aElementsSrc,
									aCount);
		} else
		{
			SConstructAndMoveNotScalar(aElementsDst,
									   aElementsSrc,
									   aCount);
		}
	}

	template<typename T>
	static constexpr void	SCopy (T&		aElementsDst,
								   const T&	aElementsSrc)
	{
		SCopy(&aElementsDst, &aElementsSrc, 1_cnt);
	}

	template<typename T>
	static constexpr void	SCopy (T*		aElementsDst,
								   const T*	aElementsSrc)
	{
		SCopy(aElementsDst, aElementsSrc, 1_cnt);
	}

	template<typename T>
	static constexpr void	SCopy (T*				aElementsDst,
								   const T*			aElementsSrc,
								   const count_t	aCount)
	{
		if (aCount == 0_cnt)
		{
			return;
		}

		if constexpr (   (std::is_scalar_v<T>)
					  || (std::is_trivially_copyable_v<T> && std::is_trivial_v<T>))
		{
			SCopyScalar(aElementsDst,
						aElementsSrc,
						aCount);
		} else
		{
			SCopyNotScalar(aElementsDst,
						   aElementsSrc,
						   aCount);
		}
	}

	template<typename T>
	[[nodiscard]] static constexpr ssize_t	SCompare (const T& aElementsA,
													  const T& aElementsB) noexcept
	{
		return SCompare(&aElementsA, &aElementsB, 1_cnt);
	}

	template<typename T>
	[[nodiscard]] static constexpr ssize_t	SCompare (const T* aElementsA,
													  const T* aElementsB) noexcept
	{
		return SCompare(aElementsA, aElementsB, 1_cnt);
	}

	template<typename T>
	[[nodiscard]] static constexpr ssize_t	SCompare (const T*		aElementsA,
													  const T*	 	aElementsB,
													  const count_t	aCount) noexcept
	{
		if constexpr (   (std::is_scalar<T>::value)
					  || (std::is_trivially_copyable_v<T> && std::is_trivial_v<T>))

		{
			return SCompareScalar(aElementsA,
								  aElementsB,
								  aCount);
		} else
		{
			return SCompareNotScalar(aElementsA,
									 aElementsB,
									 aCount);
		}
	}

private:
	template<typename T>
	static void SConstructScalar (T*			aElements,
								  const count_t	aCount)
	{
		static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");
		const size_t s = (aCount * count_t::SMake(sizeof(T))).ValueAs<size_t>();
		std::memset(aElements, 0, s);
	}

	template<typename		T,
			 typename...	Ts>
	static void SConstructNotScalar	(T* 			aElements,
									 const count_t	aCount,
									 Ts&&...		aArgs)
	{
		T* e = aElements;

		count_t constructedCnt = 0_cnt;
		try
		{
			for ( ; constructedCnt < aCount; ++constructedCnt)
			{
				::new (static_cast<void*>(e)) T(std::forward<Ts>(aArgs)...);
				e++;
			}
		} catch (...)
		{
			if (constructedCnt > 0_cnt)
			{
				SDestruct(aElements, constructedCnt);
			}

			throw;
		}
	}

	template<typename T>
	static void SDestructScalar (T* 			aElements,
								 const count_t	aCount) noexcept
	{
		static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");
		const size_t s = (aCount * count_t::SMake(sizeof(T))).ValueAs<size_t>();
		std::memset(aElements, 0, s);
	}

	template<typename T>
	static void SDestructNotScalar (T*				aElements,
									const count_t	aCount) noexcept
	{
		for (count_t id = 0_cnt; id < aCount; ++id)
		{
			aElements->~T();
			aElements++;
		}
	}

	template<typename T>
	static constexpr void SConstructAndMoveScalar (T*				aElementsDst,
												   T*				aElementsSrc,
												   const count_t	aCount)
	{
		static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");

		const size_t s = (aCount * count_t::SMake(sizeof(T))).ValueAs<size_t>();

		std::memcpy(aElementsDst, aElementsSrc, s);
	}

	template<typename T>
	static void SConstructAndMoveNotScalar (T*	 	 		aElementsDst,
											T*				aElementsSrc,
											const count_t	aCount)
	{
		T* dst = aElementsDst;
		T* src = aElementsSrc;

		//Move
		count_t movedCnt = 0_cnt;
		try
		{
			for ( ; movedCnt < aCount; ++movedCnt)
			{
				::new (static_cast<void*>(dst)) T(std::move(*src));
				dst++;
				src++;
			}
		} catch (...)
		{
			if (movedCnt > 0_cnt)
			{
				SDestruct(aElementsDst, movedCnt);
			}

			throw;
		}
	}

	template<typename T>
	static constexpr void	SCopyScalar (T* 	 		aElementsDst,
										 const T*		aElementsSrc,
										 const count_t	aCount)
	{
		const size_t s = (aCount * count_t::SMake(sizeof(T))).ValueAs<size_t>();
		std::memcpy(aElementsDst, aElementsSrc, s);
	}

	template<typename T>
	static void	SCopyNotScalar (T*				aElementsDst,
								const T*		aElementsSrc,
								const count_t	aCount)
	{
		T*			dst = aElementsDst;
		const T*	src = aElementsSrc;

		//Copy
		count_t copyCnt = 0_cnt;
		try
		{
			for ( ; copyCnt < aCount; ++copyCnt)
			{
				::new (static_cast<void*>(dst)) T(*src);
				dst++;
				src++;
			}
		} catch (...)
		{
			if (copyCnt > 0_cnt)
			{
				Destruct(aElementsDst, copyCnt);
			}

			throw;
		}
	}

	template<typename T>
	[[nodiscard]] static constexpr ssize_t	SCompareScalar (const T*		aElementsA,
															const T*		aElementsB,
															const count_t	aCount) noexcept
	{
		const size_t s = (aCount * count_t::SMake(sizeof(T))).ValueAs<size_t>();
		return std::memcmp(aElementsA, aElementsB, s);
	}

	template<typename T>
	[[nodiscard]]static ssize_t	SCompareNotScalar (const T*			aElementsA,
												   const T*			aElementsB,
												   const count_t	aCount) noexcept
	{
		for (count_t id = 0_cnt; id < aCount; ++id)
		{
			const T& a = *aElementsA++;
			const T& b = *aElementsB++;

			if (a > b)
			{
				return (id + 1_cnt).ValueAs<ssize_t>();
			} else if (a < b)
			{
				return -(id + 1_cnt).ValueAs<ssize_t>();
			}
		}

		return 0;
	}
};

using MemOps = GpMemOps;

}//GPlatform

#endif//#if defined(GP_USE_MEMORY_MNG)
