#include "snEventMgr.h"

#include "snEntity.h"
#include "snSceneManager.h"
#include "snScene.h"

//#include "AI.h"
//
//#include "CState.h"
//#include "CUIMgr.h"

void CEventMgr::update()
{
	//===================================
	//���� �����ӿ��� ����ص� DeadObject���� ����
	//=====================================
	for (size_t i = 0; i < m_vecDead.size(); i++) {
		delete m_vecDead[i];
	}
	m_vecDead.clear();

	//=========
	//Event ó��
	//========

	for (size_t i = 0; i < m_vecEvent.size(); i++) {
		Execute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void CEventMgr::Execute(const tEvent& _eve)
{
	//switch (_eve.eEven) {
	//case eEventType::CREATE_OBJECT:
	//	//lParam : Object Address
	//	//wParam : GroupType
	//{
	//	Entity* pNewObj = (Entity*)_eve.lParam;
	//	GROUP_TYPE eType = (GROUP_TYPE)_eve.WParam;

	//	CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	//}
	//break;

	//case eEventType::DELETE_OBJECT:
	//{
	//	//lParam : Object Address
	//	//wParam : �ʿ����

	//	//Object�� Dead���·� ����
	//	//�������� ������Ʈ���� ��Ƶд�.
	//	Entity* pDeadObj = (Entity*)_eve.lParam;
	//	pDeadObj->
	//	m_vecDead.push_back(pDeadObj);
	//}
	//break;

	//case eEventType::SCENE_CHANGE:
	//{
	//	// lParam: NextSceneType
	//	// Scene ����
	//	CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);

	//	SceneManager::LoadScene(L"VillageScene");
	//}
	//break;

	//case EVENT_TYPE::CHANGE_AI_STATE:
	//	//lParam : AI
	//	//wParam : Next STATE_TYPE
	//{
	//	AI* pAI = (AI*)_eve.lParam;
	//	MON_STATE eNextState = (MON_STATE)_eve.WParam;

	//	pAI->ChangeState(eNextState);
	//}


	//break;
	int a = 0;
}

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}
