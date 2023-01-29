struct VSOutput
{
    float4 pos : SV_POSITION;
    float dist : TEXCOORD0; // distance from light 
};

float4 main(VSOutput input) : SV_Target
{
    return float4(input.dist, 0, 0, 1);
}