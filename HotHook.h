#ifndef HOT_HOOK_H_INCLUDE
#define HOT_HOOK_H_INCLUDE

#include <map>
#include <boost/preprocessor/cat.hpp>


class hook_guard
{
public:

    hook_guard( void* src, void* dest )
    {
        start_hook( src, dest );
    }

    ~hook_guard()
    {
        stop_hook();
    }

    void start_hook( void* src, void* dest );
    void stop_hook();

private:

    std::map<void*, char*> m_src_instruction_backup;
};


template<typename member_function_type> void* get_member_function_address_vc6( member_function_type member_function )
{
    union
    {
        member_function_type memfun;
        void* addr;
    } ut;

    ut.memfun = member_function;
    return ut.addr;
}

#define get_member_function_address_vc8(member_function_address, member_function_type)              \
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
    void* src_addr;                                                                                 \
    void* dest_addr;                                                                                \
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


