struct VSInput
{
    float4 pos : POSITION;
    float4 modelRowX : INS_MODEL_X;
    float4 modelRowY : INS_MODEL_Y;
    float4 modelRowZ : INS_MODEL_Z;
    float4 modelRowW : INS_MODEL_W;
    float2 radius : INS_RADIUS;
    uint2 segments : INS_SEGMENTS;
    uint instanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 pos : SV_Position;
    matrix model : INS_MODEL;
    float2 radius : INS_RADIUS;
    uint2 segments : INS_SEGMENTS;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    o.pos = i.pos;
    o.model = transpose(float4x4(i.modelRowX, i.modelRowY, i.modelRowZ, i.modelRowW));
    o.radius = i.radius;
    o.segments = i.segments;
    
	return o;
}