#pragma once

/*#include "../GpMacro.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "../SyncPrimitives/GpSpinLockRW.hpp"
#include "../Exceptions/GpException.hpp"
#include "../Types/Strings/GpStringOps.hpp"

#include <mutex>*/

namespace GPlatform {

/*
class GpLifetimeElement
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpLifetimeElement)
    CLASS_DD(GpLifetimeElement)
    TAG_SET(THREAD_SAFE)

public:
    inline                  GpLifetimeElement   (std::string aName) noexcept;
    virtual                 ~GpLifetimeElement  (void) noexcept = default;

    std::string_view        Name                (void) const noexcept {return iName;}

    inline void             OnInit              (void);
    inline void             OnDestroy           (void);

protected:
    GpSpinLockRW&           Lock                (void) const {return iLock;}

    virtual void            _OnInit             (void) = 0;
    virtual void            _OnDestroy          (void) = 0;

private:
    mutable GpSpinLockRW    iLock;
    const std::string       iName;
    bool                    iIsInit = false;
};

GpLifetimeElement::GpLifetimeElement (std::string aName) noexcept:
iName(std::move(aName))
{
}

void    GpLifetimeElement::OnInit (void)
{
    std::scoped_lock lock(iLock);

    THROW_COND_GP
    (
        iIsInit == true,
        [&](){return "Object '"_sv + Name() + "' already initialized"_sv;}
    );

    _OnInit();
    iIsInit = true;
}

void    GpLifetimeElement::OnDestroy (void)
{
    std::scoped_lock lock(iLock);

    if (iIsInit == false)
    {
        return;
    }

    _OnDestroy();
    iIsInit = false;
}*/

}// namespace GPlatform
