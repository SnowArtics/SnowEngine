#include "snSceneManager.h"
#include "AI.h"

namespace sn
{
	Scene* SceneManager::mActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::mScenes;
	std::wstring SceneManager::ChangeSceneName = L"";
	std::queue<std::pair<AI*, MON_STATE>> SceneManager::ChangeMonsterStateQueue;


	void SceneManager::Initialize()
	{
		//mActiveScene->Initialize();
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
	}

	void SceneManager::Render()
	{
		mActiveScene->Render();
	}

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
	}

	void SceneManager::ChangeScene()
	{
		if (ChangeSceneName == L"")
			return;
		SceneManager::LoadScene(ChangeSceneName);
		ChangeSceneName = L"";
	}

	void SceneManager::ChangeAIState()
	{
		if (!ChangeMonsterStateQueue.empty()) {
			std::pair<AI*, MON_STATE> ai = ChangeMonsterStateQueue.front();
			AI* pAI = ai.first;
			MON_STATE eNextState = ai.second;

			pAI->ChangeState(eNextState);

			ChangeMonsterStateQueue.pop();
		}
	}

	void SceneManager::Release()
	{
		for (auto& iter : mScenes)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}
	Scene* SceneManager::LoadScene(std::wstring name)
	{
		std::map<std::wstring, Scene*>::iterator iter = mScenes.find(name);

		if (iter == mScenes.end())
			return nullptr;

		mActiveScene->OnExit();
		mActiveScene = iter->second;
		mActiveScene->OnEnter();

		return iter->second;
	}
}
