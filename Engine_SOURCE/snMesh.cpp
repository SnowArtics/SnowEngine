#include "snMesh.h"
#include "snRenderer.h"

namespace sn
{

	//생성자 초기화 해주자.
	Mesh::Mesh()
		: mVertexBuffer(nullptr)
		, mIndexBuffer(nullptr)
		, mVBDesc{}
		, mIBDesc{}
		, mIndexCount(0)
	{
	}

	Mesh::~Mesh()
	{
	}

	HRESULT Mesh::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	//CreateVertexBuffer를 이후에 작업해주자 얘는 무조건 버텍스 버퍼만 만드는 작업임으로,
	bool Mesh::CreateVertexBuffer(void* data, UINT Count)
	{
		mVBDesc.ByteWidth = sizeof(renderer::Vertex) * Count;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;//D3D11_BIND_VERTEX_BUFFER로 선언
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;//바꿀수 없게 설정
		mVBDesc.CPUAccessFlags = 0; // NONE //당연히 CPU가 수정 못함.

		//이제 SubResourceData를 사용해서 묶어주자.
		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		//이후 CreateBuffer를 해준다.
		if (!GetDevice()->CreateBuffer(mVertexBuffer.GetAddressOf(), &mVBDesc, &sub))
			return false;

		return true;
	}

	//CreateIndexBuffer도 비슷한 맥락이다.
	//BindFlags만 바꿔주고 Desc이름 바꿔주고, size 바꿔주고 해주면 된다.
	bool Mesh::CreateIndexBuffer(void* data, UINT Count)
	{
		mIndexCount = Count;
		mIBDesc.ByteWidth = sizeof(UINT) * Count;
		mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mIBDesc.CPUAccessFlags = 0; // NONE

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		if (!GetDevice()->CreateBuffer(mIndexBuffer.GetAddressOf(), &mIBDesc, &sub))
			return false;

		return true;
	}
	void Mesh::BindBuffer()
	{
		UINT stride = sizeof(renderer::Vertex);
		UINT offset = 0;

		GetDevice()->BindVertexBuffer(0, mVertexBuffer.GetAddressOf(), &stride, &offset);
		GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

