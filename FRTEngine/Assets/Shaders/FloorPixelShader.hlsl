#include "LightHelper.hlsli"


float4 main(PSInput input) : SV_TARGET
{
    // it can be unnormalized by interpolating
    input.normal = normalize(input.normal);

    float3 toCamera = normalize(input.cameraPosition - input.positionWorld);

    input.material.DiffuseAlbedo = float4(0.07f, 0.07f, 0.12f, 1.0f);
    float4 ambient = input.ambient * input.material.DiffuseAlbedo;

    float3 shadowFactor = 1;

    float3 pointLights[2];
    pointLights[0] = ComputePointLight(input.lights[0], input.material, input.positionWorld, input.normal, toCamera);
    pointLights[1] = ComputePointLight(input.lights[1], input.material, input.positionWorld, input.normal, toCamera);

    float4 light = float4(pointLights[0] + pointLights[1], 0);

    float4 litColor = ambient * 2 + light * 1.2;

    litColor.a = input.material.DiffuseAlbedo.a;

    return litColor;
}
