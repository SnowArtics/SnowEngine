#include "snSceneManager.h"
#include "snPlayScene.h"

namespace sn
{
	Scene* SceneManager::mActiveScene = nullptr;
	void sn::SceneManager::Initialize()
	{
		mActiveScene = new PlayScene();
	}

	void sn::SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void sn::SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
	}

	void sn::SceneManager::Render()
	{
		mActiveScene->Render();
	}
}
