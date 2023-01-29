struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoords : TEXCOORDS;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 worldPos : WORLD_POS;
    float3 normal : NORMAL;
    float3 viewVector : VIEW_VECTOR;
    float2 tex : TEXCOORDS;
};

cbuffer matrices : register(b0)
{
    matrix worldMtx;
    matrix viewMtx;
    matrix invViewMtx;
    matrix projMtx;
}

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    o.worldPos = mul(worldMtx, float4(i.pos, 1.0));
    o.normal = mul(worldMtx, float4(i.normal, 0.0)).xyz;
    
    float3 cameraPos = mul(invViewMtx, float4(0, 0, 0, -1)).xyz;
    o.viewVector = normalize(cameraPos - o.worldPos.xyz);
    
    o.tex = i.texCoords;
    o.pos = float4((i.texCoords * 2 - 1), 0.0, 1.0);
    
    return o;
}