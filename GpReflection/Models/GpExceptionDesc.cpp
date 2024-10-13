#include <GpCore2/GpReflection/Models/GpExceptionDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

#if defined(GP_USE_EXCEPTIONS)

namespace GPlatform {

REFLECT_IMPLEMENT(GpExceptionDesc, GP_MODULE_UUID)

GpExceptionDesc::GpExceptionDesc (void) noexcept
{
}

GpExceptionDesc::GpExceptionDesc (const GpExceptionDesc& aDesc):
GpReflectObject{aDesc},
message      {GpReflectUtils::SCopyValue(aDesc.message)},
line         {GpReflectUtils::SCopyValue(aDesc.line)},
column       {GpReflectUtils::SCopyValue(aDesc.column)},
file_name    {GpReflectUtils::SCopyValue(aDesc.file_name)},
function_name{GpReflectUtils::SCopyValue(aDesc.function_name)}
{
}

GpExceptionDesc::GpExceptionDesc (GpExceptionDesc&& aDesc) noexcept:
GpReflectObject{std::move(aDesc)},
message      {std::move(aDesc.message)},
line         {std::move(aDesc.line)},
column       {std::move(aDesc.column)},
file_name    {std::move(aDesc.file_name)},
function_name{std::move(aDesc.function_name)}
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
    line            = NumOps::SConvert<decltype(line)>(aSourceLocation.line());
    column          = NumOps::SConvert<decltype(column)>(aSourceLocation.column());
    file_name       = aSourceLocation.file_name();
    function_name   = aSourceLocation.function_name();
}

void    GpExceptionDesc::SetFromExceptionGP (const GpException& aException)
{
    message         = aException.Message();
    line            = NumOps::SConvert<decltype(line)>(aException.SourceLocation().line());
    column          = NumOps::SConvert<decltype(column)>(aException.SourceLocation().column());
    file_name       = aException.SourceLocation().file_name();
    function_name   = aException.SourceLocation().function_name();
}

void    GpExceptionDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(message);
    PROP(line);
    PROP(column);
    PROP(file_name);
    PROP(function_name);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
