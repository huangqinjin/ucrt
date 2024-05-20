//
// MIT License
// 
// Copyright (c) 2024 Huang Qinjin (huangqinjin@gmail.com)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once

namespace __crt_state_management
{
#ifdef _CRT_GLOBAL_STATE_ISOLATION
    static const size_t state_index_count = 2;
#else
    static const size_t state_index_count = 1;
#endif

#ifdef _CRT_GLOBAL_STATE_ISOLATION
    extern "C" extern DWORD __crt_global_state_mode_flsindex;

    inline bool initialize_global_state_isolation()
    {
        __crt_global_state_mode_flsindex = __acrt_FlsAlloc(nullptr);
        return __crt_global_state_mode_flsindex != FLS_OUT_OF_INDEXES;
    }

    inline void uninitialize_global_state_isolation(const bool)
    {
        if (__crt_global_state_mode_flsindex != FLS_OUT_OF_INDEXES)
        {
            __acrt_FlsFree(__crt_global_state_mode_flsindex);
            __crt_global_state_mode_flsindex = FLS_OUT_OF_INDEXES;
        }
    }
#endif

    inline size_t get_current_state_index(__crt_scoped_get_last_error_reset const&)
    {
#ifdef _CRT_GLOBAL_STATE_ISOLATION
        return (size_t)(ULONG_PTR)__acrt_FlsGetValue(__crt_global_state_mode_flsindex);
#else
        return 0;
#endif
    }

    inline size_t get_current_state_index()
    {
        __crt_scoped_get_last_error_reset const last_error_reset;
        return get_current_state_index(last_error_reset);
    }

#ifdef _CRT_GLOBAL_STATE_ISOLATION
    void enter_os_call();
    void leave_os_call();

    inline bool is_os_call()
    {
        return get_current_state_index() == 1;
    }

    template<typename F, typename... Args>
    auto wrapped_invoke(F f, Args... args)
    {
        struct os_guard
        {
            os_guard() { enter_os_call(); }
            ~os_guard() { leave_os_call(); }
        } _;
        return f(args...);
    }
#endif

    class scoped_global_state_reset
    {
#ifdef _CRT_GLOBAL_STATE_ISOLATION
    public:
        scoped_global_state_reset() : _was_os_call(is_os_call())
        {
            if (_was_os_call) leave_os_call();
        }

        ~scoped_global_state_reset()
        {
            if (_was_os_call) enter_os_call();
        }

    private:
    	bool _was_os_call;
#endif
    };

    template <typename T>
    class dual_state_global
    {
    public:
        void initialize(T const new_value)
        {
            for (size_t i = 0; i < state_index_count; ++i)
                _value[i] = new_value;
        }

        void uninitialize(void(*f)(T&))
        {
            for (size_t i = 0; i < state_index_count; ++i)
                f(_value[i]);
        }

        template<typename U>
        void initialize_from_array(U& data_array)
        {
            T* global_array = dangerous_get_state_array();
            for (size_t i = 0; i < state_index_count; ++i)
                global_array[i] = data_array[i];
        }

        T& value_explicit(size_t index) throw() { return _value[index]; }
        T const& value_explicit(size_t index) const throw() { return _value[index]; }

        T& value() throw() { return _value[get_current_state_index()]; }
        T const& value() const throw() { return _value[get_current_state_index()]; }

        T& value(__crt_cached_ptd_host& ptd) throw();
        T const& value(__crt_cached_ptd_host& ptd) const throw();

        T* dangerous_get_state_array() { return _value; }

    private:
        T _value[state_index_count];
    };
}

extern "C" inline __acrt_lock_id __cdecl __acrt_select_exit_lock()
{
#ifdef _CRT_GLOBAL_STATE_ISOLATION
    if (__crt_state_management::is_os_call())
        return __acrt_os_exit_lock;
#endif
    return __acrt_exit_lock;
}
