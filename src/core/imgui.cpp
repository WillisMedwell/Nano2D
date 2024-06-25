#include "core/imgui.hpp"
#include "core/external_libraries.hpp"

namespace n2d::core {
    auto imgui::create_impl(core::glfw* glfw, void*(*imgui_alloc)(size_t, void*), void(*imgui_dealloc)(void*, void*)) -> result<imgui>
    {
        IMGUI_CHECKVERSION();
        void* imgui_context = reinterpret_cast<void*>(ImGui::CreateContext());
        if(!imgui_context) {
            return result<imgui>(str_literal("Unable to initalise imgui context."));
        }

        //ImGui::SetAllocatorFunctions()
        // ImGuiMemAllocFunc func;
        // //typedef void*   (*ImGuiMemAllocFunc)(size_t sz, void* user_data); 
        // ImGuiMemFreeFunc free; 
        // //typedef void    (*ImGuiMemFreeFunc)(void* ptr, void* user_data); 

        ImGuiIO& io = ImGui::GetIO(); 
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.IniFilename = nullptr;

        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 5.0f;
        style.WindowRounding = 5.0f;

        ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(glfw->get_window()), true);
        #ifdef __EMSCRIPTEN__
            ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
        #endif
        ImGui_ImplOpenGL3_Init(glfw->get_glsl_version());

        return result<imgui>(M{
            .imgui_context = imgui_context
        });
    }

   void imgui::mark_start_of_frame() const
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void imgui::render() const
    {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    imgui::~imgui()
    {
        if(_m.imgui_context) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext(reinterpret_cast<ImGuiContext*>(_m.imgui_context));
        }
        _m = M{};
    }    
}