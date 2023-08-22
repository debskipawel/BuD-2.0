struct DSOutput
{
	float4 position : SV_POSITION;
	float3 worldPosition : WORLD_POS;
    float3 normal : NORMAL;
	float3 color : COLOR;
};

float4 main(DSOutput i) : SV_TARGET
{
    return float4(i.color, 1.0f);
}