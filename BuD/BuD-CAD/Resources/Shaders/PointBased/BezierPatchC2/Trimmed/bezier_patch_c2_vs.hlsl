cbuffer mvp : register(b0)
{
	matrix viewMtx;
}

cbuffer instanceData : register(b1)
{
    float4 unpackedControlPoints[12];
    float2 rangeU;
    float2 rangeV;
    float3 color;
}

struct VSInput
{
	float3 position : POSITION;
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
	
    float3 controlPoints[16];
	
	[unroll]
    for (int iter = 0; iter < 48; iter++)
    {
        int p = iter / 3;
        int c = iter % 3;
		
        int up = iter / 4;
        int uc = iter % 4;
		
        controlPoints[p][c] = unpackedControlPoints[up][uc];
    }
	
    o.position = float4(i.position, 1.0);
	
	o.controlPoint0 = mul(viewMtx, float4(controlPoints[0], 1.0)).xyz;
	o.controlPoint1 = mul(viewMtx, float4(controlPoints[1], 1.0)).xyz;
	o.controlPoint2 = mul(viewMtx, float4(controlPoints[2], 1.0)).xyz;
	o.controlPoint3 = mul(viewMtx, float4(controlPoints[3], 1.0)).xyz;
	o.controlPoint4 = mul(viewMtx, float4(controlPoints[4], 1.0)).xyz;
	o.controlPoint5 = mul(viewMtx, float4(controlPoints[5], 1.0)).xyz;
	o.controlPoint6 = mul(viewMtx, float4(controlPoints[6], 1.0)).xyz;
	o.controlPoint7 = mul(viewMtx, float4(controlPoints[7], 1.0)).xyz;
	o.controlPoint8 = mul(viewMtx, float4(controlPoints[8], 1.0)).xyz;
	o.controlPoint9 = mul(viewMtx, float4(controlPoints[9], 1.0)).xyz;
	o.controlPoint10 = mul(viewMtx, float4(controlPoints[10], 1.0)).xyz;
	o.controlPoint11 = mul(viewMtx, float4(controlPoints[11], 1.0)).xyz;
	o.controlPoint12 = mul(viewMtx, float4(controlPoints[12], 1.0)).xyz;
	o.controlPoint13 = mul(viewMtx, float4(controlPoints[13], 1.0)).xyz;
	o.controlPoint14 = mul(viewMtx, float4(controlPoints[14], 1.0)).xyz;
    o.controlPoint15 = mul(viewMtx, float4(controlPoints[15], 1.0)).xyz;
	
    o.rangeU = rangeU;
    o.rangeV = rangeV;
	
	o.color = color;
	
	return o;
}