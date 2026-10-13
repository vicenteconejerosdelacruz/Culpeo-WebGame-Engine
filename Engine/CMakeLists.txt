cmake_minimum_required(VERSION 3.22)

# 1. Define el ejecutable de tu motor (incluimos el PCH aquí para seguimiento)
add_executable(CulpeoEngine
    "Source/Engine.cpp"
    "Source/Editor/DefaultLevel.h"
    "Source/Editor/DefaultLevel.cpp"
    "Source/Controllers/Controller.h"
    "Source/Controllers/Controller.cpp"
    "Source/Game/Game.h"
    "Source/Game/Game.cpp"
    "Source/JExpose/JObject.h"
    "Source/JExpose/JObject.cpp"
    "Source/Primitives/BoxLines.h"
    "Source/Primitives/BoxLines.cpp"
    "Source/Primitives/Cube.h"
    "Source/Primitives/Cube.cpp"
    "Source/Primitives/Decal.h"
    "Source/Primitives/Decal.cpp"
    "Source/Primitives/Floor.h"
    "Source/Primitives/Floor.cpp"
    "Source/Primitives/Sphere.h"
    "Source/Primitives/Sphere.cpp"
    "Source/Primitives/Cone.h"
    "Source/Primitives/Cone.cpp"
    "Source/Primitives/Capsule.h"
    "Source/Primitives/Capsule.cpp"
    "Source/Primitives/Pentahedron.h"
    "Source/Primitives/Pentahedron.cpp"
    "Source/Primitives/UtahTeapot.h"
    "Source/Primitives/UtahTeapot.cpp"
    "Source/Renderer/DeviceUtils/WebGpuDevice/Builder.h"
    "Source/Renderer/DeviceUtils/WebGpuDevice/Builder.cpp"
    "Source/Renderer/DeviceUtils/IndexBuffer/IndexBuffer.h"
    "Source/Renderer/DeviceUtils/IndexBuffer/IndexBuffer.cpp"
    "Source/Renderer/DeviceUtils/VertexBuffer/VertexBuffer.h"
    "Source/Renderer/DeviceUtils/VertexBuffer/VertexBuffer.cpp"
    "Source/Renderer/Renderer.h"
    "Source/Renderer/Renderer.cpp"
    "Source/Renderer/VertexFormats.h"
    "Source/Renderer/VertexFormats.cpp"
    "Source/Scene/Level.h"
    "Source/Scene/Level.cpp"
    "Source/Scene/Scene.h"
    "Source/Scene/Scene.cpp"
    "Source/Scene/SceneUnit.h"
    "Source/Scene/SceneUnit.cpp"
    "Source/Scene/SceneObject.h"
    "Source/Scene/SceneObject.cpp"
    "Source/Scene/Camera/Camera.h"
    "Source/Scene/Camera/CameraAtt.h"
    "Source/Scene/Camera/Camera.cpp"
    "Source/Scene/Camera/Projections/Orthographic.h"
    "Source/Scene/Camera/Projections/Perspective.h"
    "Source/Scene/Renderable/Renderable.h"
    "Source/Scene/Renderable/Renderable.cpp"
    "Source/Templates/JTemplate.h"
    "Source/Templates/Templates.h"
    "Source/Templates/Templates.cpp"
    "Source/Templates/HtmlUI/HtmlUI.h"
    "Source/Templates/HtmlUI/HtmlUIAtt.h"
    "Source/Templates/HtmlUI/HtmlUI.cpp"
    "Source/Templates/Material/Material.h"
    "Source/Templates/Material/MaterialAtt.h"
    "Source/Templates/Material/Material.cpp"
    "Source/Templates/Material/MeshMaterial.h"
    "Source/Templates/Material/Variables.h"
    "Source/Templates/Material/Variables.cpp"
    "Source/Templates/Mesh/Mesh.h"
    "Source/Templates/Mesh/Mesh.cpp"
    "Source/Templates/Mold/Mold.h"
    "Source/Templates/Mold/MoldAtt.h"
    "Source/Templates/Mold/Mold.cpp"
    "Source/Templates/RenderPass/RenderPass.h"
    "Source/Templates/RenderPass/RenderPassAtt.h"
    "Source/Templates/RenderPass/RenderPass.cpp"
    "Source/Templates/RenderPass/Override/OverridePass.h"
    "Source/Templates/RenderPass/Override/OverridePass.cpp"
    "Source/Templates/RenderPass/Override/MinMaxChainPass.h"
    "Source/Templates/RenderPass/Override/MinMaxChainPass.cpp"
    "Source/Templates/RenderPass/Override/MinMaxChainResultPass.h"
    "Source/Templates/RenderPass/Override/MinMaxChainResultPass.cpp"
    "Source/Templates/RenderPass/Override/ResolvePass.h"
    "Source/Templates/RenderPass/Override/ResolvePass.cpp"
    "Source/Templates/RenderPass/Override/ResolveUIPass.h"
    "Source/Templates/RenderPass/Override/ResolveUIPass.cpp"
    "Source/Templates/RenderPass/Override/ToneMappingPass.h"
    "Source/Templates/RenderPass/Override/ToneMappingPass.cpp"
    "Source/Templates/Shader/Shader.h"
    "Source/Templates/Shader/ShaderAtt.h"
    "Source/Templates/Shader/Shader.cpp"
    "Source/Templates/Sound/Sound.h"
    "Source/Templates/Sound/SoundAtt.h"
    "Source/Templates/Sound/Sound.cpp"
    "Source/Templates/Textures/Texture.h"
    "Source/Templates/Textures/TextureAtt.h"
    "Source/Templates/Textures/Texture.cpp"
    "Source/TestGame/GameControllers/GameControllers.cpp"
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Editor
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/JExpose
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/JExpose/Primitives
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene/Renderable
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene/Camera
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Templates
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Primitives
    ${CMAKE_SOURCE_DIR}/SDKs/assimp/include
    ${CMAKE_SOURCE_DIR}/SDKs/build/assimp/include
    ${CMAKE_SOURCE_DIR}/SDKs/DirectXMath/Inc
)

# 4. Configurar Precompiled Header (PCH) global para el motor
target_precompile_headers(CulpeoEngine PRIVATE
    "Source/pch/Application.h"
    "Source/pch/DeleteHook.h"
    "Source/pch/UUID.h"
    "Source/pch/NoStd.h"
    "Source/pch/NoMath.h"
    "Source/pch/Json.h"
    "Source/pch/SceneUnitId.h"
    "Source/pch/ShaderMaterials.h"
    "Source/pch/RefTracker.h"
    "Source/JExpose/JTypes.h"
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

    target_compile_options(CulpeoEngine PRIVATE
        -msimd128
        -pthread
        "-D_EDITOR"
    )

    target_link_options(CulpeoEngine PRIVATE 
        -msimd128
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Debug>:-sASSERTIONS=2>

        -pthread
        "-sPTHREAD_POOL_SIZE=4"
        "-sASYNCIFY=1" 
        "-sUSE_GLFW=3" 
        "-sALLOW_MEMORY_GROWTH=1"
        "-sGROWABLE_ARRAYBUFFERS=2"
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