// HotHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HotHook.h"


void foo( int x )
{
    std::cout << "foo: " << x << std::endl;
}

void bar( int x )
{
    std::cout << "bar: " << x << std::endl;
}


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


struct I_v
{
    virtual void foo( int x ) = 0;
    virtual void bar( int x ) = 0;
};

struct A_v : I_v
{
    virtual void foo( int x )
    {
        std::cout << "A_v::foo: " << x << std::endl;
    }

    virtual void bar( int x )
    {
        std::cout << "A_v::bar: " << x << std::endl;
    }
};

struct B_v : I_v
{
    virtual void foo( int x )
    {
        std::cout << "B_v::foo: " << x << std::endl;
    }

    virtual void bar( int x )
    {
        std::cout << "B_v::bar: " << x << std::endl;
    }
};


void caller()
{
    foo( 1 );
    A().foo( 2 );
    A_v().foo( 3 );
}


void hook_test()
{
    std::cout << "-------------------- BEFORE --------------------" << std::endl;
    caller();

    {
        std::cout << "-------------------- START  --------------------" << std::endl;
        hook_guard guard;
        guard.hook_function( foo, bar );
        guard.hook_member_function( &A::foo, &B::bar );
        guard.hook_virtual_member_function( &A_v(), 0, &B::bar );
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

