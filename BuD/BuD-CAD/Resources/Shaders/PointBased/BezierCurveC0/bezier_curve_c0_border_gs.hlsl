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

[maxvertexcount(4)]
void main(
	point VSOutput input[1],
	inout LineStream<GSOutput> output
)
{
    GSOutput o = (GSOutput) 0;
	
	float4 position0 = mul(projMtx, float4(input[0].controlPoint0, 1.0));
    o.pos = position0;
    o.depth = position0.z / position0.w;
    output.Append(o);
	
    float4 position1 = mul(projMtx, float4(input[0].controlPoint1, 1.0));
    o.pos = position1;
    o.depth = position1.z / position1.w;
    output.Append(o);
	
    float4 position2 = mul(projMtx, float4(input[0].controlPoint2, 1.0));
    o.pos = position2;
    o.depth = position2.z / position2.w;
    output.Append(o);
    
    float4 position3 = mul(projMtx, float4(input[0].controlPoint3, 1.0));
    o.pos = position3;
    o.depth = position3.z / position3.w;
    output.Append(o);
}