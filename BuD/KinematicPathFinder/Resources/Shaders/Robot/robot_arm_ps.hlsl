struct VSOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

float4 main(VSOutput i) : SV_TARGET
{
	return float4(i.color, 1.0f);
}