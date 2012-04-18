#include "stdafx.h"
#include "HotHook.h"


void hook_guard::start_hook( void* src, void* dest )
{
    unsigned long m_old_protect;

    VirtualProtect( src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );

    m_src_instruction_backup[src] = new char[5];
    memcpy(m_src_instruction_backup[src], src, 5);
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

    for ( std::map<void*, char*>::iterator it = m_src_instruction_backup.begin(); it != m_src_instruction_backup.end(); ++it )
    {
        VirtualProtect( it->first, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
        memcpy( it->first, it->second, 5 );
        VirtualProtect( it->first, 5, PAGE_EXECUTE_READ, &m_old_protect );
        delete it->second;
    }
}

