//�ȼ��ϳ��� ��µǾ�� ������ return ���� float4, ����Ÿ������ �����ٲ��ϱ� SV_TARGET�̶�� �Ѵ�.
//SV�� System Value��

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