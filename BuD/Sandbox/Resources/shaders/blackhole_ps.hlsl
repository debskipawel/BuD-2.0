#define SQRT_27 5.19615242271f
#define PI 3.14159265359f

#define MAX_ITER 1000
#define EPS 1e-6f

#define SUCCESS   0
#define ERR_ITER  1
#define ERR_OTHER 2

struct NewtonResult
{
    float Root;
    float FunctionValue;
    float DerivativeValue;
    uint ReturnFlag;
};

struct VSOut
{
    float4 pos : SV_Position;
    float3 rayOrigin : ORIGIN;
    float3 rayDir : DIRECTION;
};

struct BlackHole
{
    float3 WorldPos;
    float Mass;
};

struct Ray
{
    float3 Origin;
    float3 Dir;
};

TextureCube colorMap : register(t0);
SamplerState colorSampler : register(s0);

cbuffer blackHole : register(b0)
{
    BlackHole blackHole;
}

float3 BlackHoleToRay(Ray ray, BlackHole hole)
{
    float3 originToHole = hole.WorldPos - ray.Origin;
    float3 shortestH2R = dot(ray.Dir, originToHole) * ray.Dir - originToHole;
    
    return shortestH2R;
}

float fun(float omega, float M, float b)
{
    return 1.0f - omega * omega * (1.0f - 2.0f * M / b * omega);
}

float dfun(float omega, float M, float b)
{
    return omega * (6.0f * M / b * omega - 2.0f);
}

NewtonResult RootFindNewton(float M, float b)
{
    NewtonResult result;
    result.Root = 1.0f;
    result.FunctionValue = fun(result.Root, M, b);
    result.DerivativeValue = dfun(result.Root, M, b);

    [loop]
    for (int i = 0; i < MAX_ITER; i++)
    {
        if (abs(result.FunctionValue) <= EPS)
        {
            result.ReturnFlag = SUCCESS;
            return result;
        }
        
        result.Root = result.Root - result.FunctionValue / result.DerivativeValue;
        result.FunctionValue = fun(result.Root, M, b);
        result.DerivativeValue = dfun(result.Root, M, b);
    }

    result.ReturnFlag = ERR_ITER;
    
    return result;
}

float Integrate(float lower, float upper, float M, float b)
{
    float h = (upper - lower) / MAX_ITER;
    float integral = 0.0f;
    
    [unroll(MAX_ITER)]
    for (int i = 0; i < MAX_ITER; i++)
    {
        integral += rsqrt(fun(lower + i * h, M, b));
    }

    return h * integral;
}

float4 main(VSOut i) : SV_TARGET
{
    float4 color;
    
    Ray ray = (Ray) 0;
    ray.Origin = i.rayOrigin;
    ray.Dir = normalize(i.rayDir);
    
    float3 blackHoleToRay = BlackHoleToRay(ray, blackHole);
    float b = length(blackHoleToRay);
    
    if (SQRT_27 * blackHole.Mass > b)
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    NewtonResult rootResult = RootFindNewton(blackHole.Mass, b);
    
    float phi = 2 * Integrate(0.0f, rootResult.Root, blackHole.Mass, b) - PI;
    
    if (isinf(phi) || isnan(phi) || abs(phi) > PI)
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    float3 outer = normalize(blackHoleToRay);
    float3 front = ray.Dir;
    
    float3 rotatedRay = cos(phi) * front - sin(abs(phi)) * outer;
    
    color = colorMap.Sample(colorSampler, rotatedRay);
    
    return color;
}