cbuffer mvp : register(b0)
{
	matrix projMtx;
}

struct DSOutput
{
	float4 position  : SV_POSITION;
	float3 worldPosition : WORLD_POS;
    float3 normal : NORMAL;
	float3 color : COLOR;
    float2 texCoords : TEX_COORDS;
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

float3 DeCastiljeau2(float3 controlPoints[3], float t)
{
    float3 a = lerp(controlPoints[0], controlPoints[1], t);
    float3 b = lerp(controlPoints[1], controlPoints[2], t);
	
    return lerp(a, b, t);
}

float3 DeCastiljeau3(float3 controlPoints[4], float t)
{
	float3 a = lerp(controlPoints[0], controlPoints[1], t);
	float3 b = lerp(controlPoints[1], controlPoints[2], t);
	float3 c = lerp(controlPoints[2], controlPoints[3], t);
	float3 d = lerp(a, b, t);
	float3 e = lerp(b, c, t);
	
	return lerp(d, e, t);
}

[domain("quad")]
DSOutput main(
	HSConstantDataOutput input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HSOutput, NUM_CONTROL_POINTS> patch)
{
	DSOutput o = (DSOutput) 0;

	o.color = patch[0].color;
	
    float u = domain.x, v = domain.y;
	
	float3 uControlPoints0[4] = { patch[0].controlPoint0, patch[0].controlPoint1, patch[0].controlPoint2, patch[0].controlPoint3 };
	float3 uControlPoints1[4] = { patch[0].controlPoint4, patch[0].controlPoint5, patch[0].controlPoint6, patch[0].controlPoint7 };
	float3 uControlPoints2[4] = { patch[0].controlPoint8, patch[0].controlPoint9, patch[0].controlPoint10, patch[0].controlPoint11 };
    float3 uControlPoints3[4] = { patch[0].controlPoint12, patch[0].controlPoint13, patch[0].controlPoint14, patch[0].controlPoint15 };

    float3 vControlPoints0[4] = { patch[0].controlPoint0, patch[0].controlPoint4, patch[0].controlPoint8, patch[0].controlPoint12 };
    float3 vControlPoints1[4] = { patch[0].controlPoint1, patch[0].controlPoint5, patch[0].controlPoint9, patch[0].controlPoint13 };
    float3 vControlPoints2[4] = { patch[0].controlPoint2, patch[0].controlPoint6, patch[0].controlPoint10, patch[0].controlPoint14 };
    float3 vControlPoints3[4] = { patch[0].controlPoint3, patch[0].controlPoint7, patch[0].controlPoint11, patch[0].controlPoint15 };
	
    float3 vControlPoints[4] = { DeCastiljeau3(uControlPoints0, u), DeCastiljeau3(uControlPoints1, u), DeCastiljeau3(uControlPoints2, u), DeCastiljeau3(uControlPoints3, u) };
    float3 uControlPoints[4] = { DeCastiljeau3(vControlPoints0, v), DeCastiljeau3(vControlPoints1, v), DeCastiljeau3(vControlPoints2, v), DeCastiljeau3(vControlPoints3, v) };
	
    o.worldPosition = DeCastiljeau3(vControlPoints, v);
    o.position = mul(projMtx, float4(o.worldPosition, 1.0));
    
    float3 vDerControlPoints[3] = { 3.0f * (vControlPoints[1] - vControlPoints[0]), 3.0f * (vControlPoints[2] - vControlPoints[1]), 3.0f * (vControlPoints[3] - vControlPoints[2]) };
    float3 uDerControlPoints[3] = { 3.0f * (uControlPoints[1] - uControlPoints[0]), 3.0f * (uControlPoints[2] - uControlPoints[1]), 3.0f * (uControlPoints[3] - uControlPoints[2]) };
	
    float3 vDer = DeCastiljeau2(vDerControlPoints, v);
    float3 uDer = DeCastiljeau2(uDerControlPoints, u);
    
    o.normal = cross(vDer, uDer);
    o.texCoords = float2(
        (patch[0].rangeU.g - patch[0].rangeU.r) * u + patch[0].rangeU.r,
        (patch[0].rangeV.g - patch[0].rangeV.r) * v + patch[0].rangeV.r
    );
    
	return o;
}
