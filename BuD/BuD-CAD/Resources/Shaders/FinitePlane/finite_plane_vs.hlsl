cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 insPosition : INS_POSITION;
    float3 insVectorU : INS_DERIVATIVE_U;
    float3 insVectorV : INS_DERIVATIVE_V;
    float2 insSize : INS_SIZE;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 dU : DERIVATIVE_U;
    float3 dV : DERIVATIVE_V;
    float2 uvCoords : UV_COORDS;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float3 worldPosition = i.insPosition + 0.5f * (i.position.x * i.insSize.x * i.insVectorU + i.position.y * i.insSize.y * i.insVectorV);
    
    o.position = mul(projMtx, mul(viewMtx, float4(worldPosition, 1.0f)));
    o.dU = i.insVectorU;
    o.dV = i.insVectorV;
    o.uvCoords = 0.5f * i.position.xy + float2(0.5f, 0.5f);  
    
	return o;
}