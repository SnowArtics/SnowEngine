#pragma once

enum class eShaderStage
{
	VS, //VertexShader
	HS, //HullShader
	DS, //DynamicShader
	GS, //GraphicShader
	PS, //PixelShader
	CS, //Compute Shader
	End,
};

enum class eCBType//enumConstantBufferType
{
	Transform,
	End,
};