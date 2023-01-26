
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

// ----- TEXTURES -----
Texture2D transmissionMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D SSSMap : register(t2);
Texture2D roughnessMap : register(t3);
Texture2D normalMap : register(t4);
Texture2D mikroNormalMask : register(t5);
Texture2D mikroNormalMap : register(t6);
Texture2D diffuseMap : register(t7);
Texture2D ambientOcclusionMap : register(t8);
Texture2D depthMapAbsorption : register(t9);

sampler samp : register(s0);

// ----- MAP FLAGS -----
cbuffer texturesOn : register(b0)
{
	int transmissionMapOn;
	int specularMapOn;
	int SSSMapOn;
	int roughnessMapOn;
	int normalMapOn;
	int mikroNormalMaskOn;
	int mikroNormalMapOn;
	int diffuseMapOn;
	int ambientOcclusionMapOn;
	int depthMapAbsorptionOn;
};

// ----- MESH MATERIAL -----
cbuffer material : register(b1)
{
	float3 Ka; // Ambient Color
	float3 Kd; // Diffuse Color
	float3 Ks; // Specular Color
    float __padding1;
	float  Ns; // Specular Exponent
	float  Ni; // Optical Density
	float  d;  // Dissolve
	int illum; // Illumination
	
	float3 passingExpMultipilier;
};

cbuffer lightBuffer : register(b2)
{
	float4 lightPosition;
	float4 lightColor;
};

static float3 surfaceColorStatic = float3(1.0, 0, 0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 resultColor = float4(0, 0, 0, 1);
	float3 surfaceColor = surfaceColorStatic;
	
	if (diffuseMapOn)
	{
		surfaceColor = diffuseMap.Sample(samp, input.texCoords);
	}
	
	// NORMAL
	float3 normal = input.normal;
	
	//if (normalMapOn)
	//{
	//    float3 normalTex = normalMap.Sample(samp, input.texCoords);
	//    float3 bitangent = normalize(cross(input.normal, input.tangent));
	//    float3 tangent = normalize(cross(bitangent, input.normal));
	//    matrix tbn = matrix(
	//        tangent.x, bitangent.x, normal.x, 0,
	//        tangent.y, bitangent.y, normal.y, 0,
	//        tangent.z, bitangent.z, normal.z, 0,
	//        0, 0, 0, 1
	//    );
	//    normal = normalize(mul(tbn, normalTex));
	//}
	
    float3 viewVector = normalize(input.surfaceToView);
    
	float3 lightToPoint = lightPosition.xyz - input.worldPos;
    float3 lightToPointDistance = length(lightToPoint);
	float3 lightVector = lightToPoint / lightToPointDistance;
	
	float3 halfVector = normalize(viewVector + lightVector);
	// ^^ NORMAL
	
    float3 diffusedLightColor = lightColor.xyz;
	
	// DIFFUSE
	float diffuseScalar = saturate(dot(normal, lightVector));
    float3 diffuseColor = Kd * diffuseScalar * surfaceColor * diffusedLightColor;
	
	if (depthMapAbsorptionOn)
	{
		if (dot(normal, lightVector) < 0) // te z ty³u
		{
			float inDistance = depthMapAbsorption.Sample(samp, input.lightTexCoords);
			float outDistance = length(input.worldPos - lightPosition.xyz);
			float travel = outDistance - inDistance;
			
			if (travel > 0)
			{
				float3 lightPassing = exp(-travel * passingExpMultipilier);
			
				diffuseScalar = abs(dot(normal, lightVector));
                diffuseColor = Kd * lightPassing * surfaceColor * diffuseScalar * diffusedLightColor;
            }
		}
	}
	
	resultColor += float4(diffuseColor, 0);
	// ^^ DIFFUSE
	
	// SPECULAR
	float NdotH = saturate(dot(normal, halfVector));
	float specPower = pow(NdotH, Ns);
    float3 specularColor = Ks * specPower * surfaceColor;
	
	if (specularMapOn)
	{
		float3 specularTextureColor = specularMap.Sample(samp, input.texCoords);
		specularColor *= specularTextureColor;
	}

	resultColor += float4(specularColor, 0);
	// ^^ SPECULAR
	
	//if (transmissionMapOn)
	{
		float3 transmission = transmissionMap.Sample(samp, input.texCoords);
	}
	
	//if (SSSMapOn)
	{
		float3 SSS = SSSMap.Sample(samp, input.texCoords);
	}
	
	//if (roughnessMapOn)
	{
		float3 roughness = roughnessMap.Sample(samp, input.texCoords);
	}
   
	//if (mikroNormalMaskOn)
	{
		float3 mikroNormalCoords = mikroNormalMask.Sample(samp, input.texCoords);
	}
	
	//if (mikroNormalMapOn)
	{
		float3 mikroNormal = mikroNormalMap.Sample(samp, input.texCoords);
	}
   
	// AMBIENT
	float3 ambientColor = Ka * lightColor.xyz * surfaceColor;
	
	if (ambientOcclusionMapOn)
	{
		float3 ambientOcclusionTexColor = ambientOcclusionMap.Sample(samp, input.texCoords);
		ambientColor *= ambientOcclusionTexColor;
	}
	resultColor += float4(ambientColor, 0);
	// ^^Ambient
	
	return resultColor;
}