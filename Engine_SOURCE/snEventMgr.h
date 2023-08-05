#pragma once
#include "snEnums.h"
#include "snEntity.h"
#include <vector>
#include <basetsd.h>

#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
					private:\
						type();\
						~type();\

using namespace sn;
using namespace sn::enums;

struct tEvent {
	eEventType	eEven;
	DWORD_PTR		lParam;
	DWORD_PTR		WParam;
};

class CEventMgr
{
	SINGLE(CEventMgr)

private:
	std::vector<tEvent> m_vecEvent;

	std::vector<Entity*> m_vecDead;//죽을 오브젝트를 모음.


public:
	void update();

private:
	void Execute(const tEvent& _eve);

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }
};

