cbuffer SceneConstantBuffer : register(b0)
{
    float4x4 g_mWorldViewProj;
    float4x4 model;
    float4 lightPosition;
    float4 diffuseColor;
    float4 ambient;
    float diffuseIntensity;
    float attenuationConst;
    float attenuationLinear;
    float attenuationQuad;
    float specularIntensity;
    float specularPower;
    float padding[14];
}

//cbuffer ObjectConstantBuffer : register(b1)
//{
//    float specularIntensity;
//    float specularPower;
//}

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

PSInput VShader(float3 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput result;

    //position.w = 1;
    result.position = mul(float4(position, 1.0f), g_mWorldViewProj);
    result.uv = uv;
    result.normal = mul(float4(normal, 1.0f), model);
    //result.normal = normal;

    return result;
}

float4 PShader(PSInput input) : SV_TARGET
{
    const float4 textureColor = g_texture.Sample(g_sampler, input.uv);
    const float4 vToL = lightPosition - input.position;
    const float distToLight = length(vToL);
    const float4 dirToL = vToL / distToLight;
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinear * distToLight + attenuationQuad * (distToLight * distToLight));
    
    const float4 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(dirToL, input.normal));
    
    const float4 w = input.normal * dot(vToL, input.normal);
    const float4 reflection = w * 2.0f - vToL;
    
    const float4 specular = attenuation * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(reflection), normalize(input.position))), specularPower);
    
    //return float4(saturate(((float3) diffuse + (float3) ambient/* + (float3) specular*/) * float3(.2f, 1.0f, .2f)), 1.0f);
    return ambient;

}