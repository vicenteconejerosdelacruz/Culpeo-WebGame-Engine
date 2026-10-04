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
set(DAWN_BUILD_PROTOBUF OFF CACHE BOOL "" FORCE) # Si Dawn te lo permite, apagar protobuf aligera mucho
set(TINT_BUILD_FUZZERS OFF CACHE BOOL "" FORCE)
set(DAWN_WERROR OFF CACHE BOOL "" FORCE)

# --- 3. Incluir las librerías como subdirectorios ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assimp")
    add_subdirectory(assimp)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/dawn")
    add_subdirectory(dawn)
endif()