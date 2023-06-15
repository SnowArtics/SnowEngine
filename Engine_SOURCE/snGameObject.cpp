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
		//상수버퍼로 위치정보 크기정보, 색깔, 업데이트 해줘야한다.
	}
}