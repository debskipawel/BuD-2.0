cbuffer invMvp : register(b0)
{
    matrix invView;
    matrix invProj;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 insPosition : INS_POSITION;
    float3 insNormal : INS_NORMAL;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 rayOrigin : RAY_ORIGIN;
    float3 rayDirection : RAY_DIRECTION;
    float3 pointOnPlane : PLANE_POINT;
    float3 normalToPlane : PLANE_NORMAL;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float4 farPoint = mul(invProj, float4(i.position.xy, 1.0f, 1.0f));
    farPoint /= farPoint.w;
	
    o.pos = float4(i.position.xy, 0.0f, 1.0f);
    
    o.pointOnPlane = i.insPosition;
    o.normalToPlane = i.insNormal;
    
    o.rayOrigin = mul(invView, float4(0.0, 0.0, 0.0, 1.0)).xyz;
    o.rayDirection = mul(invView, farPoint).xyz - o.rayOrigin;
    
	return o;
}