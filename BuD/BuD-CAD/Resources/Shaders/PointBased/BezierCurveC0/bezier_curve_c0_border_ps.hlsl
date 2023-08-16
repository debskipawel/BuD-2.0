struct PSOutput
{
    float4 color : SV_Target;
};

struct GSOutput
{
    float4 pos : SV_POSITION;
    float depth : DEPTH;
};

PSOutput main(GSOutput i)
{
    PSOutput o = (PSOutput) 0;
    
    o.color = float4(0.5, 1.0, 0.2, 1.0);
    
	return o;
}