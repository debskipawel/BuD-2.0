
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORDS;
};

Texture2D bakedLighting     : register(t0);
SamplerState samplerState   : register(s0);

float4 main(VSOutput i) : SV_TARGET
{
    float3 resultColor = { 0.0f, 0.0f, 0.0f };
    
    float2 tex = i.tex;
    float2 d = 5 * float2(ddx(i.tex).x, ddy(i.tex).y);
    
    float3 weights[13] =
    {
        { 0.220441, 0.437000, 0.635000 },
        { 0.076356, 0.064487, 0.039097 },
        { 0.116515, 0.103222, 0.064912 },
        { 0.064844, 0.086388, 0.062272 },
        { 0.131798, 0.151695, 0.103676 },
        { 0.025690, 0.042728, 0.033003 },
        { 0.048593, 0.064740, 0.046131 },
        { 0.048092, 0.003042, 0.000400 },
        { 0.048845, 0.005406, 0.001222 },
        { 0.051322, 0.006034, 0.001420 },
        { 0.061428, 0.009152, 0.002511 },
        { 0.030936, 0.002868, 0.000652 },
        { 0.073580, 0.023239, 0.009703 },
    };
    
    float2 samples[13] =
    {
        {  0.000000,  0.000000 },
        {  1.633992,  0.036795 },
        {  0.177801,  1.717593 },
        { -0.194906,  0.091094 },
        { -0.239737, -0.220217 },
        { -0.003530, -0.118219 },
        {  1.320107, -0.181542 },
        {  5.970690,  0.253378 },
        { -1.089250,  4.958349 },
        { -4.015465,  4.156699 },
        { -4.063099, -4.110150 },
        { -0.638605, -6.297663 },
        {  2.542348, -3.245901 },
    };
    
    resultColor += weights[0] * bakedLighting.Sample(samplerState, tex + d * samples[0]).rgb;
    resultColor += weights[1] * bakedLighting.Sample(samplerState, tex + d * samples[1]).rgb;
    resultColor += weights[2] * bakedLighting.Sample(samplerState, tex + d * samples[2]).rgb;
    resultColor += weights[3] * bakedLighting.Sample(samplerState, tex + d * samples[3]).rgb;
    resultColor += weights[4] * bakedLighting.Sample(samplerState, tex + d * samples[4]).rgb;
    resultColor += weights[5] * bakedLighting.Sample(samplerState, tex + d * samples[5]).rgb;
    resultColor += weights[6] * bakedLighting.Sample(samplerState, tex + d * samples[6]).rgb;
    resultColor += weights[7] * bakedLighting.Sample(samplerState, tex + d * samples[7]).rgb;
    resultColor += weights[8] * bakedLighting.Sample(samplerState, tex + d * samples[8]).rgb;
    resultColor += weights[9] * bakedLighting.Sample(samplerState, tex + d * samples[9]).rgb;
    resultColor += weights[10] * bakedLighting.Sample(samplerState, tex + d * samples[10]).rgb;
    resultColor += weights[11] * bakedLighting.Sample(samplerState, tex + d * samples[11]).rgb;
    resultColor += weights[12] * bakedLighting.Sample(samplerState, tex + d * samples[12]).rgb;
    
    return float4(resultColor, 1.0);
}