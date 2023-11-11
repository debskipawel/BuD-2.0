cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

Texture2D heightMap : register(t0);
SamplerState heightSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct DSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float2 uv : TEXCOORDS;
};

struct HSOutput
{
    float3 worldPos : WORLDPOS;
    float2 rangeU : RANGE_U;
    float2 rangeV : RANGE_V;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[4] : SV_TessFactor;
    float InsideTessFactor[2] : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 4

[domain("quad")]
DSOutput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HSOutput, NUM_CONTROL_POINTS> patch)
{
    DSOutput o = (DSOutput) 0;

    float3 uDir = (patch[1].worldPos - patch[0].worldPos);
    float3 vDir = (patch[3].worldPos - patch[0].worldPos);
    float3 up = float3(0.0, 1.0, 0.0);
	
    float u = patch[0].rangeU.x + domain.x * (patch[0].rangeU.y - patch[0].rangeU.x);
    float v = patch[0].rangeV.x + domain.x * (patch[0].rangeV.y - patch[0].rangeV.x);
    float w = domain.y;
	
    float4 heightSample = heightMap.SampleLevel(heightSampler, float2(u, v), 0);
    
    float3 worldPos = patch[0].worldPos + u * uDir + v * vDir + w * heightSample.r * up;

    float4 screenPos = mul(projMtx, mul(viewMtx, float4(worldPos, 1.0)));
    
    o.position = screenPos;
    o.worldPos = worldPos;
    o.uv = float2(u, v);
    
    return o;
}
