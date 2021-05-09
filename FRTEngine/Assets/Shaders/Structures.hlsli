struct Material
{
    float4 DiffuseAlbedo;
    float3 FresnelR0;
    float Shininess;
};

struct Light
{
    float3 Strength;
    float FalloffStart; // point/spot light only
    float3 Direction;   // directional/spot light only
    float FalloffEnd;   // point/spot light only
    float3 Position;    // point light only
    float SpotPower;    // spot light only
};

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
    Light lights[2] : LIGHT;
};
