//픽셸하나가 출력되어야 함으로 return 값은 float4, 렌더타겟으로 보내줄꺼니까 SV_TARGET이라고 한다.
//SV는 System Value임

struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

Texture2D albedoTexture : register(t0);

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_TARGET
{
    //return In.Color;
    float4 color = (float)0.0f;
    color = albedoTexture.Sample(anisotropicSampler, In.UV);

    return color;
}