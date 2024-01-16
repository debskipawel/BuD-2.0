struct VSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float3 cameraPos : CAMERAPOS;
    float4 color : COLOR;
};

float4 main(VSOutput i) : SV_TARGET
{
    return i.color;
}