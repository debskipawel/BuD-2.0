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
    float2 lightTex : TEXCOORDS1;
};

cbuffer matrices : register(b0)
{
    matrix worldMtx;
    matrix viewMtx;
    matrix invViewMtx;
    matrix projMtx;
}

cbuffer light : register(b1)
{
    matrix lightViewMtx;
    matrix lightProjMtx;
}

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    o.worldPos = mul(worldMtx, float4(i.pos, 1.0));
    o.normal = mul(worldMtx, float4(i.normal, 0.0)).xyz;
    
    float3 cameraPos = mul(invViewMtx, float4(0, 0, 0, -1)).xyz;
    o.viewVector = normalize(cameraPos - o.worldPos.xyz);
    
    float4 lightViewPos = mul(lightViewMtx, o.worldPos);
    float4 lightProjPos = mul(lightProjMtx, lightViewPos);
    
    o.lightTex = (lightProjPos.xy / lightProjPos.w + float2(1.0f, 1.0f)) * 0.5;
    o.lightTex.y = 1 - o.lightTex.y;
    
    o.tex = i.texCoords;
    o.tex.y = 1 - o.tex.y;
    o.pos = float4((i.texCoords * 2 - 1), 0.0, 1.0);
    
    return o;
}