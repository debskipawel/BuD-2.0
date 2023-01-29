struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoords : TEXCOORDS;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float dist : TEXCOORD0;
};

cbuffer cbWorld : register(b0)
{
    matrix projectionMatrix;
    matrix viewMatrix;
    matrix worldMatrix;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float4 worldPos = mul(worldMatrix, float4(i.pos, 1.0f));
    float4 viewPos = mul(viewMatrix, worldPos);
    
    o.pos = mul(projectionMatrix, viewPos);  
    o.dist = length(viewPos.xyz);
    
    return o;
}