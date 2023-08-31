struct VSOutput
{
	float4 position : SV_POSITION;
	float3 controlPoint0 : CONTROL_POINTS_0;
	float3 controlPoint1 : CONTROL_POINTS_1;
	float3 controlPoint2 : CONTROL_POINTS_2;
	float3 controlPoint3 : CONTROL_POINTS_3;
	float3 color : COLOR;
};

struct HSOutput
{
	float3 controlPoint0 : CONTROL_POINTS_0;
	float3 controlPoint1 : CONTROL_POINTS_1;
	float3 controlPoint2 : CONTROL_POINTS_2;
	float3 controlPoint3 : CONTROL_POINTS_3;
	float3 color : COLOR;
};

struct HSConstantDataOutput
{
	float EdgeTessFactor[2] : SV_TessFactor;
};

#define NUM_CONTROL_POINTS 1

HSConstantDataOutput CalcHSPatchConstants(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HSConstantDataOutput Output;
	
	Output.EdgeTessFactor[0] = 1;
	
	float avgDepth = -0.25 * (ip[0].controlPoint0.z + ip[0].controlPoint1.z + ip[0].controlPoint2.z + ip[0].controlPoint3.z);
	
    if (any(isnan(ip[0].controlPoint0)) || any(isnan(ip[0].controlPoint3)))
    {
        avgDepth = -0.5 * (ip[0].controlPoint1.z + ip[0].controlPoint2.z);
    }
	
    Output.EdgeTessFactor[1] = -16 * log10(0.01 * avgDepth) + 12;

	return Output;
}

[domain("isoline")]
[partitioning("fractional_odd")]
[outputtopology("line")]
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
	
	o.color = ip[i].color;

	return o;
}
