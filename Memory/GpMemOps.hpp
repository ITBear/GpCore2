#pragma once

#include "../Config/GpConfig.hpp"

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
							const size_t	aCount,
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
	static void	SDestruct (T* 			aElements,
						   const size_t	aCount) noexcept
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
	static void	SConstructAndMove (T* 			aElementsDst,
								   T* 			aElementsSrc,
								   const size_t	aCount)
	{
		if constexpr (std::is_scalar<T>::value)
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
	static void	SCopy (T* 			aElementsDst,
					   const T*		aElementsSrc,
					   const size_t	aCount)
	{
		if (aCount == 0)
		{
			return;
		}

		if constexpr (std::is_scalar<T>::value)
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
	[[nodiscard]] static ssize_t	SCompare (const T* 		aElementsA,
											  const T*		aElementsB,
											  const size_t	aCount) noexcept
	{
		if constexpr (std::is_scalar<T>::value)
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

	/*template<typename T>
	static bool	CompareSafe				(const T* 		aElementsSafe,
										 const size_t	aCountSafe,
										 const T*		aElementsUnsafe,
										 const size_t	aCountUnsafe) noexcept
	{
		static_assert((sizeof(T) == 1) && (alignof(T) == 1), "Wrong type");

		const size_t		uSize			= aCountUnsafe;
		const size_t		size			= aCountSafe;
		const T* _R_		uElements		= aElementsUnsafe;
		const T* _R_		elements		= aElementsSafe;
		volatile size_t		eualsCount		= 0;
		volatile size_t		notEualsCount	= 0;

		for (size_t id = 0; id < uSize; ++id)
		{
			if (id < size)
			{
				if (*uElements++ == *elements++)
				{
					++eualsCount;
				}
			} else
			{
				if (*uElements++ == 0)
				{
					++notEualsCount;
				}
			}
		}

		return eualsCount == uSize;
	}*/

private:
	template<typename T>
	static void SConstructScalar (T*			aElements,
								  const size_t	aCount)
	{
		static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");
		std::memset(aElements, 0, aCount*sizeof(T));
	}

	template<typename		T,
			 typename...	Ts>
	static void SConstructNotScalar	(T* 			aElements,
									 const size_t	aCount,
									 Ts&&...		aArgs)
	{
		T* e = aElements;

		size_t constructedCnt = 0;
		try
		{
			for ( ; constructedCnt < aCount; ++constructedCnt)
			{
				::new (static_cast<void*>(e)) T(std::forward<Ts>(aArgs)...);
				e++;
			}
		} catch (...)
		{
			if (constructedCnt > 0)
			{
				SDestruct(aElements, constructedCnt);
			}

			throw;
		}
	}

	template<typename T>
	static void SDestructScalar (T* 			aElements,
								 const size_t	aCount) noexcept
	{
		static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");
		std::memset(aElements, 0, aCount*sizeof(T));
	}

	template<typename T>
	static void SDestructNotScalar (T*				aElements,
									const size_t	aCount) noexcept
	{
		for (size_t id = 0; id < aCount; ++id)
		{
			aElements->~T();
			aElements++;
		}
	}

	template<typename T>
	static void SConstructAndMoveScalar (T* 			aElementsDst,
										 T* 			aElementsSrc,
										 const size_t	aCount)
	{
		static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");

		std::memcpy(aElementsDst,
					aElementsSrc,
					aCount * sizeof(T));

		std::memset(aElementsSrc, 0, aCount * sizeof(T));
	}

	template<typename T>
	static void SConstructAndMoveNotScalar (T*				aElementsDst,
											T*				aElementsSrc,
											const size_t	aCount)
	{
		T* dst = aElementsDst;
		T* src = aElementsSrc;

		//Move
		size_t movedCnt = 0;
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
			if (movedCnt > 0)
			{
				SDestruct(aElementsDst, movedCnt);
			}

			throw;
		}
	}

	template<typename T>
	static void	CopyScalar (T*				aElementsDst,
							const T*		aElementsSrc,
							const size_t	aCount)
	{
		std::memcpy(aElementsDst,
					aElementsSrc,
					aCount * sizeof(T));
	}

	template<typename T>
	static void	CopyNotScalar (T* 			aElementsDst,
							   const T*		aElementsSrc,
							   const size_t	aCount)
	{
		T*			dst = aElementsDst;
		const T*	src = aElementsSrc;

		//Copy
		size_t copyCnt = 0;
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
			if (copyCnt > 0)
			{
				Destruct(aElementsDst, copyCnt);
			}

			throw;
		}
	}

	template<typename T>
	[[nodiscard]] static ssize_t	SCompareScalar (const T* 	aElementsA,
													const T*		aElementsB,
													const size_t	aCount) noexcept
	{
		return std::memcmp(aElementsA, aElementsB, aCount * sizeof(T));
	}

	template<typename T>
	[[nodiscard]]static ssize_t	SCompareNotScalar (const T* 	aElementsA,
												   const T*		aElementsB,
												   const size_t	aCount) noexcept
	{
		for (size_t id = 0; id < aCount; ++id)
		{
			const T& a = *aElementsA++;
			const T& b = *aElementsB++;

			if (a > b)
			{
				return ssize_t(id + 1);
			} else if (a < b)
			{
				return -ssize_t(id + 1);
			}
		}

		return 0;
	}
};

using MemOps = GpMemOps;

}//GPlatform

#endif//#if defined(GP_USE_MEMORY_MNG)
