#define TESSELLATION_LIMIT 64

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 uvRange : UV_RANGE;
    float2 tessFactor : TESSFACTOR;
    float3 materialSize : MATERIAL_SIZE;
    float3 materialPosition : MATERIAL_POSITION;
    uint instanceId : SV_InstanceID;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 worldPos : WORLDPOS;
    float2 rangeU : RANGE_U;
    float2 rangeV : RANGE_V;
    float2 tessFactor : TESSFACTOR;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    // rotate in x by 90 degrees (quad is nominally defined on the XY plane, we want it on the XZ plane)
    float3 localPosition = 0.5f * i.materialSize * float3(i.position.x, i.position.z, i.position.y);
    float3 worldPosition = localPosition + i.materialPosition;
    
    o.worldPos = worldPosition;
    o.pos = float4(worldPosition, 1.0);
    
    o.rangeU = i.uvRange.xy;
    o.rangeV = i.uvRange.zw;
    
    o.tessFactor = i.tessFactor;
    
    return o;
}