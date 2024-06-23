#pragma once

#include "n2d/helpers.hpp"

#include "core/glfw.hpp"

namespace n2d::core {
    class imgui {
        struct M {
            void* imgui_context = nullptr;
        } _m;

        imgui(M&& m) : _m(std::move(m)) {}
    public:
        static auto create(glfw* glfw) -> result<imgui>;

        imgui(imgui&& other) : _m(std::exchange(other._m, M {})) {}
        ~imgui();

        void mark_start_of_frame() const;
        void render() const;

        imgui(const imgui&) = delete;
        imgui& operator=(const imgui&) = delete;
    };
}