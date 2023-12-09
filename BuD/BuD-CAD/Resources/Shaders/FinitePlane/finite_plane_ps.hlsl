struct VSOutput
{
    float4 position : SV_POSITION;
    float3 dU : DERIVATIVE_U;
    float3 dV : DERIVATIVE_V;
    float2 uvCoords : UV_COORDS;
};

float4 main(VSOutput i) : SV_TARGET
{
	return float4(0.2f, 0.2f, 0.2f, 1.0f);
}