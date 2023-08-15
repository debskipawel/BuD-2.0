struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 worldPos : WORLD_POS;
    float2 tex : TEXCOORD;
    float depth : DEPTH;
    float3 color : INS_COLOR;
};

struct PSOutput
{
    float4 color : SV_TARGET;
    float depth : SV_DEPTH;
};

PSOutput main(VSOutput i)
{
    PSOutput o = (PSOutput) 0;
    
    float radius = dot(i.tex, i.tex);
    
    float borderStart = 0.75, borderEnd = 1.0;
    o.color = float(radius <= borderStart) * float4(i.color, 1.0) + (radius > borderStart && radius <= borderEnd) * float4(1.0, 1.0, 1.0, 1.0);
    o.depth = i.depth + float(radius > 1.0) * 100.0;
    
    return o;
}