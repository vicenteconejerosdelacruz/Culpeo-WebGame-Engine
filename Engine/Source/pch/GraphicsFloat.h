#pragma once

#include <cstdint>

namespace Graphics
{
    struct FLOAT2
    {
        float x;
        float y;

        constexpr FLOAT2() = default;
        constexpr FLOAT2(float _x, float _y) : x(_x), y(_y) {}
        
        FLOAT2(const FLOAT2&) = default;
        FLOAT2& operator=(const FLOAT2&) = default;
        FLOAT2(FLOAT2&&) noexcept = default;
        FLOAT2& operator=(FLOAT2&&) noexcept = default;
    };

    struct FLOAT3
    {
        float x;
        float y;
        float z;

        constexpr FLOAT3() = default;
        constexpr FLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        FLOAT3(const FLOAT3&) = default;
        FLOAT3& operator=(const FLOAT3&) = default;
        FLOAT3(FLOAT3&&) noexcept = default;
        FLOAT3& operator=(FLOAT3&&) noexcept = default;
    };

    struct FLOAT4
    {
        float x;
        float y;
        float z;
        float w;

        constexpr FLOAT4() = default;
        constexpr FLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

        FLOAT4(const FLOAT4&) = default;
        FLOAT4& operator=(const FLOAT4&) = default;
        FLOAT4(FLOAT4&&) noexcept = default;
        FLOAT4& operator=(FLOAT4&&) noexcept = default;
    };

    struct UINT4
    {
        std::uint32_t x;
        std::uint32_t y;
        std::uint32_t z;
        std::uint32_t w;

        constexpr UINT4() = default;
        constexpr UINT4(std::uint32_t _x, std::uint32_t _y, std::uint32_t _z, std::uint32_t _w) : x(_x), y(_y), z(_z), w(_w) {}

        UINT4(const UINT4&) = default;
        UINT4& operator=(const UINT4&) = default;
        UINT4(UINT4&&) noexcept = default;
        UINT4& operator=(UINT4&&) noexcept = default;
    };
}