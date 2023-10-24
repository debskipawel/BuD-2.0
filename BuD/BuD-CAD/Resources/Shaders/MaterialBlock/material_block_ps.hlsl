
Texture2D surfaceTexture : register(t0);
SamplerState surfaceSampler
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

struct PSOutput
{
    float4 color : SV_Target;
};

PSOutput main(DSOutput i)
{
    PSOutput o = (PSOutput) 0;
    
    o.color = surfaceTexture.Sample(surfaceSampler, i.uv);
    
    return o;
}