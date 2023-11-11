struct VSOutput
{
    float4 pos : SV_Position;
    float3 worldPos : WORLDPOS;
    float2 rangeU : RANGE_U;
    float2 rangeV : RANGE_V;
    float2 tessFactor : TESSFACTOR;
};

struct HSOutput
{
	float3 worldPos : WORLDPOS;
    float2 rangeU : RANGE_U;
    float2 rangeV : RANGE_V;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4] : SV_TessFactor;
	float InsideTessFactor[2] : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 4

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

    Output.EdgeTessFactor[1] = Output.EdgeTessFactor[3] = Output.InsideTessFactor[0] = clamp(ip[0].tessFactor.x, 1.0, 64.0);
    Output.EdgeTessFactor[0] = Output.EdgeTessFactor[2] = Output.InsideTessFactor[1] = clamp(ip[0].tessFactor.y, 1.0, 64.0);

	return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
HSOutput main(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    HSOutput o = (HSOutput) 0;

    o.worldPos = ip[i].worldPos;
    o.rangeU = ip[i].rangeU;
    o.rangeV = ip[i].rangeV;

	return o;
}
