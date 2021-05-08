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
    float4 lightDirection : LIGHT_DIRECTION;
    float4 diffuseColor : DIFFUSE_COLOR;
    float diffuseIntensity : DIFFUSE_INTENSITY;
    float attenuationConst : ATTENUATION;
    float attenuationLinear : ATTENUATION_LINEAR;
    float attenuationQuad : ATTENUATION_QUAD;
    float specularIntensity : SPECULAR_INTENSITY;
    float specularPower : SPECULAR_POWER;
    float deltaTime : DELTA_TIME;
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
    const float4 textureColor = g_texture.Sample(g_sampler, input.uv);
    const float4 vToL = input.lightPosition - input.positionCam;
    const float distToLight = length(vToL);
    const float4 dirToL = vToL / distToLight;
    
    const float attenuation = (input.attenuationConst + input.attenuationLinear * distToLight + input.attenuationQuad * (distToLight * distToLight));
    
    const float4 diffuse = input.diffuseColor * input.diffuseIntensity * attenuation * max(0.0f, dot(dirToL, input.normal));
    
    const float4 w = input.normal * dot(vToL, input.normal);
    const float4 reflection = w * 2.0f - vToL;
    
    const float4 specular = attenuation * (input.diffuseColor * input.diffuseIntensity) * input.specularIntensity * pow(max(0.0f, dot(normalize(-reflection), normalize(input.positionCam))), input.specularPower);

#pragma region waves
    // const float t = InverseLerp(0.2, 0.8, input.uv.y);
    // const float t = abs(frac(input.uv.x * 5) * 2 - 1);
    const float offsetX = cos((input.uv.y) * TAU * 3.0) * 0.02;
    const float t = cos((input.uv.x + offsetX + input.deltaTime * 3.0) * TAU * 4) * 0.9 + 0.5;
    const float4 outputColor = lerp(float4(241.0 / 255.0, 227.0 / 255.0, 228.0 / 255.0, 1), float4(98.0 / 255.0, 168.0 / 255.0, 124.0 / 255.0, 1), t);

    // return saturate( outputColor );
#pragma endregion waves
    
    return saturate(diffuse + input.ambient + specular) * float4(0.1f, 0.1f, 0.2f, 1.0f);
}
