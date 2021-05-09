cbuffer SceneConstantBuffer : register(b0)
{
    // float4x4 g_mWorldViewProj;
    // float4x4 modelView;
    // float3 lightPosition;  //
    // float falloffStart;    //
    // float3 lightColor;     //
    // float falloffEnd;      //
    // float4 diffuseColor;
    // float4 ambient;
    // float4 padding1;
    // float diffuseIntensity;
    // float attenuationConst;
    // float attenuationLinear;
    // float attenuationQuad;
    // float specularIntensity;
    // float specularPower;
    // float deltaTime;
    // float padding2[5];
    float4x4 model;
    float4x4 viewProj;
    float3 cameraPosition;
    float roughness;
    float3 lightPosition1;
    float falloffStart;
    float3 lightPosition2;
    float falloffEnd;
    float3 lightColor;
    float padding0;
    float4 ambient;
    float4 diffuseAlbedo;
    float3 FresnelR0;
    float progress;
    float padding[4];
}

#include "LightHelper.hlsli"

PSInput main(float3 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput result;

    // transform to world space
    float4 positionWorld = mul(float4(position, 1), model);
    result.positionWorld = positionWorld.xyz;

    result.normal = mul(normal, (float3x3)model);
    result.uv = uv;
    
    result.positionHomo = mul(positionWorld, viewProj);

    result.cameraPosition = cameraPosition;
    result.ambient = ambient;
    
    Material material;
    material.Shininess = 1 - roughness;
    material.DiffuseAlbedo = diffuseAlbedo;
    material.FresnelR0 = FresnelR0;
    result.material = material;

    Light light1;
    light1.Position = lightPosition1;
    light1.Strength = lightColor;
    light1.FalloffStart = falloffStart;
    light1.FalloffEnd = falloffEnd;
    result.lights[0] = light1;
    
    Light light2;
    light2.Position = lightPosition2;
    light2.Strength = lightColor;
    light2.FalloffStart = falloffStart;
    light2.FalloffEnd = falloffEnd;
    result.lights[1] = light2;

    result.progress = progress;
    
    return result;
}