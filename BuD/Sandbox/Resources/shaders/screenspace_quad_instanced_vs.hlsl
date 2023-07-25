cbuffer mat : register(b0)
{
	float4 cameraPos;
	matrix invMvp;
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

VSOut main(
	float3 pos      : POSITION,
	float3 normal	: NORMAL,
	float4 xRow     : ROW_X, 
	float4 yRow     : ROW_Y, 
	float4 zRow     : ROW_Z, 
	float4 wRow     : ROW_W,
	uint instanceID : SV_InstanceID
)
{
	VSOut o = (VSOut) 0;
	
    float4x4 model = transpose(float4x4(xRow, yRow, zRow, wRow));
	
	o.pos = mul(model, float4(pos.xy, 0.0, 1.0));
	o.rayOrigin = cameraPos.xyz;
	o.rayDir = GetRayDirection(o.pos.xy);
	
	return o;
}