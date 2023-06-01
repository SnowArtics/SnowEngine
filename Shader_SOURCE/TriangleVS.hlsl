
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

cbuffer Transform : register(b0)
{
    float4 Pos;
}

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.Pos = float4(In.Pos, 1.0f);
    Out.Pos.x += Pos.x; //  -> ������۸� ���ؼ� �̵����Ͱ� �ѿ;��Ѵ�.
    
    Out.Color = In.Color;
    
    return Out;
}