#define M_PI 3.1415926535897f

cbuffer mvp : register(b0)
{
	matrix projMtx;
}

struct DSOutput
{
	float4 position  : SV_POSITION;
	float3 worldPosition : WORLD_POS;
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
	float EdgeTessFactor[2]	: SV_TessFactor;
};

#define NUM_CONTROL_POINTS 1

float3 CalculateMiddleBezierPoint(float3 p0, float3 p1, float3 p2)
{
    return p1 + 0.5 * (p1 - p0) + 0.5 * (p1 - p2);
}

float3 DeCastiljeau(float3 controlPoints[3], float t)
{
	float3 a = lerp(controlPoints[0], controlPoints[1], t);
	float3 b = lerp(controlPoints[1], controlPoints[2], t);
	
    return lerp(a, b, t);
}

float3 TrygonometricBlending(float3 controlPoints[4], float t)
{
    float3 firstMiddlePoint = CalculateMiddleBezierPoint(controlPoints[0], controlPoints[1], controlPoints[2]);
    float3 secondMiddlePoint = CalculateMiddleBezierPoint(controlPoints[1], controlPoints[2], controlPoints[3]);
	
    float3 firstSegment[3] = { controlPoints[0], firstMiddlePoint, controlPoints[2] };
    float3 secondSegment[3] = { controlPoints[1], secondMiddlePoint, controlPoints[3] };
	
    return pow(cos(M_PI * t), 2.0f) * DeCastiljeau(firstSegment, t + 0.5) + pow(sin(M_PI * t), 2.0f) * DeCastiljeau(secondSegment, t);
}

[domain("isoline")]
DSOutput main(
	HSConstantDataOutput input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HSOutput, NUM_CONTROL_POINTS> patch)
{
	DSOutput o = (DSOutput) 0;

	o.color = patch[0].color;
	
	float u = domain.x;
    float t = 0.5 * u;
	
    float3 controlPoints[4] = { patch[0].controlPoint0, patch[0].controlPoint1, patch[0].controlPoint2, patch[0].controlPoint3 };
	
    o.worldPosition = TrygonometricBlending(controlPoints, t);
	o.position = mul(projMtx, float4(o.worldPosition, 1.0));
	
	return o;
}
