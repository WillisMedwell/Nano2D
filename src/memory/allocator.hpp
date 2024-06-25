#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#endif
#include <cstdlib>

#include <concepts>

namespace n2d::memory {
    struct allocator {
        void* (*alloc)(size_t size_bytes, size_t alignment);
        void* (*realloc)(void* ptr, size_t size_bytes, size_t alignment);
        void  (*dealloc)(void* ptr, size_t size_bytes);
    };
}
