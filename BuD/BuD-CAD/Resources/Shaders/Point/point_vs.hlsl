cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 worldPosition : INS_POSITION;
    uint instanceId : SV_InstanceID;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 worldPos : WORLD_POS;
    float2 tex : TEXCOORD;
    float depth : DEPTH;
};

float ScalePoint(float3 pointPosition)
{
    float3 eyePosition = -mul(viewMtx, float4(0.0, 0.0, 0.0, 1.0)).xyz;
    float distanceToPoint = length(pointPosition - eyePosition);
    
    return 1.0 / (distanceToPoint + 10.0);
    return sqrt(10.0 * max(-0.05 * log(distanceToPoint + 1.0) + 0.1, 0.0));
}

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float4 viewSpacePosition = mul(viewMtx, float4(i.worldPosition, 1.0));
    float4 screenSpacePosition = mul(projMtx,viewSpacePosition);
    screenSpacePosition /= screenSpacePosition.w;
    
    viewSpacePosition.xy += ScalePoint(i.worldPosition) * i.position.xy;
    
    o.pos = mul(projMtx, viewSpacePosition);
    o.worldPos = i.worldPosition;
    o.tex = i.position.xy;
    o.depth = screenSpacePosition.z;
    
	return o;
}