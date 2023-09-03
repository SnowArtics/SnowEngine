#include "MonsterIdle.h"
#include "Monster.h"
#include "snSceneManager.h"
#include "snDungeon.h"
#include "DungeonMapManager.h"

using namespace sn;

MonsterIdle::MonsterIdle()
	:State(MON_STATE::IDLE)
{
}

MonsterIdle::~MonsterIdle()
{
}

void MonsterIdle::Update()
{
	std::pair<int, int> monMapPos = GetMonster()->GetMonsterMapPos();
	std::pair<int, int> playerMapPos = DungeonMapManager::GetInst()->GetPlayerMapPos();

	if (monMapPos != playerMapPos)
		return;

	SceneManager::ChangeMonsterState(GetAI(), MON_STATE::TRACE);
	//GetAI()->ChangeState(MON_STATE::TRACE);
}

void MonsterIdle::Enter()
{
}

void MonsterIdle::Exit()
{
}
