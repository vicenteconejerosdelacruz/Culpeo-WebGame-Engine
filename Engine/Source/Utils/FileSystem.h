#pragma once

#include <sstream>
#include <string>
#include <memory>

// Declaración externa de la función de JavaScript
extern "C" {
    bool js_download_asset_sync(const char* url, const char* virtualPath);
}

class AssetStream {
public:
    explicit AssetStream(const std::string& virtualPath, const std::string& url = "");
    ~AssetStream() = default;

    AssetStream(const AssetStream&) = delete;
    AssetStream& operator=(const AssetStream&) = delete;

    AssetStream(AssetStream&&) noexcept = default;
    AssetStream& operator=(AssetStream&&) noexcept = default;

    // Cambiado de std::ifstream a std::istringstream
    std::istringstream& get() { return m_stream; }
    const std::istringstream& get() const { return m_stream; }

    explicit operator bool() const { return !m_stream.fail(); }
    bool is_open() const { return true; }

    std::istringstream* operator->() { return &m_stream; }

private:
    std::istringstream m_stream;
};