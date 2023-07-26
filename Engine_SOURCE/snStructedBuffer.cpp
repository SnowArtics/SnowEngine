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
		mSize = size;//내 구조체 하나의 크기
		mStride = stride;//구조체의 배열 크기

		desc.ByteWidth = mSize * stride;//구조체가 배열이라면 배열 만큼 곱해줌
		desc.StructureByteStride = mSize;

		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;//CPU가 수정할 수 있다
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;//셰이더 리소스에 바인딩한다.그래서 t 로 들어감.
		desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;//구조화 버퍼를 사용

		if (!GetDevice()->CreateBuffer(buffer.GetAddressOf(), &desc, nullptr))//버퍼를 만든다. 데이터를 안 넣은 채로
			return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};//셰이더ResourceView를 사용함으로 셰이더 ResourceViewDesc 생성
																				//텍스쳐를 만들든 뭘 만들든 접근하려면 이 View를 생성해야 했다.
																				//얘를 통해 셰이더에서 사용하는 t레지스터에 묶을 수 있다.
		srvDesc.BufferEx.NumElements = mStride;
		srvDesc.ViewDimension = D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_BUFFEREX;

		if (!(GetDevice()->CreateShaderResourceView(buffer.Get(), &srvDesc, mSRV.GetAddressOf())))//버퍼와 srvDesc를 사용해서 mSRV(셰이더 리소스 뷰를 생성)
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
