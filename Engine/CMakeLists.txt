cmake_minimum_required(VERSION 3.22)

# 1. Define el ejecutable de tu motor (incluimos el PCH aquí para seguimiento)
add_executable(CulpeoEngine
    "Source/Engine.cpp"
    "Source/JExpose/JObject.h"
    "Source/JExpose/JObject.cpp"
    "Source/Templates/JTemplate.h"
    "Source/Templates/Templates.h"
    "Source/Templates/Templates.cpp"
    "Source/Templates/Material/MeshMaterial.h"
    "Source/Templates/Shader/Shader.h"
    "Source/Templates/Shader/ShaderAtt.h"
    "Source/Templates/Shader/Shader.cpp"
    "Source/pch/UUID.h"
    "Source/pch/NoStd.h"
    "Source/pch/SceneUnitId.h"
)

# 2. Configura la ruta de salida en /Engine/Target
set_target_properties(CulpeoEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Target"
)

# 3. Directorios de inclusión locales del motor y Assimp
target_include_directories(CulpeoEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/JExpose
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Templates
    ${CMAKE_SOURCE_DIR}/SDKs/assimp/include
    ${CMAKE_SOURCE_DIR}/SDKs/build/assimp/include
)

# 4. Configurar Precompiled Header (PCH) global para el motor
target_precompile_headers(CulpeoEngine PRIVATE
    "Source/pch/UUID.h"
    "Source/pch/NoStd.h"
    "Source/pch/SceneUnitId.h"
    "Source/pch/ShaderMaterials.h"
    "Source/pch/RefTracker.h"
    "Source/Templates/TemplateDecl.h"
    "Source/Templates/TemplateDef.h"
    "Source/Utils/Yaml2Json.h"
)

# 5. Configuración específica para WebAssembly (Emscripten)
if(EMSCRIPTEN)
    #set_target_properties(CulpeoEngine PROPERTIES SUFFIX ".html")

    # Enlazar las librerías automáticamente
    target_link_libraries(CulpeoEngine PRIVATE 
        emdawnwebgpu_cpp 
        webgpu_glfw 
        assimp
        yaml-cpp
        nlohmann_json::nlohmann_json
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
        yaml-cpp
        nlohmann_json::nlohmann_json
    )
endif()