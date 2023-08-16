#pragma once
#include "snScene.h"
#include <queue>
#include "AI.h"

namespace sn
{
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render();
		static void Destroy();
		static void ChangeScene();
		static void ChangeAIState();
		static void Release();

		template <typename T>
		static bool CreateScene(std::wstring name)
		{
			T* scene = new T();

			std::map<std::wstring, Scene*>::iterator iter
				= mScenes.find(name);

			if (iter != mScenes.end())
				return false;

			mScenes.insert(std::make_pair(name, scene));
			mActiveScene = scene;
			//scene->Initialize();
			return true;
		}
		static Scene* GetActiveScene() { return mActiveScene; }
		static Scene* LoadScene(std::wstring name);
		static void SetChangeScene(std::wstring _sceneName) { ChangeSceneName = _sceneName; }
		static void ChangeMonsterState(AI* _ai, MON_STATE _monState) { ChangeMonsterStateQueue.push(std::make_pair(_ai, _monState)); }
	private:
		static Scene* mActiveScene;
		static std::map<std::wstring, Scene*> mScenes;

		static std::wstring ChangeSceneName;

		static std::queue<std::pair<AI*, MON_STATE>> ChangeMonsterStateQueue;
	};
}