cbuffer mvp : register(b0)
{
    float aspectRatio;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 insPosition : INS_POSITION;
    float2 insOrientation : INS_ORIENTATION;
    float insLength : INS_LENGTH;
    float3 insColor : INS_COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float2 orthogonal = float2(i.insOrientation.y, -i.insOrientation.x);
    float armWidth = 0.02f;
    
    float2 worldPosition = i.insPosition.xy + 0.5f * i.position.y * i.insLength * i.insOrientation + 0.5f * i.position.x * armWidth * orthogonal;
    
    o.position = float4(worldPosition.x / aspectRatio, worldPosition.y, i.insPosition.z, 1.0f);
    o.color = i.insColor;
    
    return o;
}