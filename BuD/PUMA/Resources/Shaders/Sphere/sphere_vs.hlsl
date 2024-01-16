cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoords : TEXCOORDS;
    float3 insPosition : INS_POSITION;
};

float4 main(VSInput i) : SV_POSITION
{
    float sphereRadius = 0.1f;
    
    float3 worldPos = sphereRadius * i.position + i.insPosition;
    
    float4 position = mul(projMtx, mul(viewMtx, float4(worldPos, 1.0f)));
    
    return position;
}