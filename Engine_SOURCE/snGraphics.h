#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "snEnums.h"
#include "snMath.h"

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

#define CBSLOT_TRANSFORM		0
#define CBSLOT_GRID				2
#define CBSLOT_ANIMATION2D		3
#define CBSLOT_EDITOR			4
#define CBSLOT_PARTICLE			5

namespace sn::graphics
{
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
		Material,
		Grid,
		Animator,
		Editor,
		Particle,
		End,
	};

	enum class eSamplerType
	{
		Point,
		Anisotropic,
		End,
	};

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		End,
	};

	enum class eDSType
	{
		Less,
		LessEqual,
		Greater,
		NoWrite,
		None,
		End,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		End,
	};

	enum class eRenderingMode
	{
		Opaque,
		CutOut,
		Transparent,
		End,
	};

	enum class eViewType //Structed Buffer Type의 준말. 이름이 왜 이따군지는 나도 모르겠다.
	{
		None,
		SRV,
		UAV,
		End,
	};

	struct GpuBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		D3D11_BUFFER_DESC desc;

		GpuBuffer()
			: buffer(nullptr)
			, desc{}
		{

		}
		virtual ~GpuBuffer() = default;
	};

	struct DebugMesh
	{
		std::wstring name;

		enums::eColliderType type;
		math::Vector3 position;
		math::Vector3 rotation;
		math::Vector3 scale;

		std::vector<math::Vector3> vertexs;

		bool hit;

		float radius;
		float duration;
		float time;

		bool enable;
	};

	struct LightAttribute
	{
		math::Vector4 color;
		math::Vector4 position;
		math::Vector4 direction;

		enums::eLightType type;
		float radius;
		float angle;
		int pad;
	};

	struct Particle
	{
		math::Vector4 position;
		math::Vector4 direction;

		float endTime;
		float time;
		float speed;
		UINT active;
	};

	struct ParticleShared
	{
		UINT sharedActiveCount;
	};
}