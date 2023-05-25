#pragma once

#include "SnowEngine.h"
#include "snGraphicDevice_Dx11.h"

using namespace sn::math;

namespace sn::renderer {
	struct Vertex {
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[];
	extern ID3D11Buffer* triangleBuffer;
	extern ID3DBlob* errorBlob;
	extern ID3DBlob* triangleVSBlob;
	extern ID3D11VertexShader* triangleVSShader;
	extern ID3DBlob* trianglePSBlob;
	extern ID3D11PixelShader* trianglePSShader;

	void Initialize();
}

