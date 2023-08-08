#pragma once
#include "snComputeShader.h"
#include "snStructedBuffer.h"

namespace sn::graphics
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		~ParticleShader();

		virtual void Binds() override;
		virtual void Clear() override;

		void SetParticleBuffer(StructedBuffer* particleBuffer);
		void SetSharedBuffer(StructedBuffer* sharedBuffer) { mSharedBuffer = sharedBuffer; }

	private:
		StructedBuffer* mParticleBuffer;
		StructedBuffer* mSharedBuffer;
	};
}
