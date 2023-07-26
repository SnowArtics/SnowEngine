#include "snStructedBuffer.h"
#include "snGraphicDevice_Dx11.h"

namespace sn::graphics
{
	StructedBuffer::StructedBuffer()
		: mSRV(nullptr)
		, mType(eSRVType::None)
		, mSize(0)
		, mStride(0)
	{

	}

	StructedBuffer::~StructedBuffer()
	{
	}
	bool StructedBuffer::Create(UINT size, UINT stride, eSRVType type)
	{
		mSize = size;//�� ����ü �ϳ��� ũ��
		mStride = stride;//����ü�� �迭 ũ��

		desc.ByteWidth = mSize * stride;//����ü�� �迭�̶�� �迭 ��ŭ ������
		desc.StructureByteStride = mSize;

		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;//CPU�� ������ �� �ִ�
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;//���̴� ���ҽ��� ���ε��Ѵ�.�׷��� t �� ��.
		desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;//����ȭ ���۸� ���

		if (!GetDevice()->CreateBuffer(buffer.GetAddressOf(), &desc, nullptr))//���۸� �����. �����͸� �� ���� ä��
			return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};//���̴�ResourceView�� ��������� ���̴� ResourceViewDesc ����
																				//�ؽ��ĸ� ����� �� ����� �����Ϸ��� �� View�� �����ؾ� �ߴ�.
																				//�긦 ���� ���̴����� ����ϴ� t�������Ϳ� ���� �� �ִ�.
		srvDesc.BufferEx.NumElements = mStride;
		srvDesc.ViewDimension = D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_BUFFEREX;

		if (!(GetDevice()->CreateShaderResourceView(buffer.Get(), &srvDesc, mSRV.GetAddressOf())))//���ۿ� srvDesc�� ����ؼ� mSRV(���̴� ���ҽ� �並 ����)
			return false;

		return true;
	}

	void StructedBuffer::SetData(void* data, UINT stride)
	{
		if (mStride < stride)
			Create(mSize, stride, eSRVType::None);
		else
			GetDevice()->BindBuffer(buffer.Get(), data, mSize * stride);
	}

	void StructedBuffer::Bind(eShaderStage stage, UINT slot)
	{
		GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
	}
}
