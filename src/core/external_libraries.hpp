#pragma once

#pragma once

#include <glm/glm.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

namespace n2d::core::constants {
    constexpr static bool is_emscripten = 
    #ifdef __EMSCRIPTEN__
        true;
    #else 
        false;
    #endif
}