cmake_minimum_required(VERSION 3.22)

# Asegurar que CULPEO_CONFIG tenga un valor por defecto si no se pasa
if(NOT CULPEO_CONFIG)
    set(CULPEO_CONFIG "Development" CACHE STRING "Culpeo configuration mode" FORCE)
endif()

# --- DETERMINAR SUBDIRECTORIOS Y CARPETAS AISLADAS SEGÚN CONFIGURACIÓN HÍBRIDA ---
# Para Editor_Debug y Debug: Assimp/Yaml/Json usan Debug, PhysX/Dawn usan Release (con símbolos)
# Para Development/Editor_Development/Release/Editor_Release: Todos usan Release
if(CULPEO_CONFIG STREQUAL "Editor_Debug" OR CULPEO_CONFIG STREQUAL "Debug")
    set(LIB_SUBDIR_STANDARD "Debug")
    set(LIB_SUBDIR_PHYSX "Release")
    set(SDK_BUILD_DIR_STANDARD "${CMAKE_SOURCE_DIR}/build_Debug")
    set(SDK_BUILD_DIR_PHYSX    "${CMAKE_SOURCE_DIR}/build_Release")
    set(SDK_BUILD_DIR_YAMLCPP  "${CMAKE_SOURCE_DIR}/build_Release")
else()
    set(LIB_SUBDIR_STANDARD "Release")
    set(LIB_SUBDIR_PHYSX "Release")
    set(SDK_BUILD_DIR_STANDARD "${CMAKE_SOURCE_DIR}/build_Release")
    set(SDK_BUILD_DIR_PHYSX    "${CMAKE_SOURCE_DIR}/build_Release")
    set(SDK_BUILD_DIR_YAMLCPP  "${CMAKE_SOURCE_DIR}/build_Release")
endif()

# 1. Define el ejecutable de tu motor
add_executable(CulpeoEngine
    "Source/Engine.cpp"
    "Source/Editor/DefaultLevel.h"
    "Source/Editor/DefaultLevel.cpp"
    "Source/Editor/Editor.h"
    "Source/Editor/Editor.cpp"
    "Source/Editor/EditorMouseCamera.h"
    "Source/Editor/EditorMouseCamera.cpp"
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
    "Source/Renderer/DeviceUtils/RenderPass/RenderToTexturePass.h"
    "Source/Renderer/DeviceUtils/RenderPass/RenderToTexturePass.cpp"
    "Source/Renderer/DeviceUtils/RenderPass/SwapChainPass.h"
    "Source/Renderer/DeviceUtils/RenderPass/SwapChainPass.cpp"
    "Source/Renderer/DeviceUtils/VertexBuffer/VertexBuffer.h"
    "Source/Renderer/DeviceUtils/VertexBuffer/VertexBuffer.cpp"
    "Source/Renderer/DeviceUtils/UniformBuffer/UniformBuffer.h"
    "Source/Renderer/DeviceUtils/UniformBuffer/UniformBuffer.cpp"
    "Source/Renderer/Renderer.h"
    "Source/Renderer/Renderer.cpp"
    "Source/Renderer/VertexFormats.h"
    "Source/Renderer/VertexFormats.cpp"
    "Source/Shaders/Compiler/ShaderCompiler.h"
    "Source/Shaders/Compiler/ShaderCompiler.cpp"
    "Source/Scene/Binder.h"
    "Source/Scene/Binder.cpp"
    "Source/Scene/Level.h"
    "Source/Scene/Level.cpp"
    "Source/Scene/Scene.h"
    "Source/Scene/Scene.cpp"
    "Source/Scene/SceneUnit.h"
    "Source/Scene/SceneUnit.cpp"
    "Source/Scene/SceneObject.h"
    "Source/Scene/SceneObject.cpp"
    "Source/Scene/Light/Light.h"
    "Source/Scene/Light/LightAtt.h"
    "Source/Scene/Light/Light.cpp"
    "Source/Scene/Camera/Camera.h"
    "Source/Scene/Camera/CameraAtt.h"
    "Source/Scene/Camera/Camera.cpp"
    "Source/Scene/Camera/Projections/Orthographic.h"
    "Source/Scene/Camera/Projections/Perspective.h"
    "Source/Scene/Renderable/Renderable.h"
    "Source/Scene/Renderable/RenderableAtt.h"
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
    "Source/Utils/FileSystem.h"
    "Source/Utils/FileSystem.cpp"
)

# 2. Configura la ruta de salida usando directamente el nombre de la configuración
set_target_properties(CulpeoEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${CULPEO_CONFIG}"
)

# 3. Directorios de inclusión locales del motor y dependencias (Actualizado dinámicamente)
target_include_directories(CulpeoEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Editor
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/JExpose
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/JExpose/Primitives
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Shaders/Compiler
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene/Camera
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene/Light
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene/Renderable
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Templates
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Primitives
    ${CMAKE_SOURCE_DIR}/SDKs/assimp/include
    ${CMAKE_SOURCE_DIR}/SDKs/yaml-cpp/include
    ${CMAKE_SOURCE_DIR}/SDKs/tinyddsloader/include
    ${SDK_BUILD_DIR_STANDARD}/SDKs/assimp/include
    ${CMAKE_SOURCE_DIR}/SDKs/DirectXMath/Inc
)

# 4. Configurar Precompiled Header (PCH) global para el motor
target_precompile_headers(CulpeoEngine PRIVATE
    "Source/pch/Application.h"
    "Source/pch/Debug.h"
    "Source/pch/DeleteHook.h"
    "Source/pch/IDTypes.h"
    "Source/pch/UUID.h"
    "Source/pch/NoStd.h"
    "Source/pch/NoMath.h"
    "Source/pch/Json.h"
    "Source/pch/SceneUnitId.h"
    "Source/pch/ShaderMaterials.h"
    "Source/pch/RefTracker.h"
    "Source/pch/WebGpuTypes.h"
    "Source/JExpose/JTypes.h"
    "Source/Templates/TemplateDecl.h"
    "Source/Templates/TemplateDef.h"
    "Source/Utils/Yaml2Json.h"
)

# Determinar el nombre del archivo de assimp y yaml-cpp según si es Debug o Release
if(LIB_SUBDIR_STANDARD STREQUAL "Debug")
    set(ASSIMP_LIB_NAME "libassimpd.a")
#    set(YAMLCPP_LIB_NAME "libyaml-cppd.a")
else()
    set(ASSIMP_LIB_NAME "libassimp.a")
#    set(YAMLCPP_LIB_NAME "libyaml-cpp.a")
endif()
#yaml en release, es lentisimo en debug
set(YAMLCPP_LIB_NAME "libyaml-cpp.a")

# Definir la subcarpeta de PhysX según sea release o debug interno
if(LIB_SUBDIR_PHYSX STREQUAL "Debug")
    set(PHYSX_BIN_SUBDIR "debug")
else()
    set(PHYSX_BIN_SUBDIR "release")
endif()

# 5. Configuración específica para WebAssembly (Emscripten)
if(EMSCRIPTEN)
    target_link_libraries(CulpeoEngine PRIVATE 
        # Dawn / WebGPU
        emdawnwebgpu_cpp 
        webgpu_glfw 

        # Assimp, yaml-cpp y json (Usan el directorio estándar según Debug/Release)
        "${SDK_BUILD_DIR_STANDARD}/lib/${LIB_SUBDIR_STANDARD}/${ASSIMP_LIB_NAME}"
        #"${SDK_BUILD_DIR_STANDARD}/lib/${LIB_SUBDIR_STANDARD}/${YAMLCPP_LIB_NAME}"
        "${SDK_BUILD_DIR_YAMLCPP}/lib/Release/${YAMLCPP_LIB_NAME}"
        nlohmann_json::nlohmann_json

        # PhysX (Con rutas profundas UNKNOWN, subcarpeta dinámica y sufijo _static)
        "${SDK_BUILD_DIR_PHYSX}/physx_lib/${LIB_SUBDIR_PHYSX}/bin/UNKNOWN/${PHYSX_BIN_SUBDIR}/libPhysXFoundation_static.a"
        "${SDK_BUILD_DIR_PHYSX}/physx_lib/${LIB_SUBDIR_PHYSX}/bin/UNKNOWN/${PHYSX_BIN_SUBDIR}/libPhysXCommon_static.a"
        "${SDK_BUILD_DIR_PHYSX}/physx_lib/${LIB_SUBDIR_PHYSX}/bin/UNKNOWN/${PHYSX_BIN_SUBDIR}/libPhysX_static.a"
        "${SDK_BUILD_DIR_PHYSX}/physx_lib/${LIB_SUBDIR_PHYSX}/bin/UNKNOWN/${PHYSX_BIN_SUBDIR}/libPhysXCooking_static.a"
        "${SDK_BUILD_DIR_PHYSX}/physx_lib/${LIB_SUBDIR_PHYSX}/bin/UNKNOWN/${PHYSX_BIN_SUBDIR}/libPhysXExtensions_static.a"
    )

    # Flags condicionales según si es Editor o Debug puro
    if(CULPEO_CONFIG MATCHES "Editor")
        target_compile_definitions(CulpeoEngine PRIVATE "_EDITOR")
    endif()

    target_compile_options(CulpeoEngine PRIVATE
        -msimd128
        -pthread
    )

    target_link_options(CulpeoEngine PRIVATE 
        -msimd128
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Debug>:-sASSERTIONS=2>
        -pthread
        "-sPTHREAD_POOL_SIZE=4"
        "-sFETCH=1"
        "-sASYNCIFY=1" 
        "-sUSE_GLFW=3" 
        "-sALLOW_MEMORY_GROWTH=1"
        "-sGROWABLE_ARRAYBUFFERS=2"
    )
else()
    # Entorno nativo (si aplica)
    target_link_libraries(CulpeoEngine PRIVATE 
        webgpu_dawn 
        webgpu_glfw 
        glfw 
        assimp
        yaml-cpp
        nlohmann_json::nlohmann_json
        PhysXFoundation
        PhysXCommon
        PhysX
        PhysXCooking
        PhysXExtensions
    )
endif()