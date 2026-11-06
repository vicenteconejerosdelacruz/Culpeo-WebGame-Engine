#ifndef __EMSCRIPTEN__
#define __EMSCRIPTEN__
#endif

// Macros obligatorias para evitar bloques de ensamblador x86/SIMD en WebAssembly
#define MA_NO_CPU_DETECTION
#define MA_NO_SIMD
#define MA_NO_SSE2
#define MA_NO_AVX2

// Desactivación de módulos opcionales para aligerar la compilación
#define MA_NO_ENCODING
#define MA_NO_GENERATION
#define MA_NO_DECODE_MP3
#define MA_NO_FLAC
// #define MA_NO_WAV // Comenta esta si vas a usar archivos WAV
// #define MA_NO_RESOURCE_MANAGER
// #define MA_NO_NODE_GRAPH
// #define MA_NO_ENGINE

// #define MA_API static
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>