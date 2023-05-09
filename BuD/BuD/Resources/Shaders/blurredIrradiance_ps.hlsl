
struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 worldPos : WORLD_POS;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 surfaceToView : SURFACETOVIEW;
    float2 texCoords : TEXCOORDS;
    float2 lightTexCoords : TEXCOORDS1;
};

Texture2D blurredIrradiance		: register(t0);
Texture2D albedoMap             : register(t1);
SamplerState samplerState		: register(s0);

cbuffer lightBuffer : register(b0)
{
    float4 lightPosition;
    float4 lightColor;
};

float4 main(VSOutput i) : SV_TARGET
{
    float2 tex = i.texCoords;
    tex.x = 1 - tex.x;
    
    float3 surface = albedoMap.Sample(samplerState, tex).xyz;
    float3 irradiance = blurredIrradiance.Sample(samplerState, tex).xyz;
    float3 light = lightColor.xyz;
    
    float4 color = float4(irradiance * light * surface, 1.0f);
    
    return color;
}