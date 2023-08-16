#include "snGameObject.h"
#include "snRenderer.h"
#include "snGraphicDevice_Dx11.h"
#include "snTransform.h"

namespace sn {
	GameObject::GameObject()
		: mState(eState::Active)
		, mEnable(true)
	{
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			delete comp;
			comp = nullptr;
		}
		for (Script* script : mScripts)
		{
			if (script == nullptr)
				continue;

			delete script;
			script = nullptr;
		}
	}

	void GameObject::Initialize()
	{
		for (Component* comp : mComponents)
		{
			comp->Initialize();
		}

		for (Script* script : mScripts)
		{
			script->Initialize();
		}
	}

	void GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			comp->Update();
		}

		for (Script* script : mScripts)
		{
			script->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (Component* comp : mComponents)
		{
			comp->LateUpdate();
		}

		for (Script* script : mScripts)
		{
			script->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		for (Component* comp : mComponents)
		{
			comp->Render();
		}
		//상수버퍼로 위치정보 크기정보, 색깔, 업데이트 해줘야한다.

		for (Script* script : mScripts)
		{
			script->Render();
		}
	}

	void GameObject::OnCollisionEnter(Collider2D* other) {

	}

	void GameObject::OnCollisionStay(Collider2D* other) {

	}

	void GameObject::OnCollisionExit(Collider2D* other) {

	}
}