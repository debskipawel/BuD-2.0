cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 insModelX : INS_MODEL_X;
    float4 insModelY : INS_MODEL_Y;
    float4 insModelZ : INS_MODEL_Z;
    float4 insModelW : INS_MODEL_W;
    uint instanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD;
    float3 worldPos : WORLD_POS;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float4x4 modelMtx = float4x4(i.insModelX, i.insModelY, i.insModelZ, i.insModelW);
    
    o.tex = 0.5 * (i.position + 1.0);
    o.worldPos = mul(modelMtx, float4(o.tex, 1.0));
    o.position = mul(projMtx, mul(viewMtx, float4(o.worldPos, 1.0)));
    
    return o;
}