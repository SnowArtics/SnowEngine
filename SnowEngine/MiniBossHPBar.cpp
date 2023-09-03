#include "MiniBossHPBar.h"

#include "snGameObject.h"
#include "snSceneManager.h"
#include "snTransform.h"
#include "snMeshRenderer.h"
#include "snResources.h"
#include "snMesh.h"
#include "snMaterial.h"
#include "Monster.h"
#include "snAnimator.h"
#include "snTime.h"

using namespace sn;

MiniBossHPBar::MiniBossHPBar()
	: Component(eComponentType::MonsterHPBar)
	, bossHPBracket(nullptr)
	, bossHPBar(nullptr)
	, bossEnterUI(nullptr)
	, fakeHP(0.0f)
	, realHP(0.0f)
	, fullHP(0.0f)
	, time(0.f)
{
}

MiniBossHPBar::~MiniBossHPBar()
{
	if(bossHPBracket)
		bossHPBracket->SetState(sn::GameObject::eState::Dead);

	if (bossHPBar)
		bossHPBar->SetState(sn::GameObject::eState::Dead);

	if (bossEnterUI)
		bossEnterUI->SetState(sn::GameObject::eState::Dead);
}

void MiniBossHPBar::Initialize()
{
}

void MiniBossHPBar::Update()
{
	if (!(bossHPBracket && bossHPBar && bossEnterUI))
		return;

	time += Time::DeltaTime();
	if (time >= 2.7f)
		bossEnterUI->SetEnable(false);

	float HPGauge = realHP / fullHP;

	Transform* bossHPBarTr = bossHPBar->GetComponent<Transform>();
	bossHPBarTr->SetScale(Vector3(HPGauge * 5.6f, 0.5f, 0.0f));
	bossHPBarTr->SetPosition(Vector3(0.f - (((1.0f - HPGauge) * 5.6f)/2), -2.2f, 0.0f));
}

void MiniBossHPBar::LateUpdate()
{
}

void MiniBossHPBar::Render()
{
}

void MiniBossHPBar::CreateHpBar()
{
	realHP = static_cast<Monster*>(GetOwner())->GetMonsterInfo().fHP;
	fakeHP = realHP;
	fullHP = realHP;

	float HPGauge = fullHP / realHP;

	sn::GameObject* mon = this->GetOwner();
	Transform* monTr = mon->GetComponent<Transform>();
	Vector3 monScale = monTr->GetScale();

	// 보스 몬스터 HP 괄호 생성
	bossHPBracket = new sn::GameObject();
	bossHPBracket->SetName(L"HPBar");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::UI, bossHPBracket);
	Transform* bossHPBracketTr = bossHPBracket->GetComponent<Transform>();
	bossHPBracketTr->SetPosition(Vector3(0.f, -2.2f, 0.f));
	bossHPBracketTr->SetScale(Vector3(5.736842105263158f, 0.2f, 0.0f));
	//hpBackBarTr->SetScale(Vector3(monScale.x/2.f, monScale.x*0.03f, 0.0f));

	MeshRenderer* barMeshRenderer01 = bossHPBracket->AddComponent<MeshRenderer>();
	barMeshRenderer01->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer01->SetMaterial(Resources::Find<Material>(L"BossMonsterHPBracket"));
	
	// 보스 몬스터 HP 바 생성
	bossHPBar = new sn::GameObject();
	bossHPBar->SetName(L"HPBar");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::UI, bossHPBar);
	Transform* bossHPBarTr = bossHPBar->GetComponent<Transform>();
	bossHPBarTr->SetPosition(Vector3(0.f, -2.2f, 0.f));
	bossHPBarTr->SetScale(Vector3(5.6f, 0.5f, 0.0f));

	MeshRenderer* barMeshRenderer02 = bossHPBar->AddComponent<MeshRenderer>();
	barMeshRenderer02->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer02->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

	std::shared_ptr<Texture> atlas
		= Resources::Load<Texture>(L"Golem_Mini_Boss_HP_Bar", L"..\\Resources\\Texture\\Dungeon\\BossHPBar\\Boss_HP.png");
	Animator* at = bossHPBar->AddComponent<Animator>();

	at->Create(L"MINI_BOSS_DAMAGED", atlas, Vector2(0.0f, 0.f), Vector2(1096.f, 27.f), 6, 120.f);

	at->PlayAnimation(L"MINI_BOSS_DAMAGED", false);

	// 보스 몬스터 룸 UI 생성
	bossEnterUI = new sn::GameObject();
	bossEnterUI->SetName(L"HPBar");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::UI, bossEnterUI);
	Transform* bossEnterUITr = bossEnterUI->GetComponent<Transform>();
	bossEnterUITr->SetPosition(Vector3(0.f, -1.5f, 0.f));
	bossEnterUITr->SetScale(Vector3(5.625f, 1.5f, 0.0f));

	MeshRenderer* barMeshRenderer03 = bossEnterUI->AddComponent<MeshRenderer>();
	barMeshRenderer03->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer03->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

	atlas
		= Resources::Load<Texture>(L"Golem_Mini_Boss_Room_Open", L"..\\Resources\\Texture\\Dungeon\\BossHPBar\\Boss_room.png");
	at = bossEnterUI->AddComponent<Animator>();

	at->Create(L"MINI_BOSS_ROOM_OPEN", atlas, Vector2(0.0f, 0.f), Vector2(195.f, 52.f), 26, 240.f);

	at->PlayAnimation(L"MINI_BOSS_ROOM_OPEN", false);
}

void MiniBossHPBar::PlayDamage(float _damage)
{
	fakeHP = realHP;
	realHP -= _damage;

	Animator* at = bossHPBar->GetComponent<Animator>();
	at->PlayAnimation(L"MINI_BOSS_DAMAGED", false);
}
