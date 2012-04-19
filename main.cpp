// HotHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HotHook.h"


struct A
{
    static void foo( int x )
    {
        std::cout << "A::foo: " << x << std::endl;
    }

    void bar( int x )
    {
        std::cout << "A::bar: " << x << std::endl;
    }

    virtual void foobar( int x )
    {
        std::cout << "A::foobar: " << x << std::endl;
    }
};

struct B
{
    static void foo( int x )
    {
        std::cout << "B::foo: " << x << std::endl;
    }

    void bar( int x )
    {
        std::cout << "B::bar: " << x << std::endl;
    }

    virtual void foobar( int x )
    {
        std::cout << "B::foobar: " << x << std::endl;
    }
};


void caller()
{
    A a;
    a.foo( 1 );
    a.bar( 2 );
    a.foobar( 3 );
}


void hook_test()
{
    std::cout << "-------------------- BEFORE --------------------" << std::endl;
    caller();

    {
        std::cout << "-------------------- START  --------------------" << std::endl;
        hook_guard guard(A::foo, B::foo);
        hook_member_function_guard( A::bar, B::bar );
        hook_member_function_guard( A::foobar, B::foobar );
        caller();
    }

    std::cout << "-------------------- STOP   --------------------" << std::endl;
    caller();
}


int _tmain(int argc, _TCHAR* argv[])
{
    hook_test();

    return 0;
}

