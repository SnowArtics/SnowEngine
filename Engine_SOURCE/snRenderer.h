#pragma once

#include "SnowEngine.h"
#include "snGraphicDevice_Dx11.h"
#include "snMesh.h"
#include "snShader.h"
#include "snConstantBuffer.h"
#include "snCamera.h"

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

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 CameraPosition;
		Vector2 CameraScale;
		Vector2 Resolution;
	};

	extern sn::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End];

	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

	extern sn::Camera* mainCamera;
	extern std::vector<sn::Camera*> cameras;
	extern std::vector<DebugMesh> debugMeshs;

	void Initialize();
	void Render();
	void Release();

	void PushDebugMeshInfo(DebugMesh mesh);
}