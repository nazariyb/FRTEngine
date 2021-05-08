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

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

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

#pragma region waves
    const float t = InverseLerp(input.progress, input.progress + 0.1, input.uv.y);
    // const float t = abs(frac(input.uv.x * 5) * 2 - 1);
    // const float offsetX = cos((input.uv.y) * TAU * 3.0) * 0.02;
    // const float t = cos((input.uv.x + offsetX + input.deltaTime * 3.0) * TAU * 4) * 0.9 + 0.5;
    const float4 outputColor = lerp(float4(0.38, 0.66, 0.49, 1), float4(0.92, 0.32, 0.38, 1), saturate(t));
    
    // return saturate( outputColor );
#pragma endregion waves

    // input.material.DiffuseAlbedo = float4(0.5f, 0.5f, 0.4f, 1.0f);
    input.material.DiffuseAlbedo = outputColor;
    float4 ambient = input.ambient * input.material.DiffuseAlbedo;

    float3 shadowFactor = 1;

    float3 pointLight = ComputePointLight(input.light, input.material, input.positionWorld, input.normal, toCamera);
    float4 light = float4(pointLight, 0);

    float4 litColor = ambient + light * 0.9;

    litColor.a = input.material.DiffuseAlbedo.a;

    
    return litColor;
}
