cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

float4 main(float3 pos : POSITION) : SV_POSITION
{    
    return mul(projMtx, mul(viewMtx, float4(pos, 1.0)));
}