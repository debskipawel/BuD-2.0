cbuffer torus : register(b0)
{
    matrix modelMtx;
    float2 radius;
    uint2 segments;
    float3 color;
}

struct HSInput
{
    float4 pos : SV_Position;
};

struct HS_CONTROL_POINT_OUTPUT
{
    matrix model : MODEL;
    float2 radius : RADIUS;
    float3 color : COLOR;
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

    Output.EdgeTessFactor[0] = Output.EdgeTessFactor[2] = segments.g;
    Output.EdgeTessFactor[1] = Output.EdgeTessFactor[3] = segments.r;
    
    Output.InsideTessFactor[0] = segments.r;
    Output.InsideTessFactor[1] = segments.g;

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

    Output.model = modelMtx;
    Output.radius = radius;
    Output.color = color;

	return Output;
}
