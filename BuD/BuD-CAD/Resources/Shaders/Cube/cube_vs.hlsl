cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 position : POSITION;
    float3 normal : NORMAL;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    o.pos = mul(projMtx, mul(viewMtx, float4(i.position, 1.0)));
    o.normal = i.normal;
    
	return o;
}