#include "GpExceptionDesc.hpp"

#if defined(GP_USE_EXCEPTIONS)
#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpExceptionDesc, GP_MODULE_UUID)

GpExceptionDesc::GpExceptionDesc (void) noexcept
{
}

GpExceptionDesc::~GpExceptionDesc (void) noexcept
{
}

void    GpExceptionDesc::SetFromExceptionSTD
(
    const std::exception&   aException,
    const SourceLocationT&  aSourceLocation
)
{
    message         = "std::exception: "_sv + aException.what();
    line            = count_t::SMake(aSourceLocation.line());
    column          = count_t::SMake(aSourceLocation.column());
    file_name       = aSourceLocation.file_name();
    function_name   = aSourceLocation.function_name();
}

void    GpExceptionDesc::SetFromExceptionGP (const GpException& aException)
{
    message         = aException.Message();
    line            = count_t::SMake(aException.SourceLocation().line());
    column          = count_t::SMake(aException.SourceLocation().column());
    file_name       = aException.SourceLocation().file_name();
    function_name   = aException.SourceLocation().function_name();
}

void    GpExceptionDesc::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(message);
    PROP(line);
    PROP(column);
    PROP(file_name);
    PROP(function_name);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
#endif//#if defined(GP_USE_EXCEPTIONS)
