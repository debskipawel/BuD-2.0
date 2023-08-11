struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 worldPos : WORLD_POS;
    float2 tex : TEXCOORD;
    float depth : DEPTH;
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
    
    o.color = float(radius <= 1.0) * float4(1.0, 1.0, 1.0, 1.0);
    o.depth = i.depth + float(radius > 1.0) * 100.0;
    
    return o;
}