#include "snLayer.h"

namespace sn {
	sn::Layer::Layer()
	{
	}

	sn::Layer::~Layer()
	{
		for (GameObject* gameObject : mGameObjects) {
			delete gameObject;
		}

		mGameObjects.clear();
	}

	void sn::Layer::Initialize()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Initialize();
		}
	}

	void sn::Layer::Update()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Update();
		}
	}

	void sn::Layer::LateUpdate()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->LateUpdate();
		}
	}

	void sn::Layer::Render()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Render();
		}
	}
}