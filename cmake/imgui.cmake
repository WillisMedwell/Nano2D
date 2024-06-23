cmake_minimum_required(VERSION 3.20)

project(wom_imgui LANGUAGES CXX)

# IMGUI = Immediate Mode UI
set(IMGUI_DOWNLOAD_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/imgui-src)
if(NOT EXISTS ${IMGUI_DOWNLOAD_DIRECTORY})
    message(STATUS "Downloading: ImGui (tagged release = ${IMGUI_DOWNLOAD_VERSION})")

    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui.cpp ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui.cpp TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui.h   ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui.h TIMEOUT 60)
    
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imconfig.h       ${IMGUI_DOWNLOAD_DIRECTORY}/include/imconfig.h TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_internal.h ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_internal.h TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imstb_textedit.h ${IMGUI_DOWNLOAD_DIRECTORY}/include/imstb_textedit.h TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imstb_rectpack.h ${IMGUI_DOWNLOAD_DIRECTORY}/include/imstb_rectpack.h TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imstb_truetype.h ${IMGUI_DOWNLOAD_DIRECTORY}/include/imstb_truetype.h TIMEOUT 60)

    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_demo.cpp ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_demo.cpp TIMEOUT 60)

    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_draw.cpp ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_draw.cpp TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_draw.h   ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_draw.h TIMEOUT 60)
    
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_tables.cpp ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_tables.cpp TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_tables.h   ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_tables.h TIMEOUT 60)
    
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_widgets.cpp ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_widgets.cpp TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/imgui_widgets.h   ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_widgets.h TIMEOUT 60)

    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/backends/imgui_impl_opengl3.cpp ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_impl_opengl3.cpp TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/backends/imgui_impl_opengl3.h   ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_impl_opengl3.h TIMEOUT 60)

    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/backends/imgui_impl_glfw.cpp ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_impl_glfw.cpp TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/backends/imgui_impl_glfw.h   ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_impl_glfw.h TIMEOUT 60)

    file(DOWNLOAD https://raw.githubusercontent.com/ocornut/imgui/${IMGUI_DOWNLOAD_VERSION}/backends/imgui_impl_opengl3_loader.h   ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_impl_opengl3_loader.h TIMEOUT 60)

else()
    message(STATUS "Found: ImGui")
endif()

set(IMGUI_SOURCE_FILES 
    ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui.cpp 
    ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_demo.cpp 
    ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_draw.cpp 
    ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_tables.cpp 
    ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_widgets.cpp
    ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_impl_glfw.cpp
    ${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_impl_opengl3.cpp)
set(IMGUI_HEADER_FILES
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imconfig.h 
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_internal.h
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imstb_textedit.h
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imstb_rectpack.h
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imstb_truetype.h
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui.h 
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_draw.h 
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_tables.h 
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_widgets.h 
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_impl_glfw.h
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_impl_opengl3.h
    ${IMGUI_DOWNLOAD_DIRECTORY}/include/imgui_impl_opengl3_loader.h)
set(IMGUI_INCLUDE_DIR  ${IMGUI_DOWNLOAD_DIRECTORY}/include/)

if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(imgui SHARED)
else()
    add_library(imgui STATIC)
endif()

if(NOT EMSCRIPTEN)
    target_link_libraries(imgui PRIVATE glfw OpenGL::GL)
else()
    target_link_options(imgui PUBLIC -sUSE_WEBGL2=1 -sUSE_GLFW=3 -sFULL_ES3=1 -sALLOW_MEMORY_GROWTH)
endif()
target_sources(imgui PRIVATE ${IMGUI_SOURCE_FILES})

target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_compile_features(imgui PUBLIC cxx_std_17)

set_target_properties(imgui 
    PROPERTIES
        FOLDER "wom_core_dependencies"
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 20
)

set_source_files_properties(${IMGUI_DOWNLOAD_DIRECTORY}/src/imgui_impl_opengl3.cpp PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)