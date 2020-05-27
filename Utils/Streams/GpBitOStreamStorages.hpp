#pragma once

#include "GpBitOStream.hpp"

namespace GPlatform {

// --------------------- Fixed size ---------------------
class GPCORE_API GpBitOStreamFixedSize: public GpBitOStream<GpBitOStreamFixedSize>
{
	friend class GpBitOStream;

public:
							GpBitOStreamFixedSize	(std::byte*			aData,
													 const size_bit_t	aCount,
													 const size_bit_t	aOffset) noexcept: GpBitOStream(aData, aCount, aOffset)
																						   {}

							~GpBitOStreamFixedSize	(void) noexcept = default;

protected:
	void					_AllocateNext			(const size_bit_t aSize);
};

// --------------------- GpBytesArray ---------------------
/*class GPCORE_API GpBitOStreamByteArray: public GpBitOStream<GpBitOStreamByteArray>
{
	friend class GpBitOStream;

public:
							GpBitOStreamByteArray	(GpBytesArray& aOut) noexcept: GpBitOStream(aOut.data(), 0_bit, 0_bit),
																				   iOut(aOut)
																				   {}
							~GpBitOStreamByteArray	(void) noexcept = default;

protected:
	void					_AllocateNext			(const size_bit_t aSize);

private:
	GpBytesArray&			iOut;
};*/

}//GPlatform
