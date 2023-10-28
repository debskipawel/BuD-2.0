cbuffer mvp : register(b0)
{
    matrix modelMtx;
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoords : TEXCOORDS;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float3 normal : NORMAL;
    float3 viewVector : VIEW;
    float2 texCoords : TEXCOORDS;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float4 worldPos = mul(modelMtx, float4(i.position, 1.0));
    float4 worldNormal = mul(modelMtx, float4(i.normal, 0.0));
    
    float4 screenPos = mul(projMtx, mul(viewMtx, worldPos));
    
    float3 eyePos = -mul(viewMtx, float4(0.0, 0.0, 0.0, 1.0)).xyz;
    
    o.position = screenPos;
    o.worldPos = worldPos.xyz;
    o.normal = worldNormal.xyz;
    o.viewVector = eyePos - worldPos.xyz;
    o.texCoords = i.texCoords;
    
    return o;
}