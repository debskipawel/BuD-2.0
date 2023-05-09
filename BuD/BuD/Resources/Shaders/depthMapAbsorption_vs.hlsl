struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoords : TEXCOORDS;
};

struct VSOutput
{
    float4 hpos : SV_POSITION;
    float dist : TEXCOORD0; // distance from light 
};

cbuffer cbWorld : register(b0)
{
    matrix projectionMatrix;
    matrix viewMatrix;
    matrix worldMatrix;
    float  grow;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 P = float4(input.pos, 1.0f);
    P.xyz += input.normal * grow; // scale vertex along normal   
    output.hpos = mul(projectionMatrix, mul(viewMatrix, mul(worldMatrix, P)));
    
    float4 worldPos = mul(worldMatrix, float4(input.pos, 1));
    float4 viewPos = mul(viewMatrix, worldPos);
    
    output.dist = length(viewPos.xyz);
    return output;
}