#include "snMesh.h"
#include "snRenderer.h"

namespace sn
{

	//������ �ʱ�ȭ ������.
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

	//CreateVertexBuffer�� ���Ŀ� �۾������� ��� ������ ���ؽ� ���۸� ����� �۾�������,
	bool Mesh::CreateVertexBuffer(void* data, UINT Count)
	{
		mVBDesc.ByteWidth = sizeof(renderer::Vertex) * Count;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;//D3D11_BIND_VERTEX_BUFFER�� ����
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;//�ٲܼ� ���� ����
		mVBDesc.CPUAccessFlags = 0; // NONE //�翬�� CPU�� ���� ����.

		//���� SubResourceData�� ����ؼ� ��������.
		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		//���� CreateBuffer�� ���ش�.
		if (!GetDevice()->CreateBuffer(mVertexBuffer.GetAddressOf(), &mVBDesc, &sub))
			return false;

		return true;
	}

	//CreateIndexBuffer�� ����� �ƶ��̴�.
	//BindFlags�� �ٲ��ְ� Desc�̸� �ٲ��ְ�, size �ٲ��ְ� ���ָ� �ȴ�.
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

