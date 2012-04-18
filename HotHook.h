#ifndef HOT_HOOK_H_INCLUDE
#define HOT_HOOK_H_INCLUDE


class hook_guard
{
public:

    template<typename src_mem_fun_type, typename dest_mem_fun_type>
    hook_guard( src_mem_fun_type src_mem_fun, dest_mem_fun_type dest_mem_fun )
    {
        m_src = get_member_function_address( src_mem_fun );
        void* dest = get_member_function_address( dest_mem_fun );

        memcpy( m_src_instruction_backup, m_src, 5 );
        start_hook( m_src, dest );
    }

    template<void*, void*>
    hook_guard( void* src, void* dest )
    {
        m_src = src;
        memcpy( m_src_instruction_backup, m_src, 5 );
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

private:

    void* m_src;
    char m_src_instruction_backup[5];
};





#endif
