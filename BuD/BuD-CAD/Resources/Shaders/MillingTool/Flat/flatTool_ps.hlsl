struct VSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float3 normal : NORMAL;
    float3 viewVector : VIEW;
    float2 texCoords : TEXCOORDS;
};

struct PSOutput
{
    float4 color : SV_TARGET;
};

PSOutput main(VSOutput i)
{
    float3 LIGHT_POS = float3(20.0f, 20.0f, -20.0f);
    float3 SURFACE_COLOR = float3(0.3, 0.3, 0.3);
    
    PSOutput o = (PSOutput) 0;
    
    float3 N = normalize(i.normal);
    float3 L = normalize(LIGHT_POS - i.worldPos);
    float3 V = normalize(i.viewVector);
    float3 H = normalize(L + V);
    
    float ambient = 0.1;
    float diffuse = 0.65f;
    float specular = 0.25f;
    
    float3 ambientColor = ambient * SURFACE_COLOR;
    float3 diffuseColor = diffuse * saturate(dot(N, L)) * SURFACE_COLOR;
    float3 specularColor = pow(saturate(dot(N, H)), specular) * SURFACE_COLOR;
    
    o.color = float4(ambientColor + diffuseColor + specularColor, 1.0);
    
    return o;
}