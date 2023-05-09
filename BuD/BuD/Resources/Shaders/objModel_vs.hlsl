struct VSObjModelIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoords : TEXCOORDS;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 worldPos : WORLD_POS;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 surfaceToView : SURFACETOVIEW;
    float2 texCoords : TEXCOORDS;
    float2 lightTexCoords : TEXCOORDS1;
};

cbuffer cbWorld : register(b0)
{
    matrix projectionMatrix;
    matrix viewMatrix;
    matrix invViewMatrix;
    matrix worldMatrix;
};

cbuffer cbDepthAbsorption : register(b1)
{
    matrix lightProjectionMatrix;
    matrix lightViewMatrix;
};

VSOutput main(VSObjModelIn input)
{
    VSOutput o;
    float4 worldPosition = mul(worldMatrix, float4(input.pos, 1.0));
    o.worldPos = worldPosition.xyz;
    o.pos = mul(projectionMatrix, mul(viewMatrix, worldPosition));
    
    float4 cameraPosition = mul(invViewMatrix, float4(0, 0, 0, -1));
    o.surfaceToView = cameraPosition.xyz - worldPosition.xyz;
    
    o.normal = normalize(mul(worldMatrix, float4(input.normal, 0.0f)));
    o.tangent = normalize(input.tangent); //normalize(mul(worldMatrix, float4(input.tangent, 0.0f)));

    o.texCoords = input.texCoords * -1 + 1;
   
    float4 lightSpacePos = mul(lightViewMatrix, worldPosition);
    float4 lightPerspectivePosition = mul(lightProjectionMatrix, lightSpacePos);
    o.lightTexCoords = (lightPerspectivePosition.xy / lightPerspectivePosition.w + 1) / 2.0f;
    o.lightTexCoords.y = (o.lightTexCoords.y * -1) + 1;
    return o;
}