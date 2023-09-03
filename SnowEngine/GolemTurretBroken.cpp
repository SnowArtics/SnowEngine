#include "GolemTurretBroken.h"

#include "AI.h"
#include "snAnimator.h"
#include "snSceneManager.h"
#include "DungeonMapManager.h"
#include "Projectile.h"
#include "snResources.h"
#include "snTime.h"
#include "snMesh.h"
#include "snMeshRenderer.h"
#include "snMaterial.h"
#include "MonsterHPBar.h"
#include "snPlayer.h"
#include "PlayerHP.h"

using namespace sn;

GolemTurretBroken::GolemTurretBroken()
	:shootTime(0.f)
{
}

GolemTurretBroken::~GolemTurretBroken()
{
}

void GolemTurretBroken::Initialize()
{
	Monster::Initialize();
}

void GolemTurretBroken::Update()
{
	AI* ai = GetComponent<AI>();
	MonDir monDir = ai->GetCurDir();
	MON_STATE monState = ai->GetCurStateName();

	Transform* monTr = this->GetComponent <Transform>();
	sn::Collider2D* collider = this->GetComponent<sn::Collider2D>();

	Animator* animator = GetComponent<Animator>();

	if (monState == MON_STATE::ATT) {
		shootTime += Time::DeltaTime();
	}

	if (monDir != ai->GetPrevDir() || monState != ai->GetPrevStateName()) {
		switch (monState)
		{
		case MON_STATE::IDLE:
			break;
		case MON_STATE::WALK:
			break;
		case MON_STATE::TRACE:
		{
			shootTime = 0.f;
			switch (monDir)
			{
			case MonDir::UP:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_UP", true);
				break;
			case MonDir::DOWN:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_DOWN", true);
				break;
			case MonDir::RIGHT:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_RIGHT", true);
				break;
			case MonDir::LEFT:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_LEFT", true);
				break;
			default:
				break;
			}
		}
		break;
		case MON_STATE::ATT:
		{
			
			switch (monDir)
			{
			case MonDir::UP:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_SHOT_UP", true);
				break;
			case MonDir::DOWN:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_SHOT_DOWN", true);
				break;
			case MonDir::RIGHT:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_SHOT_RIGHT", true);
				break;
			case MonDir::LEFT:
				animator->PlayAnimation(L"GOLEM_TURRET_BROKEN_SHOT_LEFT", true);
				break;
			default:
				break;
			}			
		}
		break;
		case MON_STATE::TARGET_ATT:
			break;
		case MON_STATE::DEAD:
			break;
		case MON_STATE::END:
			break;
		default:
			break;
		}
	}	

	if (shootTime >= 0.5f) {
		CreateProjectile();
		shootTime -= 0.5f;
	}

	Monster::Update();
}

void GolemTurretBroken::LateUpdate()
{
	Monster::LateUpdate();
}

void GolemTurretBroken::Render()
{
	Monster::Render();
}

void GolemTurretBroken::OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me)
{
	Monster::OnCollisionEnter(other, me);
	if (other->GetName() == L"SecondCollider") {
		tMonInfo monInfo = GetMonsterInfo();
		monInfo.fHP -= 30.f;
		this->GetComponent<MonsterHPBar>()->PlayDamage(30.f);
		this->GetComponent<MonsterHPBar>()->SetEnable(true);
		SetMonsterInfo(monInfo);


		if (monInfo.fHP <= 0.f) {
			std::vector<std::vector<RoomInfo>>& vecRoomInfo = DungeonMapManager::GetInst()->GetRoomInfoArr();
			vecRoomInfo[monsterMapPos.first][monsterMapPos.second].monsterNum -= 1;

			if (vecRoomInfo[monsterMapPos.first][monsterMapPos.second].monsterNum <= 0) {
				vecRoomInfo[monsterMapPos.first][monsterMapPos.second].clear = true;
				DungeonMapManager::GetInst()->SetRoomInfoArr(vecRoomInfo);
				DungeonMapManager::GetInst()->SetDungeonClear(true);
			}
		}
	}
	if (other->GetName() == L"FisrtCollider") {
		snPlayer* player = static_cast<snPlayer*>(other->GetOwner());
		float playerHP = player->GetHP();
		playerHP -= 30.f;
		player->SetHP(playerHP);
		PlayerHP* playerHPComponent = player->GetComponent<PlayerHP>();
		playerHPComponent->PlayDamage(30.f);
	}
}

void GolemTurretBroken::OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me)
{
}

void GolemTurretBroken::OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me)
{
}

void GolemTurretBroken::CreateProjectile()
{
	AI* ai = GetComponent<AI>();
	MonDir monDir = ai->GetCurDir();	
	
	Projectile* projectile = new Projectile();
	projectile->SetName(L"BrokenTurretProjectile");
	Transform* tr = projectile->GetComponent<Transform>();
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::Projectile, static_cast<GameObject*>(projectile));

	sn::Collider2D* collider = projectile->AddComponent<sn::Collider2D>();
	collider->SetSize(Vector2(0.1f, 0.1f));

	MeshRenderer* mr = projectile->AddComponent<MeshRenderer>();
	mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

	std::shared_ptr<Texture> atlas
		= Resources::Load<Texture>(L"Golem_Turret_Projectile", L"..\\Resources\\Texture\\Dungeon\\Enemy\\GolemTurretProjectile\\Golem_Turret_Projectile.png");
	Animator* at = projectile->AddComponent<Animator>();

	at->Create(L"TURRET_PROJECTILE_COLLISION_DOWN", atlas, Vector2(0.0f, 0.0f), Vector2(22.0f, 22.0f), 5, 0.05f);
	at->Create(L"TURRET_PROJECTILE_COLLISION_LEFT", atlas, Vector2(0.0f, 22.0f), Vector2(22.0f, 22.0f), 5, 0.05f);
	at->Create(L"TURRET_PROJECTILE_COLLISION_RIGHT", atlas, Vector2(0.0f, 44.0f), Vector2(22.0f, 22.0f), 5, 0.05f);
	at->Create(L"TURRET_PROJECTILE_COLLISION_UP", atlas, Vector2(0.0f, 66.0f), Vector2(22.0f, 22.0f), 5, 0.05f);

	at->Create(L"GOLEM_TURRET_PROJECTILE_DOWN", atlas, Vector2(0.0f, 88.0f), Vector2(22.0f, 22.0f), 6);
	at->Create(L"GOLEM_TURRET_PROJECTILE_LEFT", atlas, Vector2(0.0f, 110.0f), Vector2(22.0f, 22.0f), 6);
	at->Create(L"GOLEM_TURRET_PROJECTILE_RIGHT", atlas, Vector2(0.0f, 132.0f), Vector2(22.0f, 22.0f), 6);
	at->Create(L"GOLEM_TURRET_PROJECTILE_UP", atlas, Vector2(0.0f, 154.0f), Vector2(22.0f, 22.0f), 6);
	
	projectile->SetSpeed(2.5f);


	Vector3 projectilePos = tr->GetPosition();
	projectilePos = this->GetComponent<Transform>()->GetPosition();
	switch (monDir)
	{
	case MonDir::UP:
		at->PlayAnimation(L"GOLEM_TURRET_PROJECTILE_UP", true);
		projectile->SetDir(MonDir::UP);
		projectilePos.y += 0.3f;
		break;
	case MonDir::DOWN:
		at->PlayAnimation(L"GOLEM_TURRET_PROJECTILE_DOWN", true);
		projectile->SetDir(MonDir::DOWN);
		projectilePos.y -= 0.3f;
		break;
	case MonDir::RIGHT:
		at->PlayAnimation(L"GOLEM_TURRET_PROJECTILE_RIGHT", true);
		projectile->SetDir(MonDir::RIGHT);
		projectilePos.x += 0.3f;
		break;
	case MonDir::LEFT:
		at->PlayAnimation(L"GOLEM_TURRET_PROJECTILE_LEFT", true);
		projectile->SetDir(MonDir::LEFT);
		projectilePos.x -= 0.3f;
		break;
	default:
		break;
	}

	tr->SetPosition(projectilePos);
}
