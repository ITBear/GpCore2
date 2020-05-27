#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

template<typename StorageT>
class GpByteOStream
{
	CLASS_REMOVE_CTRS(GpByteOStream);

protected:
							GpByteOStream	(std::byte*		aData,
											 const count_t	aLeftFreeBytes) noexcept;

public:
							~GpByteOStream	(void) noexcept = default;

	void					UInt8			(const u_int_8 aValue);
	void					SInt8			(const s_int_8 aValue);
	void					UInt16			(const u_int_16 aValue);
	void					SInt16			(const s_int_16 aValue);
	void					UInt32			(const u_int_32 aValue);
	void					SInt32			(const s_int_32 aValue);
	void					UInt64			(const u_int_64 aValue);
	void					SInt64			(const s_int_64 aValue);
	void					USizeT			(const size_t aValue);
	void					SSizeT			(const ssize_t aValue);
	void					BytesWithLen	(std::string_view aValue);
	void					Bytes			(std::string_view aValue);
	void					BytesWithLen	(const GpBytesArray& aValue);
	void					BytesWithLen	(const std::byte* aData, const count_t aCount);
	void					Bytes			(const GpBytesArray& aValue);
	void					Bytes			(const std::byte* aData, const count_t aCount);

	void					CompactSInt32	(const s_int_32 aValue);

private:
	void					AllocateNext	(const count_t aSize);

	template<typename T>
	void					WritePOD		(const T aValue)
	{
		AllocateNext(count_t::SMake(sizeof(aValue)));

		const T nVal = BitOps::H2N(aValue);

		std::memcpy(iData, &nVal, sizeof(T));
		iLeftFreeBytes	 = count_t::SMake(iLeftFreeBytes.Value() - sizeof(T));
		iData			+= sizeof(T);
	}

protected:
	std::byte*				iData			= nullptr;
	count_t					iLeftFreeBytes	= 0_cnt;
};

template<typename StorageT>
GpByteOStream<StorageT>::GpByteOStream (std::byte*		aData,
										const count_t	aLeftFreeBytes) noexcept:
iData(aData),
iLeftFreeBytes(aLeftFreeBytes)
{
}

template<typename StorageT>
void	GpByteOStream<StorageT>::UInt8 (const u_int_8 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::SInt8 (const s_int_8 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::UInt16 (const u_int_16 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::SInt16 (const s_int_16 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::UInt32 (const u_int_32 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::SInt32 (const s_int_32 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::UInt64 (const u_int_64 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::SInt64 (const s_int_64 aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::USizeT (const size_t aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::SSizeT (const ssize_t aValue)
{
	WritePOD<decltype(aValue)>(aValue);
}

template<typename StorageT>
void	GpByteOStream<StorageT>::BytesWithLen (std::string_view aValue)
{
	BytesWithLen(reinterpret_cast<const std::byte*>(aValue.data()),
				 count_t::SMake(aValue.size()));
}

template<typename StorageT>
void	GpByteOStream<StorageT>::Bytes (std::string_view aValue)
{
	Bytes(reinterpret_cast<const std::byte*>(aValue.data()),
		  count_t::SMake(aValue.size()));
}

template<typename StorageT>
void	GpByteOStream<StorageT>::BytesWithLen (const GpBytesArray& aValue)
{
	BytesWithLen(aValue.data(),
				 count_t::SMake(aValue.size()));
}

template<typename StorageT>
void	GpByteOStream<StorageT>::BytesWithLen (const std::byte* aData, const count_t aCount)
{
	if ((aData == nullptr) || (aCount == 0_cnt))
	{
		return;
	}

	//Length
	SInt32(aCount.ValueAs<s_int_32>());

	//Data
	AllocateNext(aCount);
	std::memcpy(iData, aData, aCount.Value());
	iLeftFreeBytes	-= aCount;
	iData			+= aCount.Value();
}

template<typename StorageT>
void	GpByteOStream<StorageT>::Bytes (const GpBytesArray& aValue)
{
	Bytes(aValue.data(),
		  count_t::SMake(aValue.size()));
}

template<typename StorageT>
void	GpByteOStream<StorageT>::Bytes (const std::byte* aData, const count_t aCount)
{
	if ((aData == nullptr) || (aCount == 0_cnt))
	{
		return;
	}

	//Data
	AllocateNext(aCount);
	std::memcpy(iData, aData, aCount.Value());
	iLeftFreeBytes	-= aCount;
	iData			+= aCount.Value();
}

template<typename StorageT>
void	GpByteOStream<StorageT>::CompactSInt32 (const s_int_32 aValue)
{
	THROW_GPE_COND_CHECK_M((aValue >= s_int_32(0)) && (aValue <= s_int_32(0x0FFFFFFF)), "aValue is out of range");

	std::array <u_int_32, 4> buf;

	size_t i = buf.size();
	s_int_32 value = aValue;
	do
	{
		buf[--i] = value & 0x7f;
		value = value >> 7;
	} while (value > 0);

	while (i < buf.size())
	{
		if (i != buf.size() - 1)
		{
			UInt8(u_int_8(buf[i++] | 0x80));
		} else
		{
			UInt8(u_int_8(buf[i++]));
		}
	}
}

template<typename StorageT>
void	GpByteOStream<StorageT>::AllocateNext (const count_t aSize)
{
	static_cast<StorageT*>(this)->_AllocateNext(aSize);
}

}//GPlatform
