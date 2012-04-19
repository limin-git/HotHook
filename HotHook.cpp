#include "stdafx.h"
#include "HotHook.h"


void hook_guard::start_hook( void* src, void* dest )
{
    unsigned long m_old_protect;

    // adjust short jmp address
    long* jump_to_address_ptr = reinterpret_cast<long*>( reinterpret_cast<unsigned long>( src ) + 1 );
    m_src_address_backup[src] = *jump_to_address_ptr;
    long jump_to_address = *( reinterpret_cast<long*>( reinterpret_cast<unsigned long>( dest ) + 1 ) ) + reinterpret_cast<unsigned long>( dest ) - reinterpret_cast<unsigned long>( src );

    VirtualProtect( src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
    *jump_to_address_ptr = jump_to_address;
    VirtualProtect( src, 5, PAGE_EXECUTE_READ, &m_old_protect );
}


void hook_guard::stop_hook()
{
    unsigned long m_old_protect;

    for ( std::map<void*, long>::iterator it = m_src_address_backup.begin(); it != m_src_address_backup.end(); ++it )
    {
        long* jump_to_address_ptr = reinterpret_cast<long*>( reinterpret_cast<unsigned long>( it->first ) + 1 );

        VirtualProtect( it->first, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
        *jump_to_address_ptr = it->second;
        VirtualProtect( it->first, 5, PAGE_EXECUTE_READ, &m_old_protect );
    }
}

