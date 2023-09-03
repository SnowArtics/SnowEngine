#include "MonsterHPBar.h"

#include "snGameObject.h"
#include "snSceneManager.h"
#include "snTransform.h"
#include "snMeshRenderer.h"
#include "snResources.h"
#include "snMesh.h"
#include "snMaterial.h"
#include "Monster.h"

using namespace sn;

MonsterHPBar::MonsterHPBar()
	:Component(eComponentType::None)
	, hpBackBar(nullptr)
	, hpMiddleBar(nullptr)
	, hpFrontBar(nullptr)
	, fakeHP(0.0f)
	, realHP(0.0f)
	, fullHP(0.0f)
{
}
	

MonsterHPBar::~MonsterHPBar()
{
	hpBackBar->SetState(sn::GameObject::eState::Dead);
	hpMiddleBar->SetState(sn::GameObject::eState::Dead);
	hpFrontBar->SetState(sn::GameObject::eState::Dead);	
}

void MonsterHPBar::Initialize()
{
}

void MonsterHPBar::Update()
{
	float realHPGauge = realHP / fullHP;
	float fakeHPGauge = 0.0f;

	if(abs(realHP-fakeHP)>0.3f){
		fakeHP -= 0.3f;
	}
	else {
		SetEnable(false);
	}

	fakeHPGauge = fakeHP / fullHP;

	sn::GameObject* mon = this->GetOwner();
	Transform* monTr = mon->GetComponent<Transform>();
	Vector3 monPos = monTr->GetPosition();

	Transform* hpBackBarTr = hpBackBar->GetComponent<Transform>();
	hpBackBarTr->SetPosition(Vector3(monPos.x, monPos.y+0.5f, monPos.z -1.0f));

	Transform* hpMiddleBarrTr = hpMiddleBar->GetComponent<Transform>();
	hpMiddleBarrTr->SetPosition(Vector3(monPos.x - ((1.0f - fakeHPGauge) / 4.f), monPos.y + 0.5f, monPos.z-1.0f));
	hpMiddleBarrTr->SetScale(Vector3(fakeHPGauge / 2.f, 0.03, 0.0f));

	Transform* hpFrontBarTr = hpFrontBar->GetComponent<Transform>();
	hpFrontBarTr->SetPosition(Vector3(monPos.x - ((1.0f - realHPGauge) / 4.f), monPos.y + 0.5f, monPos.z-1.0f));
	hpFrontBarTr->SetScale(Vector3(realHPGauge / 2.f, 0.03, 0.0f));
}

void MonsterHPBar::LateUpdate()
{
}

void MonsterHPBar::Render()
{
}

void MonsterHPBar::CreateHpBar()
{
	realHP = static_cast<Monster*>(GetOwner())->GetMonsterInfo().fHP;
	fakeHP = realHP;
	fullHP = realHP;

	float HPGauge = fullHP / realHP;

	sn::GameObject* mon = this->GetOwner();
	Transform* monTr = mon->GetComponent<Transform>();
	Vector3 monScale = monTr->GetScale();

	hpBackBar = new sn::GameObject();
	hpBackBar->SetName(L"HPBar");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::MonsterHPBar, hpBackBar);
	Transform* hpBackBarTr = hpBackBar->GetComponent<Transform>();
	hpBackBarTr->SetScale(Vector3(HPGauge / 2.f, 0.03, 0.0f));
	//hpBackBarTr->SetScale(Vector3(monScale.x/2.f, monScale.x*0.03f, 0.0f));

	MeshRenderer* barMeshRenderer01 = hpBackBar->AddComponent<MeshRenderer>();
	barMeshRenderer01->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer01->SetMaterial(Resources::Find<Material>(L"MonsterHPBackBar"));

	hpMiddleBar = new sn::GameObject();
	hpMiddleBar->SetName(L"HPBar");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::MonsterHPBar, hpMiddleBar);
	Transform* hpMiddleBarrTr = hpMiddleBar->GetComponent<Transform>();
	hpMiddleBarrTr->SetScale(Vector3(HPGauge / 2.f, 0.03, 0.0f));
	//hpMiddleBarrTr->SetScale(Vector3(monScale.x/2.f, monScale.x * 0.03f, 0.0f));

	MeshRenderer* barMeshRenderer02 = hpMiddleBar->AddComponent<MeshRenderer>();
	barMeshRenderer02->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer02->SetMaterial(Resources::Find<Material>(L"MonsterHPMiddleBar"));

	hpFrontBar = new sn::GameObject();
	hpFrontBar->SetName(L"HPBar");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::MonsterHPBar, hpFrontBar);
	Transform* hpFrontBarTr = hpFrontBar->GetComponent<Transform>();
	hpFrontBarTr->SetScale(Vector3(HPGauge / 2.f, 0.03, 0.0f));
	//hpFrontBarTr->SetScale(Vector3(monScale.x/2.f, monScale.x * 0.03f, 0.0f));

	MeshRenderer* barMeshRenderer03 = hpFrontBar->AddComponent<MeshRenderer>();
	barMeshRenderer03->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer03->SetMaterial(Resources::Find<Material>(L"MonsterHPFrontBar"));
}

void MonsterHPBar::PlayDamage(float _damage)
{
	fakeHP = realHP;
	realHP -= _damage;
}

void MonsterHPBar::SetEnable(bool _enable)
{
	hpBackBar->SetEnable(_enable);
	hpMiddleBar->SetEnable(_enable);
	hpFrontBar->SetEnable(_enable);
}
