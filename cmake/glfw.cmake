# Emscripten has in-built linakage to GLFW so we just need the headers
if(DEFINED EMSCRIPTEN)
    # TODO: should probably make this an interface library
    set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/glfw-${GLFW_DOWNLOAD_VERSION}-src/include)
    if(NOT EXISTS ${GLFW_INCLUDE_DIR})
        message(STATUS "GLFW: Downloading v${GLFW_DOWNLOAD_VERSION}")
        make_directory(${GLFW_INCLUDE_DIR}/GLFW)
        file(DOWNLOAD https://raw.githubusercontent.com/glfw/glfw/${GLFW_DOWNLOAD_VERSION}/include/GLFW/glfw3.h       ${GLFW_INCLUDE_DIR}/GLFW/glfw3.h      )
        file(DOWNLOAD https://raw.githubusercontent.com/glfw/glfw/${GLFW_DOWNLOAD_VERSION}/include/GLFW/glfw3native.h ${GLFW_INCLUDE_DIR}/GLFW/glfw3native.h)
    endif()
else()
    set(GLFW_QUIET ON)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BULID_DOCS OFF)
    set(GLFW_INSTALL ON)
    
    CPMAddPackage("gh:glfw/glfw#${GLFW_DOWNLOAD_VERSION}")
    set_target_properties(glfw 
        PROPERTIES
            FOLDER "wom_core_dependencies"
            UNITY_BUILD ON
            UNITY_BUILD_BATCH_SIZE 4
    )
    set_source_files_properties(${GLFW_SOURCE_DIR}/src/win32_window.c PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)
    set_source_files_properties(${GLFW_SOURCE_DIR}/src/null_window.c PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)

    message(STATUS ${GLFW_SOURCE_DIR}/src/null_window.c)
endif()
