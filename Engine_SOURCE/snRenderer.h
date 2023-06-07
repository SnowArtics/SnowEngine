#pragma once

#include "SnowEngine.h"
#include "snGraphicDevice_Dx11.h"
#include "snMesh.h"
#include "snShader.h"
#include "snConstantBuffer.h"

using namespace sn::math;
namespace renderer {
	struct Vertex {
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[];
	extern sn::Mesh* mesh;
	extern sn::Shader* shader;
	extern sn::graphics::ConstantBuffer* constantBuffer;
	extern Vector4 constantBufferPos;

	void Initialize();
	void Release();
}