cbuffer mvp : register(b0)
{
    matrix projMtx;
}

struct VSOutput
{
    float4 position : SV_POSITION;
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

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(32)]
void main(
	point VSOutput input[1], 
	inout LineStream<GSOutput> output
)
{
    float4 controlPoints[16] =
    {
        mul(projMtx, float4(input[0].controlPoint0,  1.0)),
        mul(projMtx, float4(input[0].controlPoint1,  1.0)),
        mul(projMtx, float4(input[0].controlPoint2,  1.0)),
        mul(projMtx, float4(input[0].controlPoint3,  1.0)),
        mul(projMtx, float4(input[0].controlPoint4,  1.0)),
        mul(projMtx, float4(input[0].controlPoint5,  1.0)),
        mul(projMtx, float4(input[0].controlPoint6,  1.0)),
        mul(projMtx, float4(input[0].controlPoint7,  1.0)),
        mul(projMtx, float4(input[0].controlPoint8,  1.0)),
        mul(projMtx, float4(input[0].controlPoint9,  1.0)),
        mul(projMtx, float4(input[0].controlPoint10, 1.0)),
        mul(projMtx, float4(input[0].controlPoint11, 1.0)),
        mul(projMtx, float4(input[0].controlPoint12, 1.0)),
        mul(projMtx, float4(input[0].controlPoint13, 1.0)),
        mul(projMtx, float4(input[0].controlPoint14, 1.0)),
        mul(projMtx, float4(input[0].controlPoint15, 1.0)),
    };
    
    // lines right
    //[unroll]
	for (uint i = 0; i < 4; i++)
	{
        uint startIndex = 4 * i;
        
		GSOutput element;
        element.pos = controlPoints[startIndex];
		output.Append(element);
        
        element.pos = controlPoints[startIndex + 1];
        output.Append(element);
        
        element.pos = controlPoints[startIndex + 2];
        output.Append(element);

        element.pos = controlPoints[startIndex + 3];
        output.Append(element);
        
        output.RestartStrip();
    }
    
    // lines down
    //[unroll]
    for (uint j = 0; j < 4; j++)
    {
        uint startIndex = j;
        
        GSOutput element;
        element.pos = controlPoints[startIndex];
        output.Append(element);
        
        element.pos = controlPoints[startIndex + 4];
        output.Append(element);
        
        element.pos = controlPoints[startIndex + 8];
        output.Append(element);

        element.pos = controlPoints[startIndex + 12];
        output.Append(element);
        
        output.RestartStrip();
    }
}