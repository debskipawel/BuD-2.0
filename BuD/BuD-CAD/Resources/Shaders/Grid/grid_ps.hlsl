#define INFINITY 1000.0f

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 eyePos : VIEW_POS;
    float3 eyeDir : VIEW_DIR;
};

struct PSOutput
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

float CalculateDepth(float3 rayOrigin, float3 rayDirection)
{
    float eps = 0.001;
    
    float t = -(rayOrigin.z / rayDirection.z);
    
    return abs(rayDirection.z) < eps || t < 0 ? INFINITY : t;
}

PSOutput main(VSOutput i)
{
    PSOutput o = (PSOutput) 0;
    
    o.depth = CalculateDepth(i.eyePos, normalize(i.eyeDir));
    o.color = float4(o.depth, 0.0, 0.0, 1.0);
    
	return o;
}