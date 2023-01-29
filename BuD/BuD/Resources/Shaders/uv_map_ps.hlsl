
struct VSOutput
{
    float4 pos : SV_Position;
    float4 worldPos : WORLD_POS;
    float3 normal : NORMAL;
    float3 viewVector : VIEW_VECTOR;
    float2 tex : TEXCOORDS;
};

Texture2D albedoMap             : register(t0);
Texture2D ambientOcclusionMap   : register(t1);
Texture2D normalMap             : register(t2);
Texture2D microNormalMap        : register(t3);
Texture2D specularMap           : register(t4);

SamplerState samplerState : register(s0);

cbuffer material : register(b0)
{
    float3 Ka; // Ambient Color
    float3 Kd; // Diffuse Color
    float3 Ks; // Specular Color
    float __padding1;
    float Ns; // Specular Exponent
    float Ni; // Optical Density
    float d; // Dissolve
    int illum; // Illumination
};

cbuffer lightBuffer : register(b1)
{
    float4 lightPosition;
    float4 lightColor;
};

float4 main(VSOutput o) : SV_TARGET
{
    float3 finalColor = { 0.0f, 0.0f, 0.0f };
    
    float3 pointToLight = lightPosition.xyz - o.worldPos.xyz;
    float3 diffusedLightColor = lightColor.xyz / max(dot(pointToLight, pointToLight), 1);
    
    float3 V = normalize(o.viewVector);
    float3 N = normalize(o.normal);
    float3 L = normalize(lightPosition.xyz - o.worldPos.xyz);
    
    float3 H = normalize(V + L);
    
    float3 surfaceColor = albedoMap.Sample(samplerState, o.tex).xyz;
    
    // AMBIENT
    float3 ambientColor = Ka * diffusedLightColor * surfaceColor;
	
    float3 ambientOcclusionTexColor = ambientOcclusionMap.Sample(samplerState, o.tex);
    ambientColor *= ambientOcclusionTexColor;
    
    finalColor += ambientColor;
    
    // DIFFUSE
    float diffuseScalar = saturate(dot(N, L));
    float3 diffuseColor = Kd * diffuseScalar * surfaceColor * diffusedLightColor;
	
    finalColor += diffuseColor;
    
    // SPECULAR
    float NH = saturate(dot(N, H));
    float specCoeff = pow(NH, Ns);
    float3 specularMapColor = specularMap.Sample(samplerState, o.tex);
    float3 specularColor = Ks * specCoeff * surfaceColor * specularMapColor;
    
    finalColor += specularColor;
    
	return float4(finalColor, 1.0f);
}