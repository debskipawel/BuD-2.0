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
	float3 eyePos : EYE_POS;
	float3 farPoint : FAR_POINT;
};

VSOutput main(VSInput i)
{
	VSOutput o = (VSOutput) 0;
	
	o.pos = float4(i.position, 1.0);
	
	float4 farPoint = mul(invProj, float4(i.position.xy, 1.0, 1.0));
	farPoint /= farPoint.w;
	
	o.farPoint = mul(invView, farPoint).xyz;
	o.eyePos = mul(invView, float4(0.0, 0.0, 0.0, 1.0)).xyz;
	
	return o;
}