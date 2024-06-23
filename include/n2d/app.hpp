#pragma once

// #include "core/glfw.hpp"
// #include "core/imgui.hpp"

#include "n2d/helpers.hpp"

namespace n2d {
    class app
    {
        struct M;
        M* _m;

        app() = delete;
        app(M* m) : _m(m) {}

    public:
        static auto create() -> result<app>;
        app(app&& app) : _m(app._m) { app._m = nullptr; }

        ~app();
        
        app(const app&) = delete;
        app& operator=(const app&) = delete;
    };
}