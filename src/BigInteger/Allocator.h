#ifndef ALLOCATOR_H
#define ALLOCATOR_H

//这只是找到的alloc源码

namespace Math {
    //----------------------------------------------
    // 第1級配置器。
    //----------------------------------------------
    template <int inst>
    class __malloc_alloc_template {
    private:
        static void* oom_malloc(const size_t&);
        static void* oom_realloc(void*, size_t);
        static void (*__malloc_alloc_oom_handler)();

    public:
        static void* allocate(const size_t& n) {
            return malloc(n);
        }
        static void deallocate(void* p, const size_t& /* n */) {
            free(p);                    //直接使用 free()
        }
        static void* reallocate(void* p, const size_t& /* old_sz */, const size_t& new_sz) {
            return realloc(p, new_sz);
        }
        static void (*set_malloc_handler(void (*f)()))()
        { //類似 C++ 的 set_new_handler().
            void (*old)() = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return(old);
        }
    };
    //----------------------------------------------
    template <int inst>
    void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

    template <int inst>
    void* __malloc_alloc_template<inst>::oom_malloc(const size_t& n) {
        void (*my_malloc_handler)();
        void* result;

        for (;;) {    //不斷嘗試釋放、配置、再釋放、再配置…
            my_malloc_handler = __malloc_alloc_oom_handler;
            (*my_malloc_handler)();    //呼叫處理常式，企圖釋放記憶體
            result = malloc(n);        //再次嘗試配置記憶體
            if (result) return(result);
        }
    }

    template <int inst>
    void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
        void (*my_malloc_handler)();
        void* result;

