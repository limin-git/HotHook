#ifndef HOT_HOOK_H_INCLUDE
#define HOT_HOOK_H_INCLUDE

#include <map>

class hook_guard
{
public:

    hook_guard() {}

    hook_guard( void* src, void* dest )
    {
        hook_function( src, dest );
    }

    template<typename src_mem_fun_type, typename dest_mem_fun_type> hook_guard( src_mem_fun_type src_mem_fun, dest_mem_fun_type dest_mem_fun )
    {
        hook_member_function( src_mem_fun, dest_mem_fun );
    }

    template<typename src_obj_type, typename dest_mem_fun_type> hook_guard( src_obj_type* src_obj_ptr, unsigned long n, dest_mem_fun_type dest_mem_fun )
    {
        hook_virtual_member_function( src_obj_ptr, n, dest_mem_fun );
    }

    ~hook_guard()
    {
        stop_hook();
    }

public:

    void hook_function( void* src, void* dest )
    {
        start_hook( src, dest );
    }

    template<typename src_mem_fun_type, typename dest_mem_fun_type> void hook_member_function( src_mem_fun_type src_mem_fun, dest_mem_fun_type dest_mem_fun )
    {
        void* src = get_member_function_address( src_mem_fun );
        void* dest = get_member_function_address( dest_mem_fun );
        start_hook( src, dest );
    }

    template<typename src_obj_type, typename dest_mem_fun_type> void hook_virtual_member_function( src_obj_type* src_obj_ptr, unsigned long n, dest_mem_fun_type dest_mem_fun )
    {
        void* src = get_virtual_member_function_address( src_obj_ptr, n );
        void* dest = get_member_function_address( dest_mem_fun );
        start_hook( src, dest );
    }

private:

    void start_hook( void* src, void* dest );
    void stop_hook();

    template<typename mem_fun_type> void* get_member_function_address( mem_fun_type mem_fun_address )
    {
        return (void*)( *( ( long *)&mem_fun_address ) );
    }

    template<typename object_type> void* get_virtual_member_function_address( object_type* obj, unsigned long n )
    {
        unsigned long** vftable = reinterpret_cast<unsigned long**>(obj);
        return (void*)( (*vftable)[n] );
    }

private:

    std::map<void*, char*> m_src_instruction_backup;
};


#endif
