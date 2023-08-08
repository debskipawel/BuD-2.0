cbuffer invMvp : register(b0)
{
    matrix invView;
    matrix invProj;
}

struct VSInput
{
	float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 eyePos : VIEW_POS;
    float3 eyeDir : VIEW_DIR;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    o.pos = float4(i.position, 1.0);
    
    float4 screenSpaceViewVector = float4(i.position.xy, 1.0, 0.0);
    float4 cameraSpaceViewVector = mul(invProj, screenSpaceViewVector);
    cameraSpaceViewVector /= cameraSpaceViewVector.w;
    
    float4 worldSpaceViewVector = mul(invView, cameraSpaceViewVector);
    
    o.eyeDir = worldSpaceViewVector.xyz;
    o.eyePos = mul(invView, float4(0.0, 0.0, 0.0, 1.0));
    
    return o;
}