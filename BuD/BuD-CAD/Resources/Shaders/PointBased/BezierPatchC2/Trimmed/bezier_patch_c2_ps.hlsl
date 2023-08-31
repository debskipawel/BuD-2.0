Texture2D parameterSpace : register(t0);
SamplerState samplerState
{
    AddressU = Clamp;
    AddressV = Clamp;
    Filter = MIN_MAG_MIP_LINEAR;
};

struct DSOutput
{
	float4 position : SV_POSITION;
	float3 worldPosition : WORLD_POS;
    float3 normal : NORMAL;
    float depth : DEPTH;
    float3 color : COLOR;
    float2 texCoords : TEX_COORDS;
};

struct PSOutput
{
    float4 color : SV_TARGET;
    float depth : SV_DEPTH;
};

PSOutput main(DSOutput i)
{
    PSOutput o = (PSOutput) 0;
    
    float3 parameterSpaceSample = parameterSpace.Sample(samplerState, i.texCoords).xyz;
    
    o.color = float4(i.color, 1.0f);
    o.depth = i.depth + (dot(parameterSpaceSample, parameterSpaceSample) < 2.0f) * 100.0f;
    
    return o;
}