cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoords : TEXCOORDS;
    float4 insModelRowX : INS_MODEL_X;
    float4 insModelRowY : INS_MODEL_Y;
    float4 insModelRowZ : INS_MODEL_Z;
    float4 insModelRowW : INS_MODEL_W;
    float4 insColor : INS_COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float3 cameraPos : CAMERAPOS;
    float4 color : COLOR;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    matrix modelMtx = transpose(float4x4(i.insModelRowX, i.insModelRowY, i.insModelRowZ, i.insModelRowW));
    
    o.worldPos = mul(modelMtx, float4(i.position, 1.0f)).xyz;
    o.position = mul(projMtx, mul(viewMtx, float4(o.worldPos, 1.0f)));
    o.cameraPos = -mul(viewMtx, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    o.color = i.insColor;
    
	return o;
}