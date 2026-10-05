cmake_minimum_required(VERSION 3.20)
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

# --- 4. Incluir todas las librerías como subdirectorios ---
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

# --- 5. Meta-Target para compilar todas las SDKs juntas ---
add_custom_target(WasmSDKs)

set(SDK_TARGETS 
    assimp 
    yaml-cpp 
    webgpu_glfw 
    webgpu_cpp
)

foreach(tgt IN LISTS SDK_TARGETS)
    if(TARGET ${tgt})
        add_dependencies(WasmSDKs ${tgt})
    endif()
endforeach()