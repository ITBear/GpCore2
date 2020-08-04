#include "GpRawPtr.hpp"
#include <vector>

//https://foonathan.net/2016/12/conditionally-removing-functions/

namespace GPlatform {

class A
{
public:
    int val = 10;
};

void Constructor()
{
    /*std::vector<A>    a;  a.resize(10);
    std::string     s;  s.resize(10);

    {
        //GpRawPtr<A*>  a1;                                                 //GpRawPtr  (void) noexcept = delete;
        GpRawPtr<A*>    a2(a.data(), count_t::SMake(a.size()));             //GpRawPtr  (pointer_type aPtr, const count_t aCount);
        GpRawPtr<A*>    a3(a.data(), count_t::SMake(a.size()), 0_cnt);      //GpRawPtr  (pointer_type aPtr, const count_t aCount, const count_t aOffset);
        GpRawPtr<A*>    a4(a2);                                             //GpRawPtr  (const this_type& aRawPtr) noexcept;
        GpRawPtr<A*>    a5(std::move(a2));                                  //GpRawPtr  (this_type&& aRawPtr) noexcept;
        GpRawPtr<A*>    a6(a.data(), a.size());                             //GpRawPtr  (P aPtr, const size_t aSize);
        //GpRawPtr<A*>  a7(s.data(), s.size());                             //GpRawPtr  (P aPtr, const size_t aSize); char* -> A*
        GpRawPtr<A*>    a8(a);                                              //GpRawPtr  (const C& aContainer);
        const GpRawPtr<A*>  a9(a8);
        const GpRawPtr<A*>  a10(a9);
        //GpRawPtr<A*>  a9(s);

        //GpRawPtr<A*>  s1;                                                 //GpRawPtr  (void) noexcept = delete;
        GpRawPtr<char*> s2(a.data(), count_t::SMake(a.size()));             //GpRawPtr  (pointer_type aPtr, const count_t aCount);
        GpRawPtr<char*> s3(a2);                                             //GpRawPtr  (const this_type& aRawPtr) noexcept;
        GpRawPtr<char*> s4(a.data(), a.size());                             //GpRawPtr  (P aPtr, const size_t aSize);
        GpRawPtr<char*> s5(a.data(), a.size());                             //GpRawPtr  (P aPtr, const size_t aSize); char* -> A*
        GpRawPtr<char*> s6(s);
        GpRawPtr<char*> s7(a);
    }

    {
        //GpRawPtr<A*>  a1;                                                 //GpRawPtr  (void) noexcept = delete;
        GpRawPtr<const A*>  a2(a.data(), count_t::SMake(a.size()));             //GpRawPtr  (pointer_type aPtr, const count_t aCount);
        GpRawPtr<const A*>  a3(a.data(), count_t::SMake(a.size()), 0_cnt);      //GpRawPtr  (pointer_type aPtr, const count_t aCount, const count_t aOffset);
        GpRawPtr<const A*>  a4(a2);                                             //GpRawPtr  (const this_type& aRawPtr) noexcept;
        GpRawPtr<const A*>  a5(std::move(a2));                                  //GpRawPtr  (this_type&& aRawPtr) noexcept;
        GpRawPtr<const A*>  a6(a.data(), a.size());                             //GpRawPtr  (P aPtr, const size_t aSize);
        //GpRawPtr<const A*>    a7(s.data(), s.size());                             //GpRawPtr  (P aPtr, const size_t aSize); char* -> A*
        GpRawPtr<const A*>  a8(a);
        const GpRawPtr<const A*>    a9(a8);
        const GpRawPtr<const A*>    a10(a9);
        //GpRawPtr  (const C& aContainer);
        //GpRawPtr<const A*>    a9(s);

        //GpRawPtr<const A*>    s1;                                                 //GpRawPtr  (void) noexcept = delete;
        GpRawPtr<const char*>   s2(a.data(), count_t::SMake(a.size()));             //GpRawPtr  (pointer_type aPtr, const count_t aCount);
        GpRawPtr<const char*>   s3(a2);                                             //GpRawPtr  (const this_type& aRawPtr) noexcept;
        GpRawPtr<const char*>   s4(a.data(), a.size());                             //GpRawPtr  (P aPtr, const size_t aSize);
        GpRawPtr<const char*>   s5(a.data(), a.size());                             //GpRawPtr  (P aPtr, const size_t aSize); char* -> A*

        const std::string&  _s = s;
        const std::vector<A>& _a = a;

        GpRawPtr<const char*>   s6(_s);
        GpRawPtr<const char*>   s7(_a);
    }*/
}

void    Set (void)
{
    /*const std::vector<A>  a;//    a.resize(10);
    const std::string       s;//    s.resize(10);

    std::vector<A>  _a;//   a.resize(10);
    std::string     _s;//   s.resize(10);

    GpRawPtr<const char*>   const_char_ptr_1(a);
    GpRawPtr<const char*>   const_char_ptr_2(s);
    GpRawPtr<char*>         char_ptr_1(_a);
    GpRawPtr<char*>         char_ptr_2(_s);

    const_char_ptr_1.Set(const_char_ptr_2);
    const_char_ptr_1.Set(std::move(const_char_ptr_2));
    const_char_ptr_1.Set(a.data(), a.size());
    const_char_ptr_1.Set(s.data(), s.size());
    const_char_ptr_1.Set(a);
    const_char_ptr_1.Set(s);
    const_char_ptr_1.Set(const_char_ptr_1);
    const_char_ptr_1.Set(const_char_ptr_2);
    const_char_ptr_1.Set(char_ptr_1);
    const_char_ptr_1.Set(char_ptr_2);

    char_ptr_1.Set(char_ptr_2);
    char_ptr_1.Set(std::move(char_ptr_2));
    char_ptr_1.Set(_a.data(), _a.size());
    char_ptr_1.Set(_s.data(), _s.size());
    char_ptr_1.Set(_a);
    char_ptr_1.Set(_s);
    char_ptr_1.Set(char_ptr_1);
    char_ptr_1.Set(char_ptr_2);
    char_ptr_1.Set(char_ptr_1);
    char_ptr_1.Set(char_ptr_2);*/
}

void    OperatorEQ (void)
{
    /*std::vector<A>    a;  a.resize(10);
    std::string     s;  s.resize(10);

    GpRawPtr<const char*>   const_char_ptr_1(a);
    GpRawPtr<const char*>   const_char_ptr_2(s);
    GpRawPtr<char*>         char_ptr_1(a);
    GpRawPtr<char*>         char_ptr_2(s);

    const_char_ptr_1 = a;
    const_char_ptr_1 = s;
    const_char_ptr_1 = const_char_ptr_2;
    const_char_ptr_1 = char_ptr_1;

    char_ptr_1 = a;
    char_ptr_1 = s;
    char_ptr_1 = char_ptr_2;*/
}

void    Offset (void)
{
    /*std::vector<A>    a;  a.resize(10);
    std::string     s;  s.resize(10);

    GpRawPtr<const char*>   const_char_ptr_1(a);
    GpRawPtr<const char*>   const_char_ptr_2(s);
    GpRawPtr<char*>         char_ptr_1(a);
    GpRawPtr<char*>         char_ptr_2(s);

    const_char_ptr_1.OffsetAdd(1_cnt);
    const_char_ptr_1.OffsetSub(1_cnt);
    const_char_ptr_1.OffsetZero();
    const_char_ptr_1.OffsetAs<char*>();
    //const_char_ptr_1.OffsetAs<A*>();*/
}

void    Ptr (void)
{
    /*std::vector<A>    a;  a.resize(10);
    std::string     s;  s.resize(10);

    GpRawPtr<const char*>   const_char_ptr_1(a);
    GpRawPtr<const char*>   const_char_ptr_2(s);
    GpRawPtr<char*>         char_ptr_1(a);
    GpRawPtr<char*>         char_ptr_2(s);

    [[maybe_unused]] const char* cc = nullptr;
    cc = char_ptr_1.Ptr();
    cc = char_ptr_1.Ptr(1_cnt);
    cc = const_char_ptr_1.Ptr();
    cc = const_char_ptr_1.Ptr(1_cnt);

    [[maybe_unused]] char* c = nullptr;
    c = char_ptr_1.Ptr();
    c = char_ptr_1.Ptr(1_cnt);
    //c = const_char_ptr_1.Ptr();
    //c = const_char_ptr_1.Ptr(1_cnt);*/
}

void    CopyFrom (void)
{
    /*std::vector<A>    a;  a.resize(10);
    std::string     s;  s.resize(10);

    GpRawPtr<const char*>   const_char_ptr_1(a);
    GpRawPtr<const char*>   const_char_ptr_2(s);
    GpRawPtr<char*>         char_ptr_1(a);
    GpRawPtr<char*>         char_ptr_2(s);
    GpRawPtr<A*>            a_ptr_2(a);
    GpRawPtr<const A*>      const_a_ptr_2(a_ptr_2);

    char_ptr_1.CopyFrom(const_char_ptr_1);
    char_ptr_1.CopyFrom(char_ptr_2);
    char_ptr_1.CopyFrom(a);
    char_ptr_1.CopyFrom(s);*/
}

void    Subrange (void)
{
    /*std::vector<A>    a;  a.resize(10);
    std::string     s;  s.resize(10);

    GpRawPtr<const char*>   const_char_ptr_1(a);
    GpRawPtr<const char*>   const_char_ptr_2(s);
    GpRawPtr<char*>         char_ptr_1(a);
    GpRawPtr<char*>         char_ptr_2(s);
    GpRawPtr<A*>            a_ptr_2(a);
    GpRawPtr<const A*>      const_a_ptr_2(a_ptr_2);

    char_ptr_1.Subrange(0_cnt, 1_cnt);
    char_ptr_1.SubrangeAs<GpRawPtr<char*>>(0_cnt, 1_cnt);
    char_ptr_1.SubrangeAs<GpRawPtr<const char*>>(0_cnt, 1_cnt);*/
}

void    Test (void)
{
    Constructor();
    Set();
    OperatorEQ();
    Offset();
    Ptr();
    CopyFrom();
    Subrange();
}

}//namespace GPlatform
