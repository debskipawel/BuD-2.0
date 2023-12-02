cbuffer mvp : register(b0)
{
    float aspectRatio;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 insPosition : INS_POSITION;
    float2 insOrientation : INS_ORIENTATION;
    float insLength : INS_LENGTH;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    float2 orthogonal = float2(i.insOrientation.y, -i.insOrientation.x);
    float armWidth = 0.02f;
    
    float2 worldPosition = i.insPosition + 0.5f * i.position.y * i.insLength * i.insOrientation + 0.5f * i.position.x * armWidth * orthogonal;
    
    o.position = float4(worldPosition.x / aspectRatio, worldPosition.y, 0.0f, 1.0f);
    
    return o;
}