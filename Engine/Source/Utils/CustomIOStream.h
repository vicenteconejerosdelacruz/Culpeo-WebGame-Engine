#include <assimp/IOStream.hpp>
#include <string>
#include <cstring>

class CustomIOStream : public Assimp::IOStream {
public:
    explicit CustomIOStream(std::string data) 
        : m_data(std::move(data)), m_cursor(0) {}

    size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override;
    size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override;
    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;
    size_t Tell() const override;
    size_t FileSize() const override;
    void Flush() override;

private:
    std::string m_data;
    size_t m_cursor;
};