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
    float4 insModelX : INS_MODEL_X;
    float4 insModelY : INS_MODEL_Y;
    float4 insModelZ : INS_MODEL_Z;
    float4 insModelW : INS_MODEL_W;
    float3 insColor : INS_COLOR;
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
    
    matrix modelMtx = transpose(float4x4(i.insModelX, i.insModelY, i.insModelZ, i.insModelW));
    
    float3 worldPos = mul(modelMtx, float4(i.position, 1.0f)).xyz;
    float3 cameraPos = -mul(viewMtx, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    
    o.worldPos = worldPos;
    o.cameraPos = cameraPos;
    
    o.position = mul(projMtx, mul(viewMtx, float4(worldPos, 1.0f)));
    o.color = float4(i.insColor, 1.0f);
    
	return o;
}