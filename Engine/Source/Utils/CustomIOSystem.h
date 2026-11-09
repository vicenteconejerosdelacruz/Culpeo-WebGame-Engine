#include <assimp/IOSystem.hpp>
#include "AssetStream.h"

class CustomIOSystem : public Assimp::IOSystem {
public:
    CustomIOSystem() = default;
    ~CustomIOSystem() = default;

    bool Exists(const char* pFile) const override;
    char getOsSeparator() const override;
    Assimp::IOStream* Open(const char* pFile, const char* pMode = "rb") override;
    void Close(Assimp::IOStream* pFile) override;
};