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

struct HSOutput
{
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

struct HSConstantDataOutput
{
	float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 1

HSConstantDataOutput CalcHSPatchConstants(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HSConstantDataOutput Output;
	
    float3 midPoints[4] =
    {
        -0.5f * (ip[0].controlPoint0 + ip[0].controlPoint12),
        -0.5f * (ip[0].controlPoint0 + ip[0].controlPoint3),
        -0.5f * (ip[0].controlPoint3 + ip[0].controlPoint15),
        -0.5f * (ip[0].controlPoint12 + ip[0].controlPoint15)
    };
	
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        Output.edges[i] = -8 * log10(0.01 * midPoints[i].z) + 3;
    }
	
    float3 mid = -0.5f * (ip[0].controlPoint5 + ip[0].controlPoint10);
    
    Output.inside[0] = Output.inside[1] = -8 * log10(0.01 * mid.z) + 3;

	return Output;
}

[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(NUM_CONTROL_POINTS)]
[patchconstantfunc("CalcHSPatchConstants")]
HSOutput main(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HSOutput o = (HSOutput) 0;

	o.controlPoint0 = ip[i].controlPoint0;
	o.controlPoint1 = ip[i].controlPoint1;
	o.controlPoint2 = ip[i].controlPoint2;
	o.controlPoint3 = ip[i].controlPoint3;
	o.controlPoint4 = ip[i].controlPoint4;
	o.controlPoint5 = ip[i].controlPoint5;
	o.controlPoint6 = ip[i].controlPoint6;
	o.controlPoint7 = ip[i].controlPoint7;
	o.controlPoint8 = ip[i].controlPoint8;
	o.controlPoint9 = ip[i].controlPoint9;
	o.controlPoint10 = ip[i].controlPoint10;
	o.controlPoint11 = ip[i].controlPoint11;
	o.controlPoint12 = ip[i].controlPoint12;
	o.controlPoint13 = ip[i].controlPoint13;
	o.controlPoint14 = ip[i].controlPoint14;
    o.controlPoint15 = ip[i].controlPoint15;
	
    o.rangeU = ip[i].rangeU;
    o.rangeV = ip[i].rangeV;
    
	o.color = ip[i].color;

	return o;
}
