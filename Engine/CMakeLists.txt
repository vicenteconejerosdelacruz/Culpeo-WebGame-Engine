cmake_minimum_required(VERSION 3.22)

# 1. Define el ejecutable de tu motor
add_executable(CulpeoEngine
    "Source/Engine.cpp"
)

# 2. Configura la ruta de salida en /Engine/Target
set_target_properties(CulpeoEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Target"
)

# 3. Directorios de inclusión locales del motor y Assimp
target_include_directories(CulpeoEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/SDKs/assimp/include
    ${CMAKE_SOURCE_DIR}/SDKs/build/assimp/include
)

# 4. Configuración específica para WebAssembly (Emscripten)
if(EMSCRIPTEN)
    #set_target_properties(CulpeoEngine PROPERTIES SUFFIX ".html")

    # Enlazar las librerías automáticamente (Esto inyecta por arte de magia todos los include paths de Dawn)
    target_link_libraries(CulpeoEngine PRIVATE 
        emdawnwebgpu_cpp 
        webgpu_glfw 
        assimp
    )

    target_link_options(CulpeoEngine PRIVATE 
        "-sASYNCIFY=1" 
        "-sUSE_GLFW=3" 
        "-sALLOW_MEMORY_GROWTH=1"
    )
else()
    target_link_libraries(CulpeoEngine PRIVATE 
        webgpu_dawn 
        webgpu_glfw 
        glfw 
        assimp
    )
endif()