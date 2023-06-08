#include "snGameObject.h"
#include "snRenderer.h"
#include "snGraphicDevice_Dx11.h"

namespace sn {
	sn::GameObject::GameObject()
		: mState(eState::Active)
	{
	}

	sn::GameObject::~GameObject()
	{
	}

	void sn::GameObject::Initialize()
	{
	}

	void sn::GameObject::Update()
	{
		// �� ������ ����

		// ����
	}

	void sn::GameObject::LateUpdate()
	{
	}

	void sn::GameObject::Render()
	{
		//������۷� ��ġ���� ũ������, ����, ������Ʈ ������Ѵ�.

		renderer::mesh->BindBuffer();
		renderer::shader->Binds();
		graphics::GetDevice()->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);
	}
}