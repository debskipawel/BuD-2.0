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
    float3 color : COLOR;
};

struct HSConstantDataOutput
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 1

float3 DeCastiljeau(float3 controlPoints[4], float t)
{
	float3 a = lerp(controlPoints[0], controlPoints[1], t);
	float3 b = lerp(controlPoints[1], controlPoints[2], t);
	float3 c = lerp(controlPoints[2], controlPoints[3], t);
	float3 d = lerp(a, b, t);
	float3 e = lerp(b, c, t);
	
	return lerp(d, e, t);
}

static const float4x4 bSplineToBernstein =
{
    1.0 / 6, 2.0 / 3, 1.0 / 6, 0.0,
	0.0, 2.0 / 3, 1.0 / 3, 0.0,
	0.0, 1.0 / 3, 2.0 / 3, 0.0,
	0.0, 1.0 / 6, 2.0 / 3, 1.0 / 6
};

[domain("quad")]
DSOutput main(
	HSConstantDataOutput input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HSOutput, NUM_CONTROL_POINTS> patch)
{
	DSOutput o = (DSOutput) 0;

	o.color = patch[0].color;
	
    float u = domain.x, v = domain.y;
	
    float4x3 uPointsMatrix0 = { patch[0].controlPoint0, patch[0].controlPoint1, patch[0].controlPoint2, patch[0].controlPoint3 };
    float4x3 uPointsMatrix1 = { patch[0].controlPoint4, patch[0].controlPoint5, patch[0].controlPoint6, patch[0].controlPoint7 };
    float4x3 uPointsMatrix2 = { patch[0].controlPoint8, patch[0].controlPoint9, patch[0].controlPoint10, patch[0].controlPoint11 };
    float4x3 uPointsMatrix3 = { patch[0].controlPoint12, patch[0].controlPoint13, patch[0].controlPoint14, patch[0].controlPoint15 };
	
    float4x3 uPointsInBernstein0 = mul(bSplineToBernstein, uPointsMatrix0);
    float4x3 uPointsInBernstein1 = mul(bSplineToBernstein, uPointsMatrix1);
    float4x3 uPointsInBernstein2 = mul(bSplineToBernstein, uPointsMatrix2);
    float4x3 uPointsInBernstein3 = mul(bSplineToBernstein, uPointsMatrix3);
	
    float3 uControlPoints0[4] = { uPointsInBernstein0._11_12_13, uPointsInBernstein0._21_22_23, uPointsInBernstein0._31_32_33, uPointsInBernstein0._41_42_43 };
    float3 uControlPoints1[4] = { uPointsInBernstein1._11_12_13, uPointsInBernstein1._21_22_23, uPointsInBernstein1._31_32_33, uPointsInBernstein1._41_42_43 };
    float3 uControlPoints2[4] = { uPointsInBernstein2._11_12_13, uPointsInBernstein2._21_22_23, uPointsInBernstein2._31_32_33, uPointsInBernstein2._41_42_43 };
    float3 uControlPoints3[4] = { uPointsInBernstein3._11_12_13, uPointsInBernstein3._21_22_23, uPointsInBernstein3._31_32_33, uPointsInBernstein3._41_42_43 };

    float4x3 vPointsMatrix = { DeCastiljeau(uControlPoints0, u), DeCastiljeau(uControlPoints1, u), DeCastiljeau(uControlPoints2, u), DeCastiljeau(uControlPoints3, u) };
    float4x3 vPointsInBernstein = mul(bSplineToBernstein, vPointsMatrix);
    
    float3 vControlPoints[4] = { vPointsInBernstein._11_12_13, vPointsInBernstein._21_22_23, vPointsInBernstein._31_32_33, vPointsInBernstein._41_42_43 };
    
    o.worldPosition = DeCastiljeau(vControlPoints, v);
	o.position = mul(projMtx, float4(o.worldPosition, 1.0));
	
	return o;
}
