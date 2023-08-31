cbuffer mvp : register(b0)
{
	matrix viewMtx;
}

struct VSInput
{
	float3 position : POSITION;
	float3 controlPoint0 : INS_POINT0;
	float3 controlPoint1 : INS_POINT1;
	float3 controlPoint2 : INS_POINT2;
	float3 controlPoint3 : INS_POINT3;
    float3 color : INS_COLOR;
	uint instanceId : SV_InstanceID;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 controlPoint0 : CONTROL_POINTS_0;
	float3 controlPoint1 : CONTROL_POINTS_1;
	float3 controlPoint2 : CONTROL_POINTS_2;
	float3 controlPoint3 : CONTROL_POINTS_3;
    float3 color : COLOR;
};

VSOutput main(VSInput i)
{
	VSOutput o = (VSOutput) 0;
	
	o.position = float4(i.position, 1.0);
	
	o.controlPoint0 = mul(viewMtx, float4(i.controlPoint0, 1.0)).xyz;
	o.controlPoint1 = mul(viewMtx, float4(i.controlPoint1, 1.0)).xyz;
	o.controlPoint2 = mul(viewMtx, float4(i.controlPoint2, 1.0)).xyz;
	o.controlPoint3 = mul(viewMtx, float4(i.controlPoint3, 1.0)).xyz;
	
    o.color = i.color;
	
	return o;
}