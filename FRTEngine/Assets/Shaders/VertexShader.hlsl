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
    float padding[9];
}

#include "LightHelper.hlsli"

struct PSInput
{
    // float4 position : SV_POSITION;
    // float4 positionCam : POSITION_CAM;
    // float2 uv : TEXCOORD;
    // float4 normal : NORMAL;
    // float4 ambient : AMBIENT;
    // float3 lightPosition : LIGHT_POSITION; //
    // float falloffStart : FALLOFF_START;    //
    // float3 lightColor : LIGHT_COLOR;       //
    // float falloffEnd : FALLOFF_END;        //
    // float4 diffuseColor : DIFFUSE_COLOR;
    // float diffuseIntensity : DIFFUSE_INTENSITY;
    // float attenuationConst : ATTENUATION;
    // float attenuationLinear : ATTENUATION_LINEAR;
    // float attenuationQuad : ATTENUATION_QUAD;
    // float specularIntensity : SPECULAR_INTENSITY;
    // float specularPower : SPECULAR_POWER;
    // float deltaTime : DELTA_TIME;
    float4 positionHomo : SV_POSITION;
    float3 positionWorld : POSITION;
    float3 normal : NORMAL;
    float3 cameraPosition : CAMERA_POSITION;
    float4 ambient : AMBIENT;
    Material material : MATERIAL;
    Light light : LIGHT;
};


PSInput main(float3 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput result;

    // result.positionCam = float4(position, 1);
    // result.position = mul(float4(position, 1.0f), g_mWorldViewProj);
    // result.uv = uv;
    // result.normal = float4(mul(normal, (float3x3) modelView), 1.0f);
    // result.ambient = ambient;
    // result.lightPosition = lightPosition;
    // result.lightColor = lightColor;
    // result.diffuseColor = diffuseColor;
    // result.diffuseIntensity = diffuseIntensity;
    // result.attenuationConst = attenuationConst;
    // result.attenuationLinear = attenuationLinear;
    // result.attenuationQuad = attenuationQuad;
    // result.specularIntensity = specularIntensity;
    // result.specularPower = specularPower;
    // result.deltaTime = deltaTime;

    // transform to world space
    float4 positionWorld = mul(float4(position, 1), model);
    result.positionWorld = positionWorld.xyz;

    result.normal = mul(normal, (float3x3)model);

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

    return result;
}