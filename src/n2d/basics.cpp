#include "n2d/basics.hpp"

#if defined(_WIN32) || defined(_WIN64)
    #include <malloc.h>
#endif
#include <cstdlib>
#include <cstdio>


#include <stacktrace>

namespace n2d::memory 
{
    void* default_allocator::alloc(size_t size_bytes, size_t alignment)
    {
        //auto trace = *(++std::stacktrace::current().begin());
        //std::printf("allocation at: %s:%u:0 %zu \n", trace.source_file().data(), trace.source_line(), size_bytes);

        #if (defined(_WIN32) || defined(_WIN64)) && !defined(__clang__)
            return _aligned_malloc(size_bytes, alignment);
        #elif (defined(_WIN32) || defined(_WIN64))
            return std::malloc(size_bytes);
        #else
            return std::aligned_alloc(size_bytes, alignment);
        #endif
    }
    void default_allocator::dealloc(void* ptr)
    {
        #if (defined(_WIN32) || defined(_WIN64)) && !defined(__clang__)
            _aligned_free(ptr);
        #elif (defined(_WIN32) || defined(_WIN64))
            return std::free(ptr);
        #else
            std::free(ptr);
        #endif
    }

    void* default_allocator::realloc(void* ptr, size_t size_bytes, size_t alignment [[maybe_unused]])
    {
        //std::printf("re-allocating: %zu \n", size_bytes);
        #if (defined(_WIN32) || defined(_WIN64)) && !defined(__clang__)
            return _aligned_realloc(ptr, size_bytes, alignment);
        #elif (defined(_WIN32) || defined(_WIN64))
            return std::realloc(ptr, size_bytes);
        #else
            return std::realloc(ptr, size_bytes);
        #endif
    }
}