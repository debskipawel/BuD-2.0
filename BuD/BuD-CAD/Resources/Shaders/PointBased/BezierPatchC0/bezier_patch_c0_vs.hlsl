cbuffer mvp : register(b0)
{
	matrix viewMtx;
}

struct VSInput
{
	float3 position : POSITION;
	float3 controlPoint0  : INS_POINT0;
	float3 controlPoint1  : INS_POINT1;
	float3 controlPoint2  : INS_POINT2;
	float3 controlPoint3  : INS_POINT3;
	float3 controlPoint4  : INS_POINT4;
	float3 controlPoint5  : INS_POINT5;
	float3 controlPoint6  : INS_POINT6;
	float3 controlPoint7  : INS_POINT7;
	float3 controlPoint8  : INS_POINT8;
	float3 controlPoint9  : INS_POINT9;
	float3 controlPoint10 : INS_POINT10;
	float3 controlPoint11 : INS_POINT11;
	float3 controlPoint12 : INS_POINT12;
	float3 controlPoint13 : INS_POINT13;
	float3 controlPoint14 : INS_POINT14;
    float3 controlPoint15 : INS_POINT15;
    float2 rangeU : INS_PARAMETER_RANGE_U;
    float2 rangeV : INS_PARAMETER_RANGE_V;
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
    float3 controlPoint4 : CONTROL_POINTS_4;
    float3 controlPoint5 : CONTROL_POINTS_5;
    float3 controlPoint6 : CONTROL_POINTS_6;
    float3 controlPoint7 : CONTROL_POINTS_7;
    float3 controlPoint8 : CONTROL_POINTS_8;
    float3 controlPoint9 : CONTROL_POINTS_9;
    float3 controlPoint10 : CONTROL_POINTS_10;
    float3 controlPoint11 : CONTROL_POINTS_11;
    float3 controlPoint12 : CONTROL_POINTS_12;
    float3 controlPoint13 : CONTROL_POINTS_13;
    float3 controlPoint14 : CONTROL_POINTS_14;
    float3 controlPoint15 : CONTROL_POINTS_15;
    float2 rangeU : PARAMETER_RANGE_U;
    float2 rangeV : PARAMETER_RANGE_V;
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
	o.controlPoint4 = mul(viewMtx, float4(i.controlPoint4, 1.0)).xyz;
	o.controlPoint5 = mul(viewMtx, float4(i.controlPoint5, 1.0)).xyz;
	o.controlPoint6 = mul(viewMtx, float4(i.controlPoint6, 1.0)).xyz;
	o.controlPoint7 = mul(viewMtx, float4(i.controlPoint7, 1.0)).xyz;
	o.controlPoint8 = mul(viewMtx, float4(i.controlPoint8, 1.0)).xyz;
	o.controlPoint9 = mul(viewMtx, float4(i.controlPoint9, 1.0)).xyz;
	o.controlPoint10 = mul(viewMtx, float4(i.controlPoint10, 1.0)).xyz;
	o.controlPoint11 = mul(viewMtx, float4(i.controlPoint11, 1.0)).xyz;
	o.controlPoint12 = mul(viewMtx, float4(i.controlPoint12, 1.0)).xyz;
	o.controlPoint13 = mul(viewMtx, float4(i.controlPoint13, 1.0)).xyz;
	o.controlPoint14 = mul(viewMtx, float4(i.controlPoint14, 1.0)).xyz;
    o.controlPoint15 = mul(viewMtx, float4(i.controlPoint15, 1.0)).xyz;
	
    o.rangeU = i.rangeU;
    o.rangeV = i.rangeV;
	
	o.color = i.color;
	
	return o;
}