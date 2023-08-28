cbuffer mvp : register(b0)
{
	matrix projMtx;
}

struct DSOutput
{
	float4 position  : SV_POSITION;
	float3 worldPosition : WORLD_POS;
    float3 normal : NORMAL;
    float depth : DEPTH;
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
	
    float4x3 vPointsMatrix0 = { patch[0].controlPoint0, patch[0].controlPoint4, patch[0].controlPoint8, patch[0].controlPoint12 };
    float4x3 vPointsMatrix1 = { patch[0].controlPoint1, patch[0].controlPoint5, patch[0].controlPoint9, patch[0].controlPoint13 };
    float4x3 vPointsMatrix2 = { patch[0].controlPoint2, patch[0].controlPoint6, patch[0].controlPoint10, patch[0].controlPoint14 };
    float4x3 vPointsMatrix3 = { patch[0].controlPoint3, patch[0].controlPoint7, patch[0].controlPoint11, patch[0].controlPoint15 };
    
    float4x3 uPointsInBernstein0 = mul(bSplineToBernstein, uPointsMatrix0);
    float4x3 uPointsInBernstein1 = mul(bSplineToBernstein, uPointsMatrix1);
    float4x3 uPointsInBernstein2 = mul(bSplineToBernstein, uPointsMatrix2);
    float4x3 uPointsInBernstein3 = mul(bSplineToBernstein, uPointsMatrix3);
    
    float4x3 vPointsInBernstein0 = mul(bSplineToBernstein, vPointsMatrix0);
    float4x3 vPointsInBernstein1 = mul(bSplineToBernstein, vPointsMatrix1);
    float4x3 vPointsInBernstein2 = mul(bSplineToBernstein, vPointsMatrix2);
    float4x3 vPointsInBernstein3 = mul(bSplineToBernstein, vPointsMatrix3);
	
    float3 uControlPoints0[4] = { uPointsInBernstein0._11_12_13, uPointsInBernstein0._21_22_23, uPointsInBernstein0._31_32_33, uPointsInBernstein0._41_42_43 };
    float3 uControlPoints1[4] = { uPointsInBernstein1._11_12_13, uPointsInBernstein1._21_22_23, uPointsInBernstein1._31_32_33, uPointsInBernstein1._41_42_43 };
    float3 uControlPoints2[4] = { uPointsInBernstein2._11_12_13, uPointsInBernstein2._21_22_23, uPointsInBernstein2._31_32_33, uPointsInBernstein2._41_42_43 };
    float3 uControlPoints3[4] = { uPointsInBernstein3._11_12_13, uPointsInBernstein3._21_22_23, uPointsInBernstein3._31_32_33, uPointsInBernstein3._41_42_43 };

    float3 vControlPoints0[4] = { vPointsInBernstein0._11_12_13, vPointsInBernstein0._21_22_23, vPointsInBernstein0._31_32_33, vPointsInBernstein0._41_42_43 };
    float3 vControlPoints1[4] = { vPointsInBernstein1._11_12_13, vPointsInBernstein1._21_22_23, vPointsInBernstein1._31_32_33, vPointsInBernstein1._41_42_43 };
    float3 vControlPoints2[4] = { vPointsInBernstein2._11_12_13, vPointsInBernstein2._21_22_23, vPointsInBernstein2._31_32_33, vPointsInBernstein2._41_42_43 };
    float3 vControlPoints3[4] = { vPointsInBernstein3._11_12_13, vPointsInBernstein3._21_22_23, vPointsInBernstein3._31_32_33, vPointsInBernstein3._41_42_43 };
    
    float4x3 vPointsMatrix = { DeCastiljeau3(uControlPoints0, u), DeCastiljeau3(uControlPoints1, u), DeCastiljeau3(uControlPoints2, u), DeCastiljeau3(uControlPoints3, u) };
    float4x3 uPointsMatrix = { DeCastiljeau3(vControlPoints0, v), DeCastiljeau3(vControlPoints1, v), DeCastiljeau3(vControlPoints2, v), DeCastiljeau3(vControlPoints3, v) };
    
    float4x3 vPointsInBernstein = mul(bSplineToBernstein, vPointsMatrix);
    float4x3 uPointsInBernstein = mul(bSplineToBernstein, uPointsMatrix);
    
    float3 vControlPoints[4] = { vPointsInBernstein._11_12_13, vPointsInBernstein._21_22_23, vPointsInBernstein._31_32_33, vPointsInBernstein._41_42_43 };
    float3 uControlPoints[4] = { uPointsInBernstein._11_12_13, uPointsInBernstein._21_22_23, uPointsInBernstein._31_32_33, uPointsInBernstein._41_42_43 };
    
    o.worldPosition = DeCastiljeau3(vControlPoints, v);
    o.position = mul(projMtx, float4(o.worldPosition, 1.0));
    o.depth = o.position.z / o.position.w;
    
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
