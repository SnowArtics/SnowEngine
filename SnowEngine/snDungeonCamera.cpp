#include "snDungeonCamera.h"

#include "snDungeon.h"
#include "snSceneManager.h"

namespace sn {
	DungeonCamera::DungeonCamera()
		:prevPlayerMapPos({-1,-1})
		, curPlayerMapPos({ -1,-1 })
	{
	}
	DungeonCamera::~DungeonCamera()
	{
	}
	void sn::DungeonCamera::Update()
	{
		Dungeon* dungeonScene = static_cast<Dungeon*>(SceneManager::GetActiveScene());
		prevPlayerMapPos = curPlayerMapPos;
		curPlayerMapPos = dungeonScene->GetPlayerMapPos();

		Transform* tr = GetOwner()->GetComponent<Transform>();

		if (prevPlayerMapPos != curPlayerMapPos) {
			tr->SetPosition(Vector3(curPlayerMapPos.second * 9.72f, -(curPlayerMapPos.first * 5.45f), -10.0f));
		}
	}
}