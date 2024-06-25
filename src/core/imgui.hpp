#pragma once

#include "n2d/basics.hpp"
#include "core/glfw.hpp"

namespace n2d::core {
    class imgui {
        struct M {
            void* imgui_context = nullptr;
        } _m;

        friend class result<imgui>;
        imgui(M&& m) : _m(std::move(m)) {}

        static auto create_impl(glfw* glfw, void*(*imgui_alloc)(size_t, void*), void(*imgui_dealloc)(void*, void*)) -> result<imgui>;

    public:
        template<memory::is_static_allocator allocator = memory::default_allocator>
        static auto create(glfw* glfw) -> result<imgui>
        {
            thread_local static auto alloc = [](size_t size_bytes, void*) noexcept -> void* { return allocator::alloc(size_bytes, sizeof(max_align_t)); };
            thread_local static auto dealloc = [](void* ptr, void*) noexcept -> void { allocator::dealloc(ptr); };
            return create_impl(glfw, alloc, dealloc);
        }

        imgui(imgui&& other) : _m(std::exchange(other._m, M {})) {}
        ~imgui();

        void mark_start_of_frame() const;
        void render() const;

        imgui(const imgui&) = delete;
        imgui& operator=(const imgui&) = delete;
    };
}