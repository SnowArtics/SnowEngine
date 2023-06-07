#pragma once
#include "snGraphics.h"

namespace sn::graphics
{
	class ConstantBuffer : public GpuBuffer//구조체를 상속받는다.
	{
	public:
		ConstantBuffer(const eCBType type);
		~ConstantBuffer();

		bool Create(size_t size);
		void SetData(void* data);//상수버퍼를 내가 만든 데이터로 묶어준다.(MAP)
		void Bind(eShaderStage stage);//만든 상수버퍼를 GPU에 전송한다

	private:
		const eCBType mType;
	};
}