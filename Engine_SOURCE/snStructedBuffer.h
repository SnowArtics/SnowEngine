#pragma once
#include "snEntity.h"
#include "snGraphics.h"

namespace sn::graphics
{
	class StructedBuffer : public GpuBuffer
	{
	public:
		StructedBuffer();
		~StructedBuffer();

		//����ȭ ���۸� ���� �� ���� �Լ�, ���� �� ���� ���� �����ϱ� �������ش�.
		bool Create(UINT size, UINT stride, eSRVType type);
		//���� ����ȭ ���ۿ� �����͸� �������ش�. �迭�� ������ ���� �����ϱ� � ���� ���� �Ķ���� ����
		void SetData(void* data, UINT stride);
		//GPU�� �����ش�.
		void Bind(eShaderStage stage, UINT slot);

		//Get �Լ� ����
		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

	private:
		//������۸� GPU�� ���� �� �־��� ������ ShaderResource View�� �����ϱ� �����̴�.
		//������ GraphicDevice�� �����ϴ��� �̿��ؼ� �������. ����ȭ ���ۿ��� �ϳ� �����ؼ�
		//������ ���̴�.
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		eSRVType mType;

		UINT mSize;//����ü ������ ũ��
		UINT mStride;//����ü ���۸� �迭�� ���� �� ũ��
	};
}
