
//우리는 버텍스를 3개를 만들었음으로 VSIn이 3개가 들어올것이다
struct VSIn//VS로 들어오는 코드다. 우리가 만든 Vertex가 일로 들어온다.
{
    float3 Pos : POSITION;//이 : 문법을 Sementic이라고 부른다.
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut//VS에서 나가는 코드다
{
    //SV_Position Sementic 언어는 레스터라이즈로 보내는 최종값을 지정해준다. 지금은 버텍스 셰이더만 써서 버텍스 셰이더에서 적어주는데
    //나중에 Hull Shader나 Tessellator Shader를 쓰면 버텍스 셰이더 스테이지에서 제일 마지막 셰이더에 써준다.
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
    Out.Pos.x += Position.x; //  -> 상수버퍼를 통해서 이데이터가 넘와야한다.
    Out.Pos.y += Position.y;
    Out.UV = In.UV;

    Out.Color = In.Color;
    
    return Out;
}