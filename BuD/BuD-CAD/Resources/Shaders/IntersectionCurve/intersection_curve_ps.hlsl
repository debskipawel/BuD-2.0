struct DSOutput
{
	float4 position : SV_POSITION;
	float3 worldPosition : WORLD_POS;
};

float4 main(DSOutput i) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}