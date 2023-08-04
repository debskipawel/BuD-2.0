struct VSInput
{
    float4 pos : POSITION;
    float3 insPos : INS_POSITION;
    float3 insOrientation : INS_ORIENTATION;
    float2 radius : INS_RADIUS;
    uint2 segments : INS_SEGMENTS;
    uint instanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 insPos : INS_POSITION;
    float3 insOrientation : INS_ORIENTATION;
    float2 radius : INS_RADIUS;
    uint2 segments : INS_SEGMENTS;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    o.pos = i.pos;
    o.insPos = i.insPos;
    o.insOrientation = i.insOrientation;
    o.radius = i.radius;
    o.segments = i.segments;
    
	return o;
}