struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 color : INS_COLOR;
};

float4 main(PSInput i) : SV_TARGET
{
	return float4(i.color, 1.0f);
}