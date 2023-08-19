#pragma once

#include "GpLifetimeElement.hpp"

namespace GPlatform {

/*
class GP_UTILS_API GpLifetimeManager
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpLifetimeManager)
    TAG_SET(THREAD_SAFE)

public:
                                GpLifetimeManager   (void) noexcept;
                                ~GpLifetimeManager  (void) noexcept;

    static GpLifetimeManager&   S                   (void) noexcept {return sInstance;}

    void                        Init                (GpLifetimeElement::SP aElement);
    void                        InitDependent       (GpLifetimeElement::SP aParentElement);
    void                        Destroy             (GpLifetimeElement::SP aElement);

private:
    GpRWSpinLock                iLock;


    static GpLifetimeManager    sInstance;
};*/

}//namespace GPlatform
