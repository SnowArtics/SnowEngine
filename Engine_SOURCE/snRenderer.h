#pragma once

#include "SnowEngine.h"
#include "snGraphicDevice_Dx11.h"
#include "snMesh.h"
#include "snShader.h"
#include "snConstantBuffer.h"
#include "snCamera.h"
#include "snLight.h"

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

	CBUFFER(AnimatorCB, CBSLOT_ANIMATION2D)
	{
		Vector2 spriteLeftTop;
		Vector2 spriteSize;
		Vector2 spriteOffset;
		Vector2 atlasSize;
		UINT animationType;
		Vector3 padding;
	};

	CBUFFER(EditorCB, CBSLOT_ANIMATION2D)
	{
		Vector4 ColliderColor;
	};

	CBUFFER(ParticleCB, CBSLOT_PARTICLE)
	{
		UINT elementCount;
		float elpasedTime;
		float deltaTime;
		int padd2;
	};

	CBUFFER(NoiseCB, CBSLOT_NOISE)
	{
		Vector4 size;
	};

	CBUFFER(MonsterCB, CBSLOT_MONSTER) {
		Vector4 color;
		int		state;
		Vector3 pad1;
	};

	extern sn::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End];

	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

	extern std::vector<sn::Light*> lights;
	extern sn::Camera* mainCamera;
	extern std::vector<sn::Camera*> cameras;
	extern std::vector<DebugMesh> debugMeshs;

	void Initialize();
	void BindLights();
	void Render();
	void Release();

	void PushDebugMeshInfo(DebugMesh mesh);
}