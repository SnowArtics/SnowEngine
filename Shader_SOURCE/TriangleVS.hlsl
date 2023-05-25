
//우리는 버텍스를 3개를 만들었음으로 VSIn이 3개가 들어올것이다
struct VSIn//VS로 들어오는 코드다. 우리가 만든 Vertex가 일로 들어온다.
{
    float3 Pos : POSITION;//이 : 문법을 Sementic이라고 부른다.
    float4 Color : COLOR;
};

struct VSOut//VS에서 나가는 코드다
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
};

VSOut main(VSIn In)//우리가 만든 VSIn hlsl코드로 넣어준다.
{
    VSOut Out = (VSOut)0.0f;



    Out.Pos = float4(In.Pos, 1.0f);//SV_Position은 무조건 float4여야함
    Out.Color = In.Color;

    return Out;
}