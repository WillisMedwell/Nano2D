#include "n2d/app.hpp"

#include "core/glfw.hpp"
#include "core/imgui.hpp"
#include "memory/allocator.hpp"

namespace n2d {

    struct app::M {
        core::glfw glfw;
        core::imgui imgui;
        memory::allocator allocator;
    };

    auto app::create() -> result<app>
    {
        core::glfw_config config;

        config.window_height = 400;
        config.window_width = 400;

        auto glfw_result = core::glfw::create(config); 
        if(!glfw_result) {
            return result<app>(glfw_result.error());
        }

        auto imgui_result = core::imgui::create(glfw_result.operator->());
        if(!imgui_result) {
            return result<app>(imgui_result.error());
        }

        M* members = (M*)memory::default_allocator.alloc(sizeof(M), alignof(M));
        new (members) M{
            .glfw = glfw_result.pop_value(), 
            .imgui = imgui_result.pop_value(),
            .allocator = memory::default_allocator,
        };
        return result<app>(members);
    }

    app::~app()
    {
        if(_m) {
            auto deallocator = _m->allocator.dealloc;
            _m->~M();
            deallocator(_m, sizeof(M));
        }
        _m = nullptr;
    }

}