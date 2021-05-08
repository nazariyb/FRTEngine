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
    float3 lightPosition;
    float falloffStart;
    float3 lightColor;
    float falloffEnd;
    float4 ambient;
    float4 diffuseAlbedo;
    float3 FresnelR0;
    float progress;
    float padding[8];
}

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

    Light light;
    light.Position = lightPosition;
    light.Strength = lightColor;
    light.FalloffStart = falloffStart;
    light.FalloffEnd = falloffEnd;
    result.light = light;

    result.progress = progress;
    
    return result;
}