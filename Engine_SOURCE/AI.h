#pragma once
#include "../Engine_SOURCE/snScript.h"

enum class MON_STATE {
	IDLE,//대기
	WALK, //걷기
	TRACE,//쫓기
	ATT,	//가까운 상대 공격
	TARGET_ATT,// 지정한 타겟공격
	DEAD,//사망
	END,
};

enum class MonDir {
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

class Monster;
class State;

class AI : public sn::Script
{
public:
	AI(Monster* _owner);
	~AI();

public:
	virtual void Update();

public:
	void AddState(State* _pState);
	void ChangeState(MON_STATE _eNextState);

	void SetCurState(MON_STATE _eState);
	void SetDir(MonDir _monDir) { curDir = _monDir; }

	State* GetState(MON_STATE _eState);
	Monster* GetOwner() { return m_pOwner; }
	MonDir GetCurDir() { return curDir; }
	MonDir GetPrevDir() { return prevDir; }
	MON_STATE GetCurStateName() { return curStateName; }
	MON_STATE GetPrevStateName() { return prevStateName; }

private:
	std::map<MON_STATE, State*>	m_mapState;
	State* m_pCurState;
	Monster* m_pOwner;
	MON_STATE curStateName;
	MON_STATE prevStateName;

	MonDir			prevDir;
	MonDir			curDir;

	friend class Monster;
};