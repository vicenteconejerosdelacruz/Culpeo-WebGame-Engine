cmake_minimum_required(VERSION 3.22)
project(CulpeoWebGameEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Parche global para evitar errores fatales con las macros de Emscripten en Abseil/Dawn
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-pragma")

if(EMSCRIPTEN)
    # 1. Definiciones globales para silenciar SAL y activar el fallback portable de DirectXMath
    add_compile_definitions(
        _XM_NO_INTRINSICS_
        __EMX__
        __x86_64__              # Evita `#error DirectX Math does not support this target` en WASM
        _SAL_VERSION=20
    )

    # 2. Banderas de compilación para C++
    add_compile_options(
        -pthread
        -msimd128               # Traduce la matemática de DirectXMath a WASM SIMD128
    )
    
    # 3. Banderas para el enlazador (wasm-ld)
    add_link_options(
        -pthread
        -msimd128
    )
endif()

# 1. Compila todas las librerías externas primero
add_subdirectory(SDKs)

# 2. Compila el motor del juego/aplicación después
add_subdirectory(Engine)