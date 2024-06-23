include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)

message(STATUS "Nano2D getting/finding dependencies")

set(GLAZE_DOWNLOAD_VERSION  "v2.5.2")
set(GLM_DOWNLOAD_VERSION    "1.0.1")
set(GLFW_DOWNLOAD_VERSION   "3.4")
set(GLEW_DOWNLOAD_VERSION   "2.1.0")
set(LODEPNG_DOWNLOAD_COMMIT ed3c5f14b136c936d615ee3b38aaa7e7e642f12c)
set(SOKOL_DOWNLOAD_COMMIT   090d544c5fbfb4ef4f690534950429c89b00fb6f)
set(IMGUI_DOWNLOAD_VERSION  "v1.90.6-docking")

# Glaze = Json library
# CPMAddPackage("gh:stephenberry/glaze#${GLAZE_DOWNLOAD_VERSION}")

# GLM = Math library
set(GLM_QUIET ON)
set(GLM_BUILD_LIBRARY OFF)
set(GLM_ENABLE_CXX_20 ON)
set(GLM_ENABLE_SIMD_SSE2 ON)
CPMAddPackage("gh:g-truc/glm#${GLM_DOWNLOAD_VERSION}")

# GLFW3 = Windowing, input, and event handling libarary
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/glfw.cmake)

# LODEPNG = Png decoder
set(LODEPNG_DOWNLOAD_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/lodepng-src)
if(NOT EXISTS ${LODEPNG_DOWNLOAD_DIRECTORY})
    message(STATUS "Downloading: LodePNG (commit hash = ${LODEPNG_DOWNLOAD_COMMIT})")
    file(DOWNLOAD https://raw.githubusercontent.com/lvandeve/lodepng/${LODEPNG_DOWNLOAD_COMMIT}/lodepng.cpp ${LODEPNG_DOWNLOAD_DIRECTORY}/src/lodepng.cpp   TIMEOUT 60)
    file(DOWNLOAD https://raw.githubusercontent.com/lvandeve/lodepng/${LODEPNG_DOWNLOAD_COMMIT}/lodepng.h   ${LODEPNG_DOWNLOAD_DIRECTORY}/include/lodepng.h TIMEOUT 60)
else()
    message(STATUS "Found: LodePNG")
endif()
set(LODEPNG_HEADER_FILES ${LODEPNG_DOWNLOAD_DIRECTORY}/include/lodepng.h)
set(LODEPNG_SOURCE_FILES ${LODEPNG_DOWNLOAD_DIRECTORY}/src/lodepng.cpp)
set(LODEPNG_INCLUDE_DIR ${LODEPNG_DOWNLOAD_DIRECTORY}/include/)

# SOKOL = Fetching for files
set(SOKOL_DOWNLOAD_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/sokol-src)
if(NOT EXISTS ${SOKOL_DOWNLOAD_DIRECTORY})
    message(STATUS "Downloading: Sokol (commit hash = ${SOKOL_DOWNLOAD_COMMIT})")
    file(DOWNLOAD https://raw.githubusercontent.com/floooh/sokol/${SOKOL_DOWNLOAD_COMMIT}/sokol_fetch.h   ${SOKOL_DOWNLOAD_DIRECTORY}/include/sokol_fetch.h TIMEOUT 60)
else()
    message(STATUS "Found: Sokol")
endif()
set(SOKOL_HEADER_FILES ${SOKOL_DOWNLOAD_DIRECTORY}/include/sokol_fetch.h)
set(SOKOL_INCLUDE_DIR  ${SOKOL_DOWNLOAD_DIRECTORY}/include/)

# IMGUI = Immediate Mode UI
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/imgui.cmake)

# OPENGL = Graphics Api
find_package(OpenGL REQUIRED)
