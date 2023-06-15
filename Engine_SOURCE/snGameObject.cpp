#include "snGameObject.h"
#include "snRenderer.h"
#include "snGraphicDevice_Dx11.h"
#include "snTransform.h"

namespace sn {
	sn::GameObject::GameObject()
		: mState(eState::Active)
	{
		AddComponent<Transform>();
	}

	sn::GameObject::~GameObject()
	{
	}

	void sn::GameObject::Initialize()
	{
	}

	void sn::GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			comp->Update();
		}
	}

	void sn::GameObject::LateUpdate()
	{
		for (Component* comp : mComponents)
		{
			comp->LateUpdate();
		}
	}

	void sn::GameObject::Render()
	{
		for (Component* comp : mComponents)
		{
			comp->Render();
		}
		//������۷� ��ġ���� ũ������, ����, ������Ʈ ������Ѵ�.
	}
}