#pragma once
#include "..\Engine_SOURCE\snSceneManager.h"
#include "snPlayScene.h"
#include "snTitleScene.h"
#include "snVillageScene.h"
#include "snShopScene.h"
#include "snDungeonEntrance.h"
#include "snDungeonScene01.h"
#include "snDungeonScene02.h"
#include "snDungeonScene03.h"
#include "snDungeonBoss.h"

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

		SceneManager::CreateScene<TitleScene>(L"TitleScene");
		SceneManager::CreateScene<VillageScene>(L"VillageScene");
		SceneManager::CreateScene<ShopScene>(L"ShopScene");
		SceneManager::CreateScene<DungeonEntrance>(L"DungeonEntrance");
		SceneManager::CreateScene<DungeonScene01>(L"DungeonScene01");
		SceneManager::CreateScene<DungeonScene02>(L"DungeonScene02");
		SceneManager::CreateScene<DungeonScene03>(L"DungeonScene03");
		SceneManager::CreateScene<DungeonBoss>(L"DungeonBossScene01");

		SceneManager::LoadScene(L"VillageScene");

		SceneManager::SetChangeScene(L"VillageScene");
		//SceneManager::LoadScene(L"VillageScene");
	}
}