#ifndef HOT_HOOK_H_INCLUDE
#define HOT_HOOK_H_INCLUDE


class hook_guard
{
public:

    // member function
    template<typename src_mem_fun_type, typename dest_mem_fun_type> hook_guard( src_mem_fun_type src_mem_fun, dest_mem_fun_type dest_mem_fun )
    {
        m_src = get_member_function_address( src_mem_fun );
        void* dest = get_member_function_address( dest_mem_fun );
        start_hook( m_src, dest );
    }

    // function
    template<void*, void*> hook_guard( void* src, void* dest )
    {
        m_src = src;
        start_hook( m_src, dest );
    }

    // virtual member function
    template<typename src_t, typename dest_t> hook_guard( src_t* src_obj, dest_t* dest_obj, unsigned long n )
    {
        m_src = get_virtual_member_function_address( src_obj, n );
        void* dest = get_virtual_member_function_address( dest_obj, n );
        start_hook( m_src, dest );
    }

    ~hook_guard()
    {
        stop_hook();
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
        unsigned long** vtable = reinterpret_cast<unsigned long**>(obj);
        return (void*)( (*vtable)[n] );
    }

private:

    void* m_src;
    char m_src_instruction_backup[5];
};


#endif
