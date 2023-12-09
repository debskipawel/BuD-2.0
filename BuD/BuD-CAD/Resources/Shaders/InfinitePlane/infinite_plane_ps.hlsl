cbuffer mvp : register(b0)
{
	matrix viewMtx;
	matrix projMtx;
}

struct VSOutput
{
	float4 pos : SV_POSITION;
	float3 rayOrigin : RAY_ORIGIN;
	float3 rayDirection : RAY_DIRECTION;
	float3 pointOnPlane : PLANE_POINT;
	float3 normalToPlane : PLANE_NORMAL;
};

struct PSOutput
{
	float4 color : SV_TARGET;
	float depth : SV_DEPTH;
};

PSOutput main(VSOutput i)
{
    PSOutput output = (PSOutput) 0;
	
	float3 o = i.rayOrigin;
	float3 d = normalize(i.rayDirection);
	
	float3 n = normalize(i.normalToPlane);
	float3 p0 = i.pointOnPlane;
	
	float denom = dot(d, n);
	
	if (abs(denom) < 1e-4f)
	{
        discard;
    }
	
    float t = dot(p0 - o, n) / denom;
	
	if (t < 0.0f)
    {
        discard;
    }
	
    float3 worldPosition = o + t * d;
    float4 screenPosition = mul(projMtx, mul(viewMtx, float4(worldPosition, 1.0f)));
	
	output.color = float4(0.2f, 0.2f, 0.2f, 1.0f);
    output.depth = screenPosition.z / screenPosition.w;
	
    return output;
}