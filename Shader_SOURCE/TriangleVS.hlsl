
//�츮�� ���ؽ��� 3���� ����������� VSIn�� 3���� ���ð��̴�
struct VSIn//VS�� ������ �ڵ��. �츮�� ���� Vertex�� �Ϸ� ���´�.
{
    float3 Pos : POSITION;//�� : ������ Sementic�̶�� �θ���.
    float4 Color : COLOR;
};

struct VSOut//VS���� ������ �ڵ��
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
};

VSOut main(VSIn In)//�츮�� ���� VSIn hlsl�ڵ�� �־��ش�.
{
    VSOut Out = (VSOut)0.0f;



    Out.Pos = float4(In.Pos, 1.0f);//SV_Position�� ������ float4������
    Out.Color = In.Color;

    return Out;
}