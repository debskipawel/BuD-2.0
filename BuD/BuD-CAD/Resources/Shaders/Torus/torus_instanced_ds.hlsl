#define TWO_PI 6.283185307179586

cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct DS_OUTPUT
{
	float4 position  : SV_POSITION;
    float3 normal : NORMAL;
};

struct HS_CONTROL_POINT_OUTPUT
{
    float3 insPos : INS_POSITION;
    float3 insOrientation : INS_ORIENTATION;
    float2 radius : INS_RADIUS;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]		: SV_TessFactor;
	float InsideTessFactor[2]	: SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 1

[domain("quad")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

    float R = patch[0].radius.r;
    float r = patch[0].radius.g;
	
    float phi = domain.r * TWO_PI;
    float theta = domain.g * TWO_PI;
	
    float cp = cos(phi), sp = sin(phi);
    float ct = cos(theta), st = sin(theta);
	
    // local position
    float3 position = float3((R + r * ct) * cp, (R + r * ct) * sp, r * st);
	
    float3 dp = float3((-R - r * ct) * sp, (R + r * ct) * cp, 0);
    float3 dt = float3(-r * cp * st, -r * sp * st, -r * ct);
    float3 normal = cross(dp, dt);
	
    // scale
    
    // rotation
    float3 origin = float3(0.0, 1.0, 0.0);
    float3 orientation = normalize(patch[0].insOrientation);
    
    float3 rotationAxis = cross(origin, orientation);
    float rotationAngle = dot(origin, orientation);
    
    // translation
    position += patch[0].insPos;
	
    Output.position = mul(projMtx, mul(viewMtx, float4(position, 1.0)));
    Output.normal = normalize(normal);
	
	return Output;
}
