struct VSInput
{
    float4 pos : POSITION;
};

struct VSOutput
{
    float4 pos : SV_Position;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    o.pos = i.pos;
    
	return o;
}