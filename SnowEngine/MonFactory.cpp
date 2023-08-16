#include "MonFactory.h"
#include "Monster.h"
#include "../Engine_SOURCE/snTransform.h"
#include "snAnimator.h"
#include "snResources.h"
#include "snCollider2D.h"
#include "snMesh.h"
#include "snMeshRenderer.h"
#include "snMaterial.h"
#include "AI.h"
#include "MonsterIdle.h"
#include "MonsterTrace.h"

#include "Golem.h"

using namespace sn;

Monster* MonFactory::CreateMonster(MonType _eType, sn::math::Vector2 _vPos)
{
    Monster* pMon = nullptr;

	switch (_eType)
	{
	case MonType::GOLEM:
	{
		pMon = new Golem;
		Transform* tr = pMon->GetComponent<Transform>();
		tr->SetPosition(Vector3(_vPos.x, _vPos.y, 0.0f));

		tMonInfo info = {};
		info.fAtt = 20.f;
		info.fAttRange = 2.f;
		info.fRecogRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = 0.5f;

		pMon->SetMonInfo(info);

		MeshRenderer* mr = pMon->AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

		std::shared_ptr<Texture> atlas
			= Resources::Load<Texture>(L"Monster_Golem_Move", L"..\\Resources\\Texture\\Dungeon\\Enemy\\GolemMove.png");
		Animator* at = pMon->AddComponent<Animator>();

		at->Create(L"GOLEM_MOVE_LEFT", atlas, Vector2(0.0f, 0.0f), Vector2(190.0f, 190.0f), 8);
		at->Create(L"GOLEM_MOVE_RIGHT", atlas, Vector2(0.0f, 190.0f), Vector2(190.0f, 190.0f), 8);
		at->Create(L"GOLEM_MOVE_UP", atlas, Vector2(0.0f, 380.0f), Vector2(190.0f, 190.0f), 8);
		at->Create(L"GOLEM_MOVE_DOWN", atlas, Vector2(0.0f, 570.0f), Vector2(190.0f, 190.0f), 8);

		atlas
			= Resources::Load<Texture>(L"Monster_Golem_Attack", L"..\\Resources\\Texture\\Dungeon\\Enemy\\GolemAttack.png");

		at->Create(L"GOLEM_ATTACK_LEFT", atlas, Vector2(0.0f, 190.0f), Vector2(190.0f, 190.0f), 13);
		at->Create(L"GOLEM_ATTACK_RIGHT", atlas, Vector2(0.0f, 190.0f), Vector2(190.0f, 190.0f), 13);
		at->Create(L"GOLEM_ATTACK_UP", atlas, Vector2(0.0f, 380.0f), Vector2(190.0f, 190.0f), 13);
		at->Create(L"GOLEM_ATTACK_DOWN", atlas, Vector2(0.0f, 570.0f), Vector2(190.0f, 190.0f), 13);

		at->PlayAnimation(L"GOLEM_MOVE_UP", true);

		sn::Collider2D* collider = pMon->AddComponent<sn::Collider2D>();
		collider->SetSize(Vector2(0.4f,0.4f));
		collider->SetCenter(Vector2(0.0f, -0.1f));

		AI* ai = pMon->AddComponent<AI>(pMon);
		ai->AddState(new MonsterIdle);
		ai->AddState(new MonsterTrace);
		ai->SetCurState(MON_STATE::IDLE);
	}
		break;
	case MonType::SLIME:
	{

	}
		break;
	default:
		break;
	}

    return pMon;
}