        for (;;) {    //不斷嘗試釋放、配置、再釋放、再配置…
            my_malloc_handler = __malloc_alloc_oom_handler;
            (*my_malloc_handler)();    //呼叫處理常式，企圖釋放記憶體。
            result = realloc(p, n);    //再次嘗試配置記憶體。
            if (result) return(result);
        }
    }
    //----------------------------------------------

    typedef __malloc_alloc_template<0>  malloc_alloc;

    //----------------------------------------------
    //第二級配置器
    //----------------------------------------------
    enum { __ALIGN = 8 };                        //小區塊的上調邊界
    enum { __MAX_BYTES = 256 };                  //小區塊的上限
    enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; //free-lists 個數

    //本例中兩個 template 參數完全沒有派上用場
    template <bool threads, int inst>
    class __default_alloc_template {
    private:
        //實際上應使用 static const int x = N
        //取代 enum { x = N }, 但目前支援該性質的編譯器不多

        static size_t ROUND_UP(const size_t& bytes) {
            return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
        }

    private:
        union obj {
            union obj* free_list_link;
        };

    private:
        static obj* volatile free_list[__NFREELISTS];
        static size_t FREELIST_INDEX(const size_t& bytes) {
            return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
        }

        // Returns an object of size n, and optionally adds to size n free list.
        static void* refill(const size_t& n);

        // Allocates a chunk for nobjs of size "size".  nobjs may be reduced
        // if it is inconvenient to allocate the requested number.
        static char* chunk_alloc(const size_t& size, int& nobjs);

        // Chunk allocation state.
        static char* start_free;
        static char* end_free;
        static size_t heap_size;

    public:

        static void* allocate(const size_t& n)  //n must be > 0
        {
            obj* volatile* my_free_list;    //obj** my_free_list;
            obj* result;

            if (n > (size_t)__MAX_BYTES) {
                return(malloc_alloc::allocate(n));
            }

            my_free_list = free_list + FREELIST_INDEX(n);
            result = *my_free_list;
            if (result == 0) {
                return refill(ROUND_UP(n));
            }

            *my_free_list = result->free_list_link;
            return (result);
        }

        static void deallocate(void* p, const size_t& n)  //p may not be 0
        {
            obj* q = (obj*)p;
            obj* volatile* my_free_list;   //obj** my_free_list;

            if (n > (size_t)__MAX_BYTES) {
                malloc_alloc::deallocate(p, n);
                return;
            }
            my_free_list = free_list + FREELIST_INDEX(n);
            q->free_list_link = *my_free_list;
            *my_free_list = q;
        }


    };
    //----------------------------------------------
    // We allocate memory in large chunks in order to
    // avoid fragmentingthe malloc heap too much.
    // We assume that size is properly aligned.
    // We hold the allocation lock.
    //----------------------------------------------
    template <bool threads, int inst>
    char*
        __default_alloc_template<threads, inst>::
        chunk_alloc(const size_t& size, int& nobjs) {
        char* result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;

        if (bytes_left >= total_bytes) {
            result = start_free;
            start_free += total_bytes;
            return(result);
        }
        else if (bytes_left >= size) {
            nobjs = bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return(result);
        }
        else {
            size_t bytes_to_get =
                2 * total_bytes + ROUND_UP(heap_size >> 4);
            // Try to make use of the left-over piece.
            if (bytes_left > 0) {
                obj* volatile* my_free_list =
                    free_list + FREELIST_INDEX(bytes_left);

                ((obj*)start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj*)start_free;
            }
            start_free = (char*)malloc(bytes_to_get);
            if (0 == start_free) {
                int i;
                obj* volatile* my_free_list, * p;

                //Try to make do with what we have. That can't
                //hurt. We do not try smaller requests, since that tends
                //to result in disaster on multi-process machines.
                for (i = size; i <= __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (0 != p) {
                        *my_free_list = p->free_list_link;
                        start_free = (char*)p;
                        end_free = start_free + i;
                        return(chunk_alloc(size, nobjs));
                        //Any leftover piece will eventually make it to the
                        //right free list.
                    }
                }
                end_free = 0;       //In case of exception.
                start_free = (char*)malloc_alloc::allocate(bytes_to_get);
                //This should either throw an exception or
                //remedy the situation. Thus we assume it
                //succeeded.
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return(chunk_alloc(size, nobjs));
        }
    }
    //----------------------------------------------
    // Returns an object of size n, and optionally adds
    // to size n free list.We assume that n is properly aligned.
    // We hold the allocation lock.
    //----------------------------------------------
    template <bool threads, int inst>
    void* __default_alloc_template<threads, inst>::
        refill(const size_t& n) {
        int nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);
        obj* volatile* my_free_list;   //obj** my_free_list;
        obj* result;
        obj* current_obj;
        obj* next_obj;
        int i;

        if (1 == nobjs) return(chunk);
        my_free_list = free_list + FREELIST_INDEX(n);

        //Build free list in chunk
        result = (obj*)chunk;
        *my_free_list = next_obj = (obj*)(chunk + n);
        for (i = 1; ; ++i) {
            current_obj = next_obj;
            next_obj = (obj*)((char*)next_obj + n);
            if (nobjs - 1 == i) {
                current_obj->free_list_link = 0;
                break;
            }
            else {
                current_obj->free_list_link = next_obj;
            }
        }
        return(result);
    }
    //----------------------------------------------
    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::start_free = 0;

    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::end_free = 0;

    template <bool threads, int inst>
    size_t __default_alloc_template<threads, inst>::heap_size = 0;

    template <bool threads, int inst>
    typename __default_alloc_template<threads, inst>::obj* volatile
        __default_alloc_template<threads, inst>::free_list[__NFREELISTS]
        = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
    //----------------------------------------------
    //令第2級配置器的名稱為 alloc
    typedef __default_alloc_template<false, 0> alloc;

    template<typename _Ty>
    class Allocator {
    public:
        typedef _Ty            value_type;
        typedef _Ty* pointer;
        typedef const _Ty* const_pointer;
        typedef _Ty& reference;
        typedef const _Ty& const_reference;
        typedef size_t        size_type;
        typedef ptrdiff_t    difference_type;
        static const size_t& getSize() {
            static const size_t TySize = sizeof(_Ty);
            return TySize;
        }
    public:
        static _Ty* allocate() {
            return static_cast<_Ty*>(alloc::allocate(getSize()));
        }
        static _Ty* allocate(const size_t& n) {
            if (n == 0) return 0;
            return static_cast<_Ty*>(alloc::allocate(getSize() * n));
        }
        static void deallocate(_Ty* ptr) {
            alloc::deallocate(static_cast<void*>(ptr), getSize());
        }
        static void deallocate(_Ty* ptr, const size_t& n) {
            if (n == 0) return;
            alloc::deallocate(static_cast<void*>(ptr), getSize() * n);
        }

        static void construct(_Ty* ptr) {
            new(ptr)_Ty();
        }
        static void construct(_Ty* ptr, const _Ty& value) {
            new(ptr)_Ty(value);
        }
        static void destroy(_Ty* ptr) {
            ptr->~_Ty();
        }
        static void destroy(_Ty* first, _Ty* last) {
            for (; first != last; ++first) {
                first->~_Ty();
            }
        }
    };
}

#endif ALLOCATOR_H