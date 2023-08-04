struct VSOutput
{
    float4 pos : SV_Position;
    float3 position : POSITION;
    float3 normal : NORMAL;
};

float4 main(VSOutput i) : SV_TARGET
{
    return float4(normalize(i.normal), 1.0f);
}