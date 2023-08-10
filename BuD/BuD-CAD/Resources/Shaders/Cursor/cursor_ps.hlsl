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
	int appState : APP_STATE;
	int axisLock : AXIS_LOCK;
};

struct PSOutput
{
	float4 color : SV_Target;
	float depth : SV_Depth;
};

float3 AxisLockColorScale(int axisLock)
{
	float3 scale;
	float greyFactor = 0.3;
	
	switch (axisLock)
	{
		case 1:
            scale = float3(1.0, greyFactor, greyFactor);
			break;
		case 2:
            scale = float3(greyFactor, 1.0, greyFactor);
			break;
		case 3:
            scale = float3(greyFactor, greyFactor, 1.0);
			break;
		default:
			scale = float3(1.0, 1.0, 1.0);
			break;
	}
	
	return scale;
}

float3 IdleCursor(float3 tex, int axisLock)
{
	float3 cursorBaseColor = float3(tex.y + tex.z < CURSOR_WIDTH, tex.x + tex.z < CURSOR_WIDTH, tex.y + tex.x < CURSOR_WIDTH);
	float3 middleDot = (dot(tex, tex) <= 0.05) * float3(1.0, 1.0, 1.0);
	
	return cursorBaseColor + middleDot;
}

float3 ArrowCursor(float3 tex, int axisLock)
{
	float maxCoord = max(tex.x, max(tex.y, tex.z));
	float3 scaleFactor = maxCoord * float3(maxCoord == tex.x, maxCoord == tex.y, maxCoord == tex.z);
	
	float arrowStartingParameter = 0.8;
	float distanceFromAxis = 
		(maxCoord > arrowStartingParameter) * (1.0 - maxCoord) / (1.0 - arrowStartingParameter) * ARROW_WIDTH 
		+ (maxCoord <= arrowStartingParameter) * CURSOR_WIDTH;
	
	float3 cursorBaseColor = float3(tex.y + tex.z < distanceFromAxis, tex.x + tex.z < distanceFromAxis, tex.y + tex.x < distanceFromAxis);
	float3 middleDot = (dot(tex, tex) <= 0.05) * float3(1.0, 1.0, 1.0);
	
	float3 axisLockScale = AxisLockColorScale(axisLock);
	
	return axisLockScale * cursorBaseColor + middleDot;
}

float3 RotationCursor(float3 tex, int axisLock)
{
	float rotationCirclesStart = 0.5;
	float rotationCirclesEnd = 0.6;
	
	float3 cursorBaseColor = float3(tex.y + tex.z < CURSOR_WIDTH, tex.x + tex.z < CURSOR_WIDTH, tex.y + tex.x < CURSOR_WIDTH);
	float3 middleDot = (dot(tex, tex) <= 0.05) * float3(1.0, 1.0, 1.0);
	
	float3 rotationCircles = float3(pow(tex.y, 2) + pow(tex.z, 2), pow(tex.x, 2) + pow(tex.z, 2), pow(tex.y, 2) + pow(tex.x, 2));
    float3 rotationCirclesColorScale = (cursorBaseColor.x + cursorBaseColor.y + cursorBaseColor.z) == 0.0
		&& tex == 0.0 
		&& rotationCircles >= pow(rotationCirclesStart, 2) && rotationCircles <= pow(rotationCirclesEnd, 2);
	
	float3 axisLockScale = AxisLockColorScale(axisLock);
	
	return axisLockScale * (cursorBaseColor + rotationCirclesColorScale) + middleDot;
}

float3 ScaleCursor(float3 tex, int axisLock)
{
	float scaleRectanglesStart = 0.5;
	float scaleRectanglesEnd = 0.6;
	
	float3 cursorBaseColor = float3(tex.y + tex.z < CURSOR_WIDTH, tex.x + tex.z < CURSOR_WIDTH, tex.y + tex.x < CURSOR_WIDTH);
	float3 middleDot = (dot(tex, tex) <= 0.05) * float3(1.0, 1.0, 1.0);
	
	float3 scaleRectangles = float3(max(tex.y, tex.z), max(tex.x, tex.z), max(tex.y, tex.x));
    float3 scaleRectanglesColorScale = (cursorBaseColor.x + cursorBaseColor.y + cursorBaseColor.z) == 0.0 
		&& tex == 0.0 
		&& scaleRectangles > scaleRectanglesStart && scaleRectangles <= scaleRectanglesEnd;
	
	float3 axisLockScale = AxisLockColorScale(axisLock);
	
	return axisLockScale * (cursorBaseColor + scaleRectanglesColorScale) + middleDot;
}

PSOutput main(VSOutput i)
{
	PSOutput o = (PSOutput) 0;
	
	float3 color;
	
	switch (i.appState)
	{
		case 1:
			color = IdleCursor(i.tex, i.axisLock);
			break;
		case 2:
			color = ArrowCursor(i.tex, i.axisLock);
			break;
		case 3:
			color = RotationCursor(i.tex, i.axisLock);
			break;
		case 4:
			color = ScaleCursor(i.tex, i.axisLock);
			break;
		default:
			break;
	}
	
	float4 screenSpacePosition = mul(projMtx, mul(viewMtx, float4(i.worldPos, 1.0)));
	float standardDepth = screenSpacePosition.z / screenSpacePosition.w;
	float colorNotOutput = (color.x + color.y + color.z) < EPSILON;
	
	o.color = float4(color, 1.0);
	o.depth = standardDepth + colorNotOutput * INFINITY;
	
	return o;
}