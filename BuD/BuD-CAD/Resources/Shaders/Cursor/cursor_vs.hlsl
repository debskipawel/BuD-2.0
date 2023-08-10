cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 insPosition : INS_POSITION;
    int appState : INS_APP_STATE;
    int axisLock : INS_AXIS_LOCK;
    uint instanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD;
    float3 worldPos : WORLD_POS;
    int appState : APP_STATE;
    int axisLock : AXIS_LOCK;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    o.tex = 0.5 * (i.position + 1.0);
    o.worldPos = o.tex + i.insPosition;
    o.position = mul(projMtx, mul(viewMtx, float4(o.worldPos, 1.0)));
    o.appState = i.appState;
    o.axisLock = i.axisLock;
    
	return o;
}