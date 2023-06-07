#pragma once
#include "snGraphics.h"

namespace sn::graphics
{
	class ConstantBuffer : public GpuBuffer//����ü�� ��ӹ޴´�.
	{
	public:
		ConstantBuffer(const eCBType type);
		~ConstantBuffer();

		bool Create(size_t size);
		void SetData(void* data);//������۸� ���� ���� �����ͷ� �����ش�.(MAP)
		void Bind(eShaderStage stage);//���� ������۸� GPU�� �����Ѵ�

	private:
		const eCBType mType;
	};
}