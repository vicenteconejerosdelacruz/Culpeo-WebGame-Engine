cmake_minimum_required(VERSION 3.22)
project(CulpeoWebGameEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Parche global para evitar errores fatales con las macros de Emscripten en Abseil/Dawn
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-pragma")

if(EMSCRIPTEN)
    # Las bibliotecas estáticas también deben compilarse con soporte pthread/atomics.
    add_compile_options(-pthread)
endif()

# 1. Compila todas las librerías externas primero
add_subdirectory(SDKs)

# 2. Compila el motor del juego/aplicación después
add_subdirectory(Engine)