#include "GpTypeStructBase.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

const GpTypeStructInfo& GpTypeStructBase::STypeStructInfo (void)
{
    static const GpTypeStructInfo sStructInfo;
    return sStructInfo;
}

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM