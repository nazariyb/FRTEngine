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

float4 main(PSInput input) : SV_TARGET
{
    // it can be unnormalized by interpolating
    input.normal = normalize(input.normal);

    float3 toCamera = normalize(input.cameraPosition - input.positionWorld);

    input.material.DiffuseAlbedo = float4(0.07f, 0.07f, 0.12f, 1.0f);
    float4 ambient = input.ambient * input.material.DiffuseAlbedo;

    float3 shadowFactor = 1;

    float3 pointLight = ComputePointLight(input.light, input.material, input.positionWorld, input.normal, toCamera);
    float4 light = float4(pointLight, 0);

    float4 litColor = ambient * 2 + light * 1.2;

    litColor.a = input.material.DiffuseAlbedo.a;

    return litColor;
}
