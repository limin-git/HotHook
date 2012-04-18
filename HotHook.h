#ifndef HOT_HOOK_H_INCLUDE
#define HOT_HOOK_H_INCLUDE


class hook_guard
{
public:

    hook_guard( void* src, void* dest );
    ~hook_guard();

private:

    void start_hook( void* src, void* dest );
    void stop_hook();

private:

    void* m_src;
    char m_src_instruction_backup[5];
};


#endif
