#include "stdafx.h"
#include "HotHook.h"


std::map<void*, long> hook_guard::m_src_address_backup_all;


hook_guard::hook_guard( void* src, void* dest )
{
    start_hook( src, dest );
    m_src_address_backup.insert( src );
}


hook_guard::~hook_guard()
{
    for ( std::set<void*>::iterator it = m_src_address_backup.begin(); it != m_src_address_backup.end(); ++it )
    {
        stop_hook( *it );
    }
}


void hook_guard::start_hook( void* src, void* dest )
{
    // adjust short jmp address
    long* jump_to_address_ptr = reinterpret_cast<long*>( reinterpret_cast<unsigned long>( src ) + 1 );
    m_src_address_backup_all[src] = *jump_to_address_ptr;
    long jump_to_address = *( reinterpret_cast<long*>( reinterpret_cast<unsigned long>( dest ) + 1 ) ) + reinterpret_cast<unsigned long>( dest ) - reinterpret_cast<unsigned long>( src );

    unsigned long m_old_protect;
    VirtualProtect( src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
    *jump_to_address_ptr = jump_to_address;
    VirtualProtect( src, 5, PAGE_EXECUTE_READ, &m_old_protect );
}


void hook_guard::stop_hook( void* src )
{
    std::map<void*, long>::iterator find_it = m_src_address_backup_all.find( src );

    if ( find_it != m_src_address_backup_all.end() )
    {
        long* jump_to_address_ptr = reinterpret_cast<long*>( reinterpret_cast<unsigned long>( src ) + 1 );

        unsigned long m_old_protect;
        VirtualProtect( src, 5, PAGE_EXECUTE_WRITECOPY, &m_old_protect );
        *jump_to_address_ptr = find_it->second;
        VirtualProtect( src, 5, PAGE_EXECUTE_READ, &m_old_protect );

        m_src_address_backup_all.erase( find_it );
    }
}

