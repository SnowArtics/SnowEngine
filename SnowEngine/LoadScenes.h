#pragma once
#include "..\Engine_SOURCE\snSceneManager.h"
#include "snPlayScene.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\SnowEngine.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\SnowEngine.lib")
#endif

namespace sn
{
	void InitializeScenes()
	{
		//PlayScene* playScene = new PlayScene();
		SceneManager::CreateScene<PlayScene>(L"PlayScene");
	}
}