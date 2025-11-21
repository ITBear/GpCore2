#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>

namespace GPlatform {

class GP_UTILS_API GpStaticInitializer
{
protected:
                    GpStaticInitializer     (void) noexcept;

public:
    virtual         ~GpStaticInitializer    (void) noexcept;

    void            Initialize              (void);

protected:
    virtual void    OnInitialize            (void) = 0;
};

}// namespace GPlatform

#define GP_STATIC_INITIALIZER(PREFIX, LIB_NAME) \
    class PREFIX LIB_NAME##_StaticInitializer \
    { \
    public: \
        static void SInitialize (void); \
    private: \
        void        OnInitialize (void); \
    };

#define GP_STATIC_INITIALIZER_IMPL(LIB_NAME) \
    void LIB_NAME##_StaticInitializer::SInitialize (void) \
    { \
        static std::atomic_flag sIsInitialized = false; \
        if (sIsInitialized.test_and_set() == true) \
        { \
            return; \
        } \
        LIB_NAME##_StaticInitializer{}.OnInitialize(); \
    }

#define GP_DO_STATIC_INIT(LIB_NAME) \
    LIB_NAME##_StaticInitializer::SInitialize()
