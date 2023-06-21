#pragma once

#include "SnowEngine.h"
#include "snGraphicDevice_Dx11.h"
#include "snMesh.h"
#include "snShader.h"
#include "snConstantBuffer.h"

using namespace sn::math;
using namespace sn::graphics;
namespace renderer {
	struct Vertex {
		Vector3 pos;
		Vector4 color;
		Vector2 uv;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix mWorld;
		Matrix mView;
		Matrix mProjection;
	};

	extern Vertex vertexes[];
	extern sn::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End];

	void Initialize();
	void Release();
}