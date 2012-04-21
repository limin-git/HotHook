#ifndef HOT_HOOK_H_INCLUDE
#define HOT_HOOK_H_INCLUDE

#include <map>
#include <set>
#include <boost/preprocessor/cat.hpp>


class hook_guard
{
public:

    hook_guard( void* src, void* dest );
    ~hook_guard();

    static void start_hook( void* src, void* dest );
    static void stop_hook( void* src );

private:

    std::set<void*> m_src_address_backup;
    static std::map<void*, long> m_src_address_backup_all;
};


#define get_member_function_address_vc8(member_function_address, member_function_type)              \
void* member_function_address;                                                                      \
{                                                                                                   \
    __asm                                                                                           \
    {                                                                                               \
        mov eax,offset member_function_type                                                         \
    };                                                                                              \
    __asm                                                                                           \
    {                                                                                               \
        mov member_function_address, eax                                                            \
    };                                                                                              \
}

#define hook_member_function_guard_impl(guard, src_addr, dest_addr, src_mem_fun, dest_mem_fun)      \
    get_member_function_address_vc8( src_addr, src_mem_fun );                                       \
    get_member_function_address_vc8( dest_addr, dest_mem_fun );                                     \
    hook_guard guard(src_addr, dest_addr )

#define hook_member_function_guard( src_mem_fun, dest_mem_fun )                                     \
    hook_member_function_guard_impl( BOOST_PP_CAT(___hook_member_function_guard___, __LINE__),      \
                                     BOOST_PP_CAT(___hook_member_function_src_addr___, __LINE__),   \
                                     BOOST_PP_CAT(___hook_member_function_dest_addr___, __LINE__),  \
                                     src_mem_fun,                                                   \
                                     dest_mem_fun )


#endif


