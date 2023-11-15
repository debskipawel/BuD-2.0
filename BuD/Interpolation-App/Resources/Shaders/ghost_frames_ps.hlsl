#define INFINITY 100.0f
#define EPSILON 0.01f

#define CURSOR_WIDTH 0.03f
#define ARROW_WIDTH 0.15f

cbuffer mvp : register(b0)
{
    matrix viewMtx;
    matrix projMtx;
}

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD;
    float3 worldPos : WORLD_POS;
};

struct PSOutput
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

float3 ArrowCursor(float3 tex)
{
    float maxCoord = max(tex.x, max(tex.y, tex.z));
    float3 scaleFactor = maxCoord * float3(maxCoord == tex.x, maxCoord == tex.y, maxCoord == tex.z);
	
    float arrowStartingParameter = 0.8;
    float distanceFromAxis =
		(maxCoord > arrowStartingParameter) * (1.0 - maxCoord) / (1.0 - arrowStartingParameter) * ARROW_WIDTH
		+ (maxCoord <= arrowStartingParameter) * CURSOR_WIDTH;
	
    float3 cursorBaseColor = float3(tex.y + tex.z < distanceFromAxis, tex.x + tex.z < distanceFromAxis, tex.y + tex.x < distanceFromAxis);
    float3 middleDot = (dot(tex, tex) <= 0.05) * float3(1.0, 1.0, 1.0);
	
    return cursorBaseColor + middleDot;
}

PSOutput main(VSOutput i)
{
    PSOutput o = (PSOutput) 0;
	
    float3 color = ArrowCursor(i.tex);
	
    float4 screenSpacePosition = mul(projMtx, mul(viewMtx, float4(i.worldPos, 1.0)));
    float standardDepth = screenSpacePosition.z / screenSpacePosition.w;
    float colorNotOutput = (color.x + color.y + color.z) < EPSILON;
	
    float3 tint = float3(0.3f, 0.3f, 0.6f);
    
    o.color = float4(tint * (color + tint), 1.0);
    o.depth = standardDepth + colorNotOutput * INFINITY + 0.0001f;
	
    return o;
}