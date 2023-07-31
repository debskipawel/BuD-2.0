cbuffer mat : register(b0)
{
	float4 cameraPos;
	matrix invMvp;
	matrix model;
}

struct VSOut
{
	float4 pos : SV_Position;
	float3 rayOrigin : ORIGIN;
	float3 rayDir : DIRECTION;
};

float3 GetRayDirection(float2 uv)
{
	float4 ssPos = float4(uv, 1.0, 1.0);
	float4 worldPos = mul(invMvp, ssPos);
	worldPos /= worldPos.w;
	
	return normalize(worldPos.xyz - cameraPos.xyz);
}

VSOut main(float3 pos : POSITION)
{
	VSOut o = (VSOut) 0;
	
	o.pos = mul(model, float4(pos.xy, 0.0, 1.0));
	o.rayOrigin = cameraPos.xyz;
	o.rayDir = GetRayDirection(o.pos.xy);
	
	return o;
}