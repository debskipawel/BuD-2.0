cbuffer mvp : register(b0)
{
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 insPosition : INS_POSITION;
    float2 insSize : INS_SIZE;
    float3 insColor : INS_COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float3 worldPosition = float3(i.insPosition.xy + 0.5f * i.position.xy * i.insSize, i.insPosition.z);
    
    o.position = mul(projMtx, float4(worldPosition, 1.0f));
    o.color = i.insColor;
    
	return o;
}