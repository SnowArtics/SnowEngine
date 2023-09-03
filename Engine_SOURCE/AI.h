#pragma once
#include "../Engine_SOURCE/snScript.h"

enum class MON_STATE {
	IDLE,//���
	WALK, //�ȱ�
	TRACE,//�ѱ�
	ATT,	//����� ��� ����
	TARGET_ATT,// ������ Ÿ�ٰ���
	DEAD,//���
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