cbuffer mvp : register(b0)
{
	matrix projMtx;
}

struct GSOutput
{
	float4 pos : SV_POSITION;
	float depth : DEPTH;
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

static const float4x4 bSplineToBernstein =
{
    1.0 / 6, 2.0 / 3, 1.0 / 6, 0.0,
	0.0, 2.0 / 3, 1.0 / 3, 0.0,
	0.0, 1.0 / 3, 2.0 / 3, 0.0,
	0.0, 1.0 / 6, 2.0 / 3, 1.0 / 6
};

[maxvertexcount(4)]
void main(
	point VSOutput input[1],
	inout LineStream<GSOutput> output
)
{
	GSOutput o = (GSOutput) 0;
	
	float4x3 pointsMatrix = { input[0].controlPoint0, input[0].controlPoint1, input[0].controlPoint2, input[0].controlPoint3 };
	float4x3 pointsInBspline = mul(bSplineToBernstein, pointsMatrix);
	
	float4 position0 = mul(projMtx, float4(pointsInBspline._11_12_13, 1.0));
	o.pos = position0;
	o.depth = position0.z / position0.w;
	output.Append(o);
	
	float4 position1 = mul(projMtx,  float4(pointsInBspline._21_22_23, 1.0));
	o.pos = position1;
	o.depth = position1.z / position1.w;
	output.Append(o);
	
	float4 position2 = mul(projMtx, float4(pointsInBspline._31_32_33, 1.0));
	o.pos = position2;
	o.depth = position2.z / position2.w;
	output.Append(o);
	
	float4 position3 = mul(projMtx, float4(pointsInBspline._41_42_43, 1.0));
	o.pos = position3;
	o.depth = position3.z / position3.w;
	output.Append(o);
}