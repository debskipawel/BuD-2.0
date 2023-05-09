struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORDS;
};

VSOutput main(float2 pos : POSITION)
{
    VSOutput o = (VSOutput) 0;
    
    o.tex = (pos + float2(1.0, 1.0)) * 0.5;
    o.tex.y = 1 - o.tex.y;
    o.pos = float4(pos, 0.0f, 1.0f);
    
	return o;
}