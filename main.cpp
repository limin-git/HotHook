// HotHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HotHook.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION HOT HOOK
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void foo( int x )
{
    std::cout << "foo: " << x << std::endl;
}

void bar( int x )
{
    std::cout << "bar: " << x << std::endl;
}

void function_caller()
{
    foo( 1 );
    bar( 2 );
}

void hot_hook_function()
{
    std::cout << "[FUNCTION HOT HOOK] ==> BEFORE" << std::endl;
    function_caller();

    std::cout << "[FUNCTION HOT HOOK] ==> START" << std::endl;

    {
        void* src_address = foo;
        void* dest_address = bar;

        hook_guard guard( foo, bar );
        function_caller();
    }

    std::cout << "[FUNCTION HOT HOOK] ==> STOP" << std::endl;
    function_caller();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEMBER FUNCTION HOT HOOK
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct A
{
    void foo( int x )
    {
        std::cout << "A::foo: " << x << std::endl;
    }
};

struct B
{
    void bar( int x )
    {
        std::cout << "B::bar: " << x << std::endl;
    }
};

void member_function_caller()
{
    A().foo( 1 );
    B().bar( 2 );
}

void hot_hook_member_function()
{
    std::cout << "[MEMBER FUNCTION HOT HOOK] ==> BEFORE" << std::endl;
    member_function_caller();

    std::cout << "[MEMBER FUNCTION HOT HOOK] ==> START" << std::endl;

    {
        typedef void (A::* AMemFun)(int);
        AMemFun foo_ptr = &A::foo;
        void* src = (void*)( *( ( long *)&foo_ptr ) );

        typedef void (B::* BMemFun)(int);
        BMemFun bar_ptr = &B::bar;
        void* dest = (void*)( *( ( long *)&bar_ptr ) );

        hook_guard guard( src, dest );
        member_function_caller();
    }

    std::cout << "[MEMBER FUNCTION HOT HOOK] ==> STOP" << std::endl;
    member_function_caller();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VIRTUAL MEMBER FUNCTION HOT HOOK
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct A_v
{
    virtual void foo( int x )
    {
        std::cout << "A_v::foo: " << x << std::endl;
    }
};

struct B_v
{
    virtual void bar( int x )
    {
        std::cout << "B_v::bar: " << x << std::endl;
    }
};

void virtual_member_function_caller()
{
    A_v* pa = new A_v;
    pa->foo( 1 );
    delete pa;

    B_v* pb = new B_v;
    pb->bar( 2 );
    delete pb;
}

void hot_hook_virtual_member_function()
{
    std::cout << "[VIRTUAL MEMBER FUNCTION HOT HOOK] ==> BEFORE" << std::endl;
    virtual_member_function_caller();

    std::cout << "[VIRTUAL MEMBER FUNCTION HOT HOOK] ==> START" << std::endl;

    {
        A_v* pa = new A_v;
        unsigned long** vtable_a = reinterpret_cast<unsigned long**>(pa);
        void* src = (void*)( (*vtable_a)[0] );
        delete pa;

        B_v* pb = new B_v;
        unsigned long** vtable_b = reinterpret_cast<unsigned long**>(pb);
        void* dest = (void*)( (*vtable_b)[0] );
        delete pb;

        hook_guard guard( src, dest );
        virtual_member_function_caller();
    }

    std::cout << "[VIRTUAL MEMBER FUNCTION HOT HOOK] ==> STOP" << std::endl;
    virtual_member_function_caller();
}


int _tmain(int argc, _TCHAR* argv[])
{
    hot_hook_function();
    hot_hook_member_function();
    hot_hook_virtual_member_function();

    return 0;
}

