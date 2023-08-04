#include <bud_pch.h>

#include <d3d11.h>

namespace BuD
{
    static const std::map<DXGI_FORMAT, std::string> hlslTypeMap =
    {
        { DXGI_FORMAT_R32G32B32A32_TYPELESS, "" },
        { DXGI_FORMAT_R32G32B32A32_FLOAT, "float4" },
        { DXGI_FORMAT_R32G32B32A32_UINT, "uint4" },
        { DXGI_FORMAT_R32G32B32A32_SINT, "int4" },
        { DXGI_FORMAT_R32G32B32_TYPELESS, "" },
        { DXGI_FORMAT_R32G32B32_FLOAT, "float3" },
        { DXGI_FORMAT_R32G32B32_UINT, "uint3" },
        { DXGI_FORMAT_R32G32B32_SINT, "int3" },
        { DXGI_FORMAT_R16G16B16A16_TYPELESS, "" },
        { DXGI_FORMAT_R16G16B16A16_FLOAT, "half4" },
        { DXGI_FORMAT_R32G32_TYPELESS, "" },
        { DXGI_FORMAT_R32G32_FLOAT, "float2" },
        { DXGI_FORMAT_R32G32_UINT, "uint2" },
        { DXGI_FORMAT_R32G32_SINT, "int2" },
        { DXGI_FORMAT_D32_FLOAT, "float" },
        { DXGI_FORMAT_R32_FLOAT, "float" },
        { DXGI_FORMAT_R32_UINT, "uint" },
        { DXGI_FORMAT_R32_SINT, "int" },
    };
}
