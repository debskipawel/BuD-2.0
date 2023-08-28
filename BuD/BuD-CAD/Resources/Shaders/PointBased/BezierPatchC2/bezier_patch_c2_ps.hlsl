struct DSOutput
{
	float4 position : SV_POSITION;
	float3 worldPosition : WORLD_POS;
    float3 normal : NORMAL;
    float depth : DEPTH;
    float3 color : COLOR;
    float2 texCoords : TEX_COORDS;
};

float4 main(DSOutput i) : SV_TARGET
{
	return float4(i.color, 1.0f);
}