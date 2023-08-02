#pragma once
#include "snMeshRenderer.h"
#include "snStructedBuffer.h"

namespace sn
{
	class ParticleSystem : public MeshRenderer
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		graphics::StructedBuffer* mBuffer;

		UINT    mCount;
		Vector4 mStartSize;
		Vector4 mEndSize;
		Vector4 mStartColor;
		Vector4 mEndColor;
		float   mLifeTime;
	};
}
