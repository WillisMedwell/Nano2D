#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#endif
#include <cstdlib>

namespace n2d::memory {
    struct allocator {
        void* (*alloc)(size_t size_bytes, size_t alignment);
        void* (*realloc)(void* ptr, size_t size_bytes, size_t alignment);
        void  (*dealloc)(void* ptr, size_t size_bytes);
    };

    namespace details {
        inline auto default_alloc(size_t size_bytes, size_t alignment) -> void*;
        inline auto default_dealloc(void* ptr, size_t) -> void;
        inline auto default_realloc(void* ptr, size_t size_bytes, size_t alignment) -> void*;
    }

    constexpr inline auto default_allocator = [] {
        return allocator {
            details::default_alloc,
            details::default_realloc,
            details::default_dealloc,
        };
    }();

    namespace details {
        inline auto default_alloc(size_t size_bytes, size_t alignment) -> void*
        {
            #if defined(_WIN32) || defined(_WIN64)
                return _aligned_malloc(size_bytes, alignment);
            #else
                return std::aligned_alloc(size_bytes, alignment);
            #endif
        }
        inline auto default_dealloc(void* ptr, size_t) -> void
        {
            #if defined(_WIN32) || defined(_WIN64)
                _aligned_free(ptr);
            #else
                std::free(ptr);
            #endif
        }
        inline auto default_realloc(void* ptr, size_t size_bytes, size_t alignment [[maybe_unused]]) -> void*
        {
            #if defined(_WIN32) || defined(_WIN64)
                return _aligned_realloc(ptr, size_bytes, alignment);
            #else
                return std::realloc(ptr, size_bytes);
            #endif
        }
    }
}
