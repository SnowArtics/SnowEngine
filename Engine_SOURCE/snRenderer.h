#pragma once

#include "SnowEngine.h"
#include "snGraphicDevice_Dx11.h"
#include "snMesh.h"
#include "snShader.h"

using namespace sn::math;

namespace renderer {
	struct Vertex {
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[];
	extern ID3D11InputLayout* triangleLayout;
	extern sn::Mesh* mesh;
	extern ID3D11Buffer* triangleConstantBuffer;
	extern Vector4 constantBufferPos;
	extern sn::Shader* shader;
	extern ID3D11PixelShader* trianglePSShader;

	int getVertexesSize();

	void Initialize();
	void Release();
}

