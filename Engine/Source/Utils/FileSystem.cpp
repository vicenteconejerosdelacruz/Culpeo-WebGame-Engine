#include "FileSystem.h"
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <unordered_set>

// Implementación de EM_JS (SOLO EN EL .CPP)
EM_JS(int, js_download_asset_to_buffer, (const char* url, uint8_t** outData, int* outSize), {
    const urlStr = UTF8ToString(url);

    try {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', urlStr, false); // Síncrono en Worker
        xhr.responseType = 'arraybuffer';
        xhr.send();

        if (xhr.status === 200) {
            const arrayBuffer = xhr.response;
            const length = arrayBuffer.byteLength;

            // Si el archivo está vacío (0 bytes)
            if (length === 0) {
                setValue(outData, 0, '*');
                setValue(outSize, 0, 'i32');
                return 1; // Éxito (archivo vacío)
            }

            const ptr = _malloc(length);
            if (!ptr) return 0;

            HEAPU8.set(new Uint8Array(arrayBuffer), ptr);
            
            setValue(outData, ptr, '*');
            setValue(outSize, length, 'i32');

            return 1;
        } else {
            console.error("[AssetStream] Error HTTP:", xhr.status, xhr.statusText, "para la URL:", urlStr);
        }
    } catch (e) {
        console.error("[AssetStream] Excepción en XHR:", e);
    }
    return 0;
});

AssetStream::AssetStream(const std::string& virtualPath) {
    std::string downloadUrl = virtualPath;

    uint8_t* bufferPtr = nullptr;
    int bufferSize = 0;

    int success = js_download_asset_to_buffer(downloadUrl.c_str(), &bufferPtr, &bufferSize);

    if (success) {
        std::string content;
        if (bufferPtr && bufferSize > 0) {
            content.assign(reinterpret_cast<char*>(bufferPtr), bufferSize);
            free(bufferPtr); // Liberar memoria de JS
        }

        // Inicializar el stream (vacío o con contenido)
        m_stream = std::istringstream(content, std::ios::binary);
    } else {
        std::cerr << "[AssetStream] No se pudo descargar el archivo: " << downloadUrl << std::endl;
        m_stream.setstate(std::ios::failbit);
    }
}