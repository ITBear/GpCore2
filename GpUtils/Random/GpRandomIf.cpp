#include "GpRandomIf.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpRandomStrMode)

GpRandomIf::GpRandomIf (void) noexcept
{
}

GpRandomIf::~GpRandomIf (void) noexcept
{
}

std::array<const std::string, 2> GpRandomIf::sStrs =
{
    std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"_sv),   // StrMode::ALPHA_NUM
    std::string("0123456789"_sv)                                                        // StrMode::NUM
};

}// namespace GPlatform
