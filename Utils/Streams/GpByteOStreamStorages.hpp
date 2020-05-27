#pragma once

#include "GpByteOStream.hpp"

namespace GPlatform {

// --------------------- Fixed size ---------------------
class GPCORE_API GpByteOStreamFixedSize: public GpByteOStream<GpByteOStreamFixedSize>
{
	friend class GpByteOStream;

public:
							GpByteOStreamFixedSize	(std::byte*		aData,
													 const count_t	aLeftFreeBytes) noexcept: GpByteOStream(aData, aLeftFreeBytes)
																							  {}

							~GpByteOStreamFixedSize	(void) noexcept = default;

protected:
	void					_AllocateNext			(const count_t aSize);
};

// --------------------- GpBytesArray ---------------------
class GPCORE_API GpByteOStreamByteArray: public GpByteOStream<GpByteOStreamByteArray>
{
	friend class GpByteOStream;

public:
							GpByteOStreamByteArray	(GpBytesArray& aOut) noexcept: GpByteOStream(aOut.data(), 0_cnt),
																				   iOut(aOut)
																				   {}
							~GpByteOStreamByteArray	(void) noexcept = default;

protected:
	void					_AllocateNext			(const count_t aSize);

private:
	GpBytesArray&			iOut;
};

}//GPlatform
