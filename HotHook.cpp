#include "stdafx.h"
#include "HotHook.h"


void hook_guard::start_hook( void* src, void* dest )
{
    unsigned long m_old_protect;

    VirtualProtect( src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
    memcpy( src, dest, 5 );

    // adjust short jmp address
    long* jump_to_address = reinterpret_cast<long*>( reinterpret_cast<unsigned long>( src ) + 1 );
    *jump_to_address += reinterpret_cast<unsigned long>( dest );
    *jump_to_address -= reinterpret_cast<unsigned long>( src );

    VirtualProtect( src, 5, PAGE_EXECUTE_READ, &m_old_protect );
}


void hook_guard::stop_hook()
{
    unsigned long m_old_protect;

    VirtualProtect( m_src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
    memcpy( m_src, m_src_instruction_backup, 5 );
    VirtualProtect( m_src, 5, PAGE_EXECUTE_READ, &m_old_protect );
}

