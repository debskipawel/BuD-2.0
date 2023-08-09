#define INFINITY 100.0f
#define EPSILON 0.5f

cbuffer mvp : register(b0)
{
	matrix viewMtx;
	matrix projMtx;
}

struct VSOutput
{
	float4 pos : SV_POSITION;
	float3 eyePos : EYE_POS;
	float3 farPoint : FAR_POINT;
};

struct PSOutput
{
	float4 color : SV_Target;
	float depth : SV_Depth;
};

float4 Grid(float4 fragPos3D, float scale)
{
	float2 coord = fragPos3D.xy * scale;
	float2 derivative = fwidth(coord);
	float2 grid = abs(frac(coord - 0.5) - 0.5) / derivative;
	
	float yMin = min(derivative.y, 1);
	float xMin = min(derivative.x, 1);

	float lineVar = min(grid.x, grid.y);
	float4 color = float4(0.2, 0.2, 0.2, 1.0 - min(lineVar, 1.0));
	
	// y axis
	if (fragPos3D.x > -0.5 * xMin && fragPos3D.x < 0.5 * xMin)
	{
		color.y = 1.0;
	}
	
	// x axis
	if (fragPos3D.y > -0.5 * yMin && fragPos3D.y < 0.5 * yMin)
	{
		color.x = 1.0;
	}
	
		return color;
}

float ComputeDepth(float3 pos)
{
	float4 clip_space_pos = mul(projMtx, mul(viewMtx, float4(pos.xyz, 1.0)));
	return (clip_space_pos.z / clip_space_pos.w);
}

float ComputeLinearDepth(float3 pos)
{
	float clipSpaceDepth = ComputeDepth(pos) * 2.0 - 1.0;

	// INFINITY here is anologous to the far clipping distance in perspective matrix
	// it never changes in the engine and currently I don't see a use-case to change it, so it's hard-coded here
	float linearDepth = 2.0f / ((1.0 - clipSpaceDepth) * INFINITY);
	
	return linearDepth / INFINITY;
}

PSOutput main(VSOutput i)
{
	PSOutput o = (PSOutput) 0;
	
	float t = -i.eyePos.z / (i.farPoint.z - i.eyePos.z);
	float3 p = i.eyePos + t * (i.farPoint - i.eyePos);
	
	float linearDepth = ComputeLinearDepth(p);
	float fading = 2 * max(0, (0.5 - linearDepth));
	
	float4 color = Grid(float4(p, 1.0), 1.0);
	color.rgb *= fading;
	
	o.color = color;
	o.depth = abs(o.color.a - 1.0) < EPSILON && t > 0 ? ComputeDepth(p) : INFINITY;
	
	return o;
}