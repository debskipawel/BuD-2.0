
struct VSOutput
{
    float4 pos : SV_Position;
    float4 worldPos : WORLD_POS;
    float3 normal : NORMAL;
    float3 viewVector : VIEW_VECTOR;
    float2 tex : TEXCOORDS;
    float2 lightTex : TEXCOORDS1;
};

Texture2D albedoMap : register(t0);
Texture2D ambientOcclusionMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D microNormalMap : register(t3);
Texture2D specularMap : register(t4);
Texture2D lightMap : register(t5);

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
    
    float3 passingExpMultiplier;
};

cbuffer lightBuffer : register(b1)
{
    float4 lightPosition;
    float4 lightColor;
};

float4 main(VSOutput o) : SV_TARGET
{
    float3 finalColor = { 0.0f, 0.0f, 0.0f };
    
    float pointToLightDistance = length(lightPosition.xyz - o.worldPos.xyz);
    float distanceDiffusion = 1.0f / max(pointToLightDistance * pointToLightDistance, 1);
    
    float3 V = normalize(-o.viewVector);
    float3 N = normalize(o.normal);
    float3 L = normalize(lightPosition.xyz - o.worldPos.xyz);
    
    float3 H = normalize(V + L);
    
    float3 surfaceColor = albedoMap.Sample(samplerState, o.tex).xyz;
    
    // AMBIENT
    float3 ambientOcclusionTexColor = ambientOcclusionMap.Sample(samplerState, o.tex);
    float3 ambientColor = 0.5 * Ka * ambientOcclusionTexColor;
    
    finalColor += ambientColor;
    
    float lightHitDistance = lightMap.Sample(samplerState, o.lightTex);
    
    // DIFFUSE
    float diffuseScalar = saturate(dot(N, L));
    float3 diffuseColor = Kd * diffuseScalar * distanceDiffusion;
	
    if (dot(N, L) < 0)
    {
        float inDistance = lightHitDistance;
        float outDistance = pointToLightDistance;
        float travel = outDistance - inDistance;
			
        if (travel > 0)
        {
            float3 lightPassing = exp(-travel * passingExpMultiplier);
			
            diffuseScalar = abs(dot(N, L));
            diffuseColor = Kd * lightPassing * diffuseScalar * distanceDiffusion;
        }
    }
    else if (pointToLightDistance > lightHitDistance + 0.001)
    {
        diffuseColor = float3(0, 0, 0);
    }
        
    finalColor += diffuseColor;
    
    // SPECULAR
    float NH = saturate(dot(N, H));
    float specCoeff = pow(NH, Ns);
    float3 specularMapColor = specularMap.Sample(samplerState, o.tex);
    float3 specularColor = 0.5 * Ks * specCoeff * specularMapColor * distanceDiffusion;
    
    finalColor += specularColor;
    
    return float4(finalColor, 1.0f);
}