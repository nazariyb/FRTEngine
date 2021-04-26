//cbuffer SceneConstantBuffer : register(b0)
//{
//    float4x4 g_mWorldViewProj;
//    float4x4 model;
//    float4 lightPosition;
//    float4 diffuseColor;
//    float4 ambient;
//    float4 padding1;
//    float diffuseIntensity;
//    float attenuationConst;
//    float attenuationLinear;
//    float attenuationQuad;
//    float specularIntensity;
//    float specularPower;
//    float padding2[10];
//}

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

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSInput input) : SV_TARGET
{
    const float4 textureColor = g_texture.Sample(g_sampler, input.uv);
    const float4 vToL = input.lightPosition - input.positionCam;
    const float distToLight = length(vToL);
    const float4 dirToL = vToL / distToLight;
    
    const float attenuation = (input.attenuationConst + input.attenuationLinear * distToLight + input.attenuationQuad * (distToLight * distToLight));
    
    const float4 diffuse = input.diffuseColor * input.diffuseIntensity * attenuation * max(0.0f, dot(dirToL, input.normal));
    
    const float4 w = input.normal * dot(vToL, input.normal);
    const float4 reflection = w * 2.0f - vToL;
    
    const float4 specular = attenuation * (input.diffuseColor * input.diffuseIntensity) * input.specularIntensity * pow(max(0.0f, dot(normalize(-reflection), normalize(input.positionCam))), input.specularPower);

    return saturate(diffuse + input.ambient + specular) * textureColor;
}
