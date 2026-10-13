cmake_minimum_required(VERSION 3.20)

# Forzar Release (o RelWithDebInfo si prefieres conservar algunos símbolos)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

project(WasmSDKs CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-pragma")

# --- 1. Opciones para Assimp ---
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # Estáticas para Emscripten

# --- 2. Opciones para Dawn ---
set(DAWN_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
#set(DAWN_ENABLE_INSTALL ON CACHE BOOL "" FORCE)
set(DAWN_FETCH_DEPENDENCIES ON CACHE BOOL "" FORCE)
set(DAWN_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(DAWN_BUILD_PROTOBUF OFF CACHE BOOL "" FORCE)
set(TINT_BUILD_FUZZERS OFF CACHE BOOL "" FORCE)
set(DAWN_WERROR OFF CACHE BOOL "" FORCE)

# --- 3. Opciones para yaml-cpp y json ---
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)

# --- 4. Opciones para PhysX (WebAssembly / Emscripten) ---
set(PX_GENERATE_STATIC_LIBRARIES ON CACHE BOOL "" FORCE)
set(PX_SUPPORT_GPU_PHYSX OFF CACHE BOOL "" FORCE)
set(PX_SUPPORT_OMNIVERSE OFF CACHE BOOL "" FORCE)
set(PX_SUPPORT_PVD OFF CACHE BOOL "" FORCE)      # <-- Asegúrate de que esté en OFF
set(NV_USE_GAMEWORKS_ALLOCATOR OFF CACHE BOOL "" FORCE)
set(PHYSX_USE_PTHREADS ON CACHE BOOL "" FORCE)

# Desactivar la generación y compilación del subsistema de metadatos PVD
set(PX_INCLUDE_METADATA_IN_SDK OFF CACHE BOOL "" FORCE)

# Forzar el nombre de plataforma de PhysX para evitar la carpeta "UNKNOWN"
set(PX_BUILDSYS_LOWER_PLATFORM "linux" CACHE STRING "" FORCE)

# La ruta al CMakeLists interno dentro del submódulo de fabmax/physx-js-webidl
set(PHYSX_INTERNAL_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/PhysX/PhysX/physx/compiler/public")

if(EMSCRIPTEN)
    add_compile_options(
        -Wno-unknown-pragmas
        -Wno-invalid-offsetof
        -Wno-unused-variable
        -Wno-missing-include-dirs
    )
endif()

# --- 5. Incluir todas las librerías como subdirectorios ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assimp")
    add_subdirectory(assimp)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/dawn")
    add_subdirectory(dawn)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp")
    add_subdirectory(yaml-cpp)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/json")
    add_subdirectory(json)
endif()

if(EXISTS "${PHYSX_INTERNAL_CMAKE_DIR}/CMakeLists.txt")
    # Configurar directorios raíz
    get_filename_component(PHYSX_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/PhysX/PhysX/physx" ABSOLUTE)
    set(PHYSX_ROOT_DIR "${PHYSX_ROOT_DIR}" CACHE PATH "PhysX Root Directory" FORCE)

    # Forzar modo Release y plataforma Linux/Emscripten para que no caiga en UNKNOWN/Debug
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    set(PX_BUILDSYS_LOWER_PLATFORM "linux" CACHE STRING "" FORCE)
    set(TARGET_BUILD_PLATFORM "emscripten" CACHE STRING "" FORCE)
    set(PX_TARGET_ARCH "wasm" CACHE STRING "" FORCE)

    # Rutas de salida para las librerías estáticas
    set(PX_OUTPUT_LIB_DIR "${CMAKE_BINARY_DIR}/physx_lib" CACHE PATH "PhysX Output Lib Directory" FORCE)
    set(PX_OUTPUT_BIN_DIR "${CMAKE_BINARY_DIR}/physx_bin" CACHE PATH "PhysX Output Bin Directory" FORCE)

    # Indicar plataforma Emscripten para que el build system active la configuración de WASM
    set(TARGET_BUILD_PLATFORM "emscripten" CACHE STRING "Target Build Platform" FORCE)
    set(PX_TARGET_ARCH "wasm" CACHE STRING "Target Architecture" FORCE)

    # Definiciones estáticas y de entorno WASM
    add_compile_definitions(
        PX_PHYSX_STATIC_LIB
        PX_WASM=1
        PX_EMSCRIPTEN=1
        __EMSCRIPTEN__=1
        PX_x86=0
        PX_LP64=0
        _Bool=bool
    )

    # Opciones opcionales para silenciar advertencias estrictas del compilador Clang de Emscripten
    add_compile_options(
        -Wno-error
        -Wno-everything
        -Wno-unknown-pragmas
        -msse
        -msse2
        -msimd128
    )

# ==========================================================================
    # PARCHES WASM: Ajustar tamaños estáticos de 64-bits a 32-bits (WASM)
    # ==========================================================================

    # 1. GuConvexMeshData.h (ConvexHullData 80 -> 68)
    set(PATH_CONVEX "${PHYSX_ROOT_DIR}/source/geomutils/src/convex/GuConvexMeshData.h")
    if(EXISTS "${PATH_CONVEX}")
        file(READ "${PATH_CONVEX}" CONTENT)
        string(REPLACE "PX_COMPILE_TIME_ASSERT(sizeof(Gu::ConvexHullData) == 80);" "PX_COMPILE_TIME_ASSERT(sizeof(Gu::ConvexHullData) == 68);" CONTENT "${CONTENT}")
        file(WRITE "${PATH_CONVEX}" "${CONTENT}")
    endif()

    # 2. GuBigConvexData.h (BigConvexRawData 40 -> 24)
    set(PATH_BIG "${PHYSX_ROOT_DIR}/source/geomutils/src/convex/GuBigConvexData.h")
    if(EXISTS "${PATH_BIG}")
        file(READ "${PATH_BIG}" CONTENT)
        string(REPLACE "PX_COMPILE_TIME_ASSERT(sizeof(Gu::BigConvexRawData) == 40);" "PX_COMPILE_TIME_ASSERT(sizeof(Gu::BigConvexRawData) == 24);" CONTENT "${CONTENT}")
        file(WRITE "${PATH_BIG}" "${CONTENT}")
    endif()

    # 3. DySolverContact4.h (SolverContactHeader4 208 -> 192)
    set(PATH_SOLVER "${PHYSX_ROOT_DIR}/source/lowleveldynamics/src/DySolverContact4.h")
    if(EXISTS "${PATH_SOLVER}")
        file(READ "${PATH_SOLVER}" CONTENT)
        string(REPLACE "PX_COMPILE_TIME_ASSERT(sizeof(SolverContactHeader4) == 208);" "PX_COMPILE_TIME_ASSERT(sizeof(SolverContactHeader4) == 192);" CONTENT "${CONTENT}")
        file(WRITE "${PATH_SOLVER}" "${CONTENT}")
    endif()

    # 4. ScActorCore.h (ActorCore 16 -> 12)
    set(PATH_ACTOR "${PHYSX_ROOT_DIR}/source/simulationcontroller/include/ScActorCore.h")
    if(EXISTS "${PATH_ACTOR}")
        file(READ "${PATH_ACTOR}" CONTENT)
        string(REPLACE "PX_COMPILE_TIME_ASSERT(sizeof(Sc::ActorCore)==16);" "PX_COMPILE_TIME_ASSERT(sizeof(Sc::ActorCore)==12);" CONTENT "${CONTENT}")
        file(WRITE "${PATH_ACTOR}" "${CONTENT}")
    endif()
    
    # 5. PxMetaDataObjects.h (Proteger _Bool para C++)
    set(PATH_METADATA "${PHYSX_ROOT_DIR}/source/physxmetadata/core/include/PxMetaDataObjects.h")
    if(EXISTS "${PATH_METADATA}")
        file(READ "${PATH_METADATA}" META_CONTENT)
        string(REPLACE "typedef bool _Bool;" "#ifndef __cplusplus\ntypedef bool _Bool;\n#endif" META_CONTENT "${META_CONTENT}")
        file(WRITE "${PATH_METADATA}" "${META_CONTENT}")
        message(STATUS "[PhysX WASM Patch] Protegido correctamente _Bool en PxMetaDataObjects.h")
    endif()
    # ==========================================================================

    # Incluir el sistema de compilación interno de PhysX
    add_subdirectory("${PHYSX_INTERNAL_CMAKE_DIR}" physx_build)
endif()

# --- 6. Meta-Target para compilar todas las SDKs juntas ---
add_custom_target(WasmSDKs)

set(SDK_TARGETS 
    assimp 
    yaml-cpp 
    webgpu_glfw 
    webgpu_cpp
    PhysXFoundation
    PhysXCommon
    PhysX
    PhysXCooking
    PhysXExtensions
)

foreach(tgt IN LISTS SDK_TARGETS)
    if(TARGET ${tgt})
        add_dependencies(WasmSDKs ${tgt})
    endif()
endforeach()