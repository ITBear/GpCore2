#include <GpCore2/GpUtils/Random/GpRandomIf.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpRandomStrMode)

std::array<const std::string, 4> GpRandomIf::sStrs =
{
    "0123456789",                                                                                   // StrMode::NUM
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",                               // StrMode::ALPHA_NUM
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_+/=",                          // StrMode::ALPHA_NUM_AND_SPECIAL
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()[]{}_+=-\\/|?.,;<>"   // StrMode::ALL_VISIBLE
};

GpRandomIf::GpRandomIf (void) noexcept
{
}

GpRandomIf::~GpRandomIf (void) noexcept
{
}

}// namespace GPlatform
