#pragma once

#include <n2d/helpers.hpp>
#include <memory/allocator.hpp>

#include <memory>
#include <utility>

namespace n2d::core 
{
    struct glfw_config {
        n2d::str_literal window_title = str_literal("n2d app");
        int window_width;
        int window_height;
        bool open_as_fullscreen;
        memory::allocator allocator = memory::default_allocator;
    };

    // Exists to handle GLFW setup and lifetime, not intended to be GLFW wrapper or anything. 
    class glfw {
        struct M {
            void* window = nullptr;
            const char* glsl_version = "#version 000";
        } _m;

        glfw(M&& m) : _m(static_cast<M&&>(m)) {}
        static auto create_impl(const glfw_config& config, void*(alloc)(size_t, void*), void*(realloc)(void* ptr, size_t size, void*), void(dealloc)(void* ptr, void*)) -> result<glfw>;
    
    public:
        template<memory::allocator allocator = memory::default_allocator>
        static auto create(const glfw_config& config) -> result<glfw>
        {
            auto glfw_alloc = [](size_t size, void*) -> void* { return allocator.alloc(size, alignof(max_align_t)); };
            auto glfw_realloc = [](void* ptr, size_t size, void*) -> void* { return allocator.realloc(ptr, size, alignof(max_align_t)); };
            auto glfw_dealloc = [](void* ptr, void*) -> void { allocator.dealloc(ptr, 0); };
            return create_impl(config, glfw_alloc, glfw_realloc, glfw_dealloc);
        }

        glfw(glfw&& other) : _m(std::exchange(other._m, M{})) {}
        ~glfw();

        inline auto get_window() const noexcept { return _m.window; }
        inline auto get_glsl_version() const noexcept { return _m.glsl_version; }

        glfw(const glfw&) = delete;
        glfw& operator=(const glfw&) = delete;
    };
}
