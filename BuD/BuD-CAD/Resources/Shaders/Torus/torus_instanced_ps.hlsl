struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

float4 main(PSInput i) : SV_TARGET
{
	return float4(normalize(i.normal), 1.0f);
}