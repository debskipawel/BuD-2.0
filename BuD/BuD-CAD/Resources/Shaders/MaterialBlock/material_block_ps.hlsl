
Texture2D surfaceTexture : register(t0);
SamplerState surfaceSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct DSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float2 uv : TEXCOORDS;
};

struct PSOutput
{
    float4 color : SV_Target;
};

PSOutput main(DSOutput i)
{
    PSOutput o = (PSOutput) 0;
    
    float3 lightPos = float3(-4.0f, 10.0f, 0.0f);
    
    float3 N = normalize(cross(ddy(i.worldPos), ddx(i.worldPos)));
    float3 L = float3(0.0f, 1.0f, 0.0f);
    
    float ambient = 0.0f;
    float diffuse = 0.6f * dot(L, N);
    
    float3 albedo = surfaceTexture.Sample(surfaceSampler, i.uv).xyz;
    
    float3 color = ambient * albedo + diffuse * albedo;
    
    o.color = float4(color, 1.0f);
    
    return o;
}