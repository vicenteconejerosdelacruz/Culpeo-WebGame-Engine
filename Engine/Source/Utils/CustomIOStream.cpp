#include "CustomIOStream.h"

size_t CustomIOStream::Read(void* pvBuffer, size_t pSize, size_t pCount)
{
    size_t bytesToRead = pSize * pCount;
    size_t remaining = m_data.size() - m_cursor;
    
    if (bytesToRead > remaining) {
        bytesToRead = remaining;
    }
    if (bytesToRead == 0) {
        return 0;
    }

    std::memcpy(pvBuffer, m_data.data() + m_cursor, bytesToRead);
    m_cursor += bytesToRead;
    return bytesToRead / pSize;
}

size_t CustomIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount)
{
    // Los assets en CulpeoEngine son de solo lectura
    return 0; 
}

aiReturn CustomIOStream::Seek(size_t pOffset, aiOrigin pOrigin)
{
    size_t newCursor = m_cursor;
    
    if (pOrigin == aiOrigin_SET) {
        newCursor = pOffset;
    } else if (pOrigin == aiOrigin_CUR) {
        newCursor += pOffset;
    } else if (pOrigin == aiOrigin_END) {
        newCursor = m_data.size();
    }

    if (newCursor > m_data.size()) {
        return aiReturn_FAILURE;
    }

    m_cursor = newCursor;
    return aiReturn_SUCCESS;
}

size_t CustomIOStream::Tell() const
{
    return m_cursor;
}

size_t CustomIOStream::FileSize() const
{
    return m_data.size();
}

void CustomIOStream::Flush()
{
    // Sin operación para lectura en memoria
}