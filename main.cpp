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

    {
        std::cout << "[FUNCTION HOT HOOK] ==> START" << std::endl;
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

    {
        std::cout << "[MEMBER FUNCTION HOT HOOK] ==> START" << std::endl;
        hook_guard guard( &A::foo, &B::bar );
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
    A_v().foo( 1 );
    B_v().bar( 2 );
}

void hot_hook_virtual_member_function()
{
    std::cout << "[VIRTUAL MEMBER FUNCTION HOT HOOK] ==> BEFORE" << std::endl;
    virtual_member_function_caller();

    {
        std::cout << "[VIRTUAL MEMBER FUNCTION HOT HOOK] ==> START" << std::endl;
        hook_guard guard( &A_v(), &B_v(), 0 );
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

