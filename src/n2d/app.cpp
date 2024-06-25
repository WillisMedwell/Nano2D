#include "n2d/app.hpp"

#include "core/glfw.hpp"
#include "core/imgui.hpp"
#include "core/external_libraries.hpp"
#include "memory/allocator.hpp"

#include <chrono>

namespace n2d {

    struct app::M {
        using unique_activity_ptr = std::unique_ptr<n2d::activity, memory::default_deleter>;
        unique_activity_ptr activity;
        core::glfw glfw;
        core::imgui imgui;
        std::chrono::time_point<std::chrono::high_resolution_clock> prev_frame_time;
    };

    auto app::create_impl(n2d::activity* activity) -> result<app>
    {
        core::glfw_config config;

        config.window_height = 400;
        config.window_width = 400;

        auto glfw_result = core::glfw::create<memory::default_allocator>(config); 
        if(!glfw_result) {
            return result<app>(glfw_result.error());
        }

        auto imgui_result = core::imgui::create<memory::default_allocator>(glfw_result.operator->());
        if(!imgui_result) {
            return result<app>(imgui_result.error());
        }

        M* members = (M*)memory::default_allocator::alloc(sizeof(M), alignof(M));
        new (members) M{
            .activity = M::unique_activity_ptr(activity),
            .glfw = glfw_result.pop_value(), 
            .imgui = imgui_result.pop_value(),
            .prev_frame_time = std::chrono::high_resolution_clock::now(),
        };
        return result<app>(members);
    }

    app::~app()
    {
        if(_m) {
            _m->~M();
            memory::default_allocator::dealloc(_m);
        }
        _m = nullptr;
    }

    bool app::is_running()
    {
        bool is_running = true;
        is_running &= !glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(_m->glfw.get_window()));
        return is_running;
    }
    void app::update()
    {
        glfwPollEvents();   
        _m->imgui.mark_start_of_frame();

    }
    void app::render()
    {
        int window_width, window_height;
        glfwGetFramebufferSize(reinterpret_cast<GLFWwindow*>(_m->glfw.get_window()), &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        _m->activity->render();

        _m->imgui.render();

        glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(_m->glfw.get_window()));
    }



    namespace details {
        #ifdef __EMSCRIPTEN__
            static std::optional<app> app_instance;
            static void app_emscripten_main_loop() { 
                app->update();
                app->render();
            }
            void app_runner(wom::app&& app)
            {
                app_instance.emplace(std::move(app));
                emscripten_set_main_loop(app_emscripten_main_loop, 0, true);
            }
        #else 
            void app_runner(app&& app)
            {
                while(app.is_running()) {
                    app.update();
                    app.render();
                }
            }
        #endif

    }

    void run_app(n2d::app&& app)
    {
        details::app_runner(std::move(app));
    }
}