cbuffer SceneConstantBuffer : register(b0)
{
    float4x4 g_mWorldViewProj;
    float4x4 modelView;
    float4 lightPosition;
    float4 diffuseColor;
    float4 ambient;
    float4 padding1;
    float diffuseIntensity;
    float attenuationConst;
    float attenuationLinear;
    float attenuationQuad;
    float specularIntensity;
    float specularPower;
    float padding2[10];
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 positionCam : POSITION_CAM;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
    float4 ambient : AMBIENT;
    float4 lightPosition : LIGHT_POSITION;
    float4 diffuseColor : DIFFUSE_COLOR;
    float diffuseIntensity : DIFFUSE_INTENSITY;
    float attenuationConst : ATTENUATION;
    float attenuationLinear : ATTENUATION_LINEAR;
    float attenuationQuad : ATTENUATION_QUAD;
    float specularIntensity : SPECULAR_INTENSITY;
    float specularPower : SPECULAR_POWER;
};


PSInput main(float3 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput result;

    result.positionCam = mul(float4(position, 1.0f), modelView);
    result.position = mul(float4(position, 1.0f), g_mWorldViewProj);
    result.uv = uv;
    result.normal = float4(mul(normal, (float3x3) modelView), 1.0f);
    result.ambient = ambient;
    result.lightPosition = lightPosition;
    result.diffuseColor = diffuseColor;
    result.diffuseIntensity = diffuseIntensity;
    result.attenuationConst = attenuationConst;
    result.attenuationLinear = attenuationLinear;
    result.attenuationQuad = attenuationQuad;
    result.specularIntensity = specularIntensity;
    result.specularPower = specularPower;

    return result;
}