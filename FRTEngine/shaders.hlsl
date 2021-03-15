struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

PSInput VShader(float4 position : POSITION, float2 uv : TEXCOORD)
{
    PSInput result;

    result.position = position;
    result.uv = uv;

    return result;
}

float4 PShader(PSInput input) : SV_TARGET
{
    return g_texture.Sample(g_sampler, input.uv);
    //return input.color;
    //return float4(0.9f, 0.9f, 0.9f, 1.0f);
}