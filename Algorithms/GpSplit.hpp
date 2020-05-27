#pragma once

#include "../Types/Units/GpUnits.hpp"
#include "../Types/Strings/GpStringOpsGlob.hpp"
#include "../Types/Strings/GpStringLiterals.hpp"
#include "../Memory/GpMemOps.hpp"
#include "../Exceptions/GpExceptions.hpp"

namespace GPlatform {
namespace Algo {

enum class SplitMode
{
	SKIP_ZERO_LENGTH_PARTS,
	COUNT_ZERO_LENGTH_PARTS
};

template<typename Element,
		 typename ContainerRes
		 >
[[nodiscard]]
ContainerRes	Split (const Element* _R_	aElements,
					   const count_t		aElementsCount,
					   const Element* _R_	aDelim,
					   const count_t		aDelimSize,
					   const count_t		aReturnPartsCountLimit,	//(0 - no liits)
					   const count_t		aDelimCountLimit,		//(0 - no liits)
					   const SplitMode		aSplitMode
					   )
{
	THROW_GPE_COND_CHECK_M(aElements != nullptr, "aElements is nullptr"_sv);
	THROW_GPE_COND_CHECK_M(aDelim != nullptr, "aDelim is nullptr"_sv);
	THROW_GPE_COND_CHECK_M(aDelimSize > 0_cnt, "aDelimSize is 0"_sv);

	const Element* _R_	partBegin	= aElements;
	size_t				partLength	= 0;
	size_t				delimCount	= 0;

	ContainerRes res;

	const size_t	elementsCount	= aElementsCount.ValueAs<size_t>();
	size_t			elementsLeft	= elementsCount;
	const size_t	delimCountLimit	= aDelimCountLimit.ValueAs<size_t>();

	for (size_t id = 0; id < elementsCount; )
	{
		if (   (aDelimSize.Value() <= elementsLeft)
			&& (std::memcmp(aElements, aDelim, aDelimSize.Value()) == 0))
		{
			if (   (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS)
				|| (partLength > 0))
			{				
				res.emplace_back(partBegin, partLength);

				if (   (aReturnPartsCountLimit > 0_cnt)
					&& (res.size() >= aReturnPartsCountLimit.Value()))
				{
					return res;
				}
			}

			++delimCount;
			if (   (delimCountLimit > 0)
				&& (delimCount >= delimCountLimit))
			{
				return res;
			}

			aElements	+= aDelimSize.Value();
			id			+= aDelimSize.Value();
			partBegin	= aElements;
			partLength	= 0;
		} else
		{
			++aElements;
			++id;
			++partLength;
		}
	}

	if (aElementsCount > 0_cnt)
	{
		if (   (partLength > 0)
			|| (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS))
		{
				res.emplace_back(partBegin, partLength);
		}
	}

	return res;
}

template<typename Element,
		 typename ContainerRes
		 >
[[nodiscard]]
ContainerRes	Split (const Element* _R_	aElements,
					   const count_t		aElementsCount,
					   const Element		aDelim,
					   const Element		aEscGroupBegin,
					   const Element		aEscGroupEnd,
					   const count_t		aReturnPartsCountLimit,	//(0 - no liits)
					   const count_t		aDelimCountLimit,		//(0 - no liits)
					   const SplitMode		aSplitMode
					   )
{
	const Element* _R_	partBegin	= aElements;
	size_t				partLength	= 0;
	size_t				delimCount	= 0;

	//Результат
	ContainerRes res;

	bool			isInEscGroup	= false;
	const size_t	elementsCount	= aElementsCount.ValueAs<size_t>();
	const size_t	delimCountLimit	= aDelimCountLimit.ValueAs<size_t>();

	for (size_t id = 0; id < elementsCount; ++id)
	{
		Element currentElement = *aElements++;

		if (   (currentElement == aEscGroupBegin)
			&& (isInEscGroup == false))
		{
			isInEscGroup = true;
			++partLength;
		} else if (   (currentElement == aEscGroupEnd)
				   && (isInEscGroup == true))
		{
			isInEscGroup = false;
			++partLength;
		} else if (   (currentElement == aDelim)
				   && (isInEscGroup == false))
		{
			if (   (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS)
				|| (partLength > 0))
			{
				res.emplace_back(partBegin, partLength);

				if (   (aReturnPartsCountLimit > 0_cnt)
					&& (res.size() >= aReturnPartsCountLimit.Value()))
				{
					return res;
				}
			}

			++delimCount;
			if (   (delimCountLimit > 0)
				&& (delimCount >= delimCountLimit))
			{
				return res;
			}

			partBegin	= aElements;
			partLength	= 0;
		} else
		{
			++partLength;
		}
	}

	if (aElementsCount > 0_cnt)
	{
		if (   (partLength > 0)
			|| (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS))
		{
			res.emplace_back(partBegin, partLength);
		}
	}

	return res;
}

}//Algo
}//GPlatform
