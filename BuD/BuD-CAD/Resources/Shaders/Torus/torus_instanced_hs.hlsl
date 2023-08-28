struct HSInput
{
    float4 pos : SV_Position;
    matrix model : INS_MODEL;
    float2 radius : INS_RADIUS;
    uint2 segments : INS_SEGMENTS;
    float3 color : INS_COLOR;
};

struct HS_CONTROL_POINT_OUTPUT
{
    matrix model : INS_MODEL;
    float2 radius : INS_RADIUS;
    float3 color : INS_COLOR;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]	    : SV_TessFactor;
	float InsideTessFactor[2]	: SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 1

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<HSInput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

    Output.EdgeTessFactor[0] = Output.EdgeTessFactor[2] = ip[PatchID].segments.g;
    Output.EdgeTessFactor[1] = Output.EdgeTessFactor[3] = ip[PatchID].segments.r;
    
    Output.InsideTessFactor[0] = ip[PatchID].segments.r;
    Output.InsideTessFactor[1] = ip[PatchID].segments.g;

	return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(1)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<HSInput, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

    Output.model = ip[i].model;
    Output.radius = ip[i].radius;
    Output.color = ip[i].color;

	return Output;
}
