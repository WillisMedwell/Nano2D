#include "core/glfw.hpp"
#include "memory/allocator.hpp"

#include "core/external_libraries.hpp"

namespace n2d::core {
    namespace details {
        static int glfw_error_code = 0;
        static char glfw_error_string[1000] = { '\0' }; 

        static void glfw_error_callback(int error, const char* description)
        {
            memset(glfw_error_string, 0, sizeof(glfw_error_string));
            #ifdef __EMSCRIPTEN__
                memcpy(glfw_error_string, description, std::min<size_t>(sizeof(glfw_error_string) - 1, strlen(description)));
            #else 
                strcpy_s(glfw_error_string, sizeof(glfw_error_string), description);
            #endif
            glfw_error_code = error;
        }

        static auto glfw_set_window_hints_and_get_glsl_version() -> const char*
        {
            #if defined(IMGUI_IMPL_OPENGL_ES2)
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                return "#version 100";
            #elif defined(__APPLE__)
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                return glsl_version = "#version 150";
            #else
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                return "#version 130";
            #endif
        }
    
    }

    auto glfw::create_impl(const glfw_config& config, void*(alloc)(size_t, void*), void*(realloc)(void* ptr, size_t size, void*), void(dealloc)(void* ptr, void*)) -> result<glfw>
    {
        glfwSetErrorCallback(details::glfw_error_callback);
        
        GLFWallocator glfw_allocator = {
            .allocate = alloc,
            .reallocate = realloc,
            .deallocate = dealloc,
            .user = nullptr
        };

        glfwInitAllocator(&glfw_allocator);

        if (!glfwInit()) {
            glfwSetErrorCallback(nullptr);
            glfwTerminate();

            if (!details::glfw_error_code) {
                return result<glfw>(str_literal("GLFW failed to initialise."));
            }
            return result<glfw>(str_literal(details::glfw_error_string));
        }

        const char* glsl_version = details::glfw_set_window_hints_and_get_glsl_version();

        GLFWwindow* window = [&]() {
            if(!constants::is_emscripten && config.open_as_fullscreen) {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                auto video_mode = glfwGetVideoMode(monitor);
                return glfwCreateWindow(video_mode->width, video_mode->height, *config.window_title, monitor, nullptr);
            }
            else {
                return glfwCreateWindow(config.window_width, config.window_height, *config.window_title, nullptr, nullptr);
            }
        }();

        // Handle window creation errors
        if (window == nullptr) {
            glfwTerminate();
            if (!details::glfw_error_code) {
                return result<glfw>(str_literal("GLFW failed to create a window."));
            }
            return result<glfw>(str_literal(details::glfw_error_string));
        }
        glfwMakeContextCurrent(window);

        if constexpr(!constants::is_emscripten) {
            glfwSwapInterval(1);
        }

        return result<glfw>(M{.window = window, .glsl_version = glsl_version});
    }

    glfw::~glfw()
    {
        if(_m.window)
        {
            glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(_m.window));
            glfwTerminate();
        }
        _m = M{};
    }
}