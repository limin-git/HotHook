#include "stdafx.h"
#include "HotHook.h"


#define FUNCTION_ENTRY(x)
#define FUNCTION_EXIT


hook_guard::hook_guard( void* src, void* dest )
    : m_src( src )
{
    FUNCTION_ENTRY( "hook_guard" );

    memcpy( m_src_instruction_backup, src, 5 );
    start_hook( src, dest );

    FUNCTION_EXIT;
}


hook_guard::~hook_guard()
{
    FUNCTION_ENTRY( "~hook_guard" );

    stop_hook();

    FUNCTION_EXIT;
}


void hook_guard::start_hook( void* src, void* dest )
{
    FUNCTION_ENTRY( "start_hook" );

    unsigned long m_old_protect;

    VirtualProtect( src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
    memcpy( src, dest, 5 );

    // adjust short jmp address
    long* jump_to_address = reinterpret_cast<long*>( reinterpret_cast<unsigned long>( src ) + 1 );
    *jump_to_address += reinterpret_cast<unsigned long>( dest );
    *jump_to_address -= reinterpret_cast<unsigned long>( src );

    VirtualProtect( src, 5, PAGE_EXECUTE_READ, &m_old_protect );

    FUNCTION_EXIT;
}


void hook_guard::stop_hook()
{
    FUNCTION_ENTRY( "stop_hook" );

    unsigned long m_old_protect;

    VirtualProtect( m_src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
    memcpy( m_src, m_src_instruction_backup, 5 );
    VirtualProtect( m_src, 5, PAGE_EXECUTE_READ, &m_old_protect );

    FUNCTION_EXIT;
}

