#include "LightHelper.hlsli"

struct PSInput
{
    float4 positionHomo : SV_POSITION;
    float3 positionWorld : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 cameraPosition : CAMERA_POSITION;
    float4 ambient : AMBIENT;
    float progress : PROGRESS;
    Material material : MATERIAL;
    Light light : LIGHT;
};

float InverseLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

#define TAU 6.28318530718

float4 Blend(float4 color1, float4 a, float4 color2, float4 b)
{
    return color1 * a + color2 * b;
}

float4 Additive(float4 color1, float4 color2)
{
    return Blend(color1, 1.0f, color2, 1.0f);
}

float4 Multiply(float4 color1, float4 color2)
{
    return Blend(color1, color2, color2, 0.f);
}

float4 main(PSInput input) : SV_TARGET
{
    // it can be unnormalized by interpolating
    input.normal = normalize(input.normal);

    float3 toCamera = normalize(input.cameraPosition - input.positionWorld);

    input.material.DiffuseAlbedo = float4(0.97f, 0.7f, 0.f, 1.0f);
    float4 ambient = input.ambient * input.material.DiffuseAlbedo;

    float3 shadowFactor = 1;

    float3 pointLight = ComputePointLight(input.light, input.material, input.positionWorld, input.normal, toCamera);
    float4 light = float4(pointLight, 0);

    float4 litColor = ambient + light;

    litColor.a = input.material.DiffuseAlbedo.a;

    return litColor;

}
