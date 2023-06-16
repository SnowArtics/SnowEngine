
//�츮�� ���ؽ��� 3���� ����������� VSIn�� 3���� ���ð��̴�
struct VSIn//VS�� ������ �ڵ��. �츮�� ���� Vertex�� �Ϸ� ���´�.
{
    float3 Pos : POSITION;//�� : ������ Sementic�̶�� �θ���.
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut//VS���� ������ �ڵ��
{
    //SV_Position Sementic ���� �����Ͷ������ ������ �������� �������ش�. ������ ���ؽ� ���̴��� �Ἥ ���ؽ� ���̴����� �����ִµ�
    //���߿� Hull Shader�� Tessellator Shader�� ���� ���ؽ� ���̴� ������������ ���� ������ ���̴��� ���ش�.
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

cbuffer Transform : register(b0)
{
    float4 Position;
    //float4 Rotation;
    //float4 Scale;
}

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.Pos = float4(In.Pos, 1.0f);
    Out.Pos.x += Position.x; //  -> ������۸� ���ؼ� �̵����Ͱ� �ѿ;��Ѵ�.
    Out.Pos.y += Position.y;
    Out.UV = In.UV;

    Out.Color = In.Color;
    
    return Out;
}