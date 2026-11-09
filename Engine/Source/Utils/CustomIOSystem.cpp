#include "CustomIOSystem.h"
#include "CustomIOStream.h"

bool CustomIOSystem::Exists(const char* pFile) const
{
    if (!pFile) return false;
    
    // Intentamos instanciar el AssetStream para verificar si carga correctamente
    AssetStream asset(pFile);
    return static_cast<bool>(asset);
}

char CustomIOSystem::getOsSeparator() const
{
    return '/'; // Estándar web y multiplataforma
}

Assimp::IOStream* CustomIOSystem::Open(const char* pFile, const char* pMode)
{
    if (!pFile) return nullptr;

    // 1. Creamos tu AssetStream usando la ruta virtual solicitada por Assimp
    AssetStream asset(pFile);
    if (!asset) {
        return nullptr; // No se pudo cargar el asset
    }

    // 2. Extraemos el contenido del std::istringstream a un string persistente
    std::string fileContent = asset.get().str();

    // 3. Retornamos el stream personalizado que administra los datos en memoria
    return new CustomIOStream(std::move(fileContent));
}

void CustomIOSystem::Close(Assimp::IOStream* pFile)
{
    delete pFile;
}