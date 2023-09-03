#include "PlayerHP.h"

#include "snGameObject.h"
#include "snSceneManager.h"
#include "snTransform.h"
#include "snMeshRenderer.h"
#include "snResources.h"
#include "snMesh.h"
#include "snMaterial.h"
#include "snPlayer.h"
#include "snTime.h"

using namespace sn;

PlayerHP::PlayerHP()
	: Component(eComponentType::MonsterHPBar)
	, playerHPBarRed(nullptr)
	, playerHPBarWhite(nullptr)
	, playerHitScreen(nullptr)
	, fakeHP(0.0f)
	, realHP(0.0f)
	, fullHP(0.0f)
	, hitTime(0.0f)
	, absoluteTime(0.f)
{
}

PlayerHP::~PlayerHP()
{
	if(playerHPBarRed)
		playerHPBarRed->SetState(sn::GameObject::eState::Dead);

	if(playerHPBarWhite)
		playerHPBarWhite->SetState(sn::GameObject::eState::Dead);

	if(playerHitScreen)
		playerHitScreen->SetState(sn::GameObject::eState::Dead);
}

void PlayerHP::Initialize()
{
}

void PlayerHP::Update()
{
	if (!(playerHPBarRed && playerHPBarWhite && playerHitScreen))
		return;

	if (playerHitScreen->GetEnable() == false) {
		hitTime = 0.f;
	}
	else {
		hitTime += Time::DeltaTime();
	}

	if (playerHitScreen->GetEnable() == true && hitTime > 0.05f) {
		playerHitScreen->SetEnable(false);
	}

	if (absoluteTime >= 0.1f) {
		absoluteTime += Time::DeltaTime();
	}

	if (absoluteTime >= 1.f) {
		sn::Collider2D* playerCol = GetOwner()->GetComponent<sn::Collider2D>();
		playerCol->SetEnable(true);
		absoluteTime = 0.f;
	}

	float realHPGauge = realHP / fullHP;
	float fakeHPGauge = 0.0f;

	if (abs(realHP - fakeHP) > 0.2f) {
		fakeHP -= 0.2f;
	}

	fakeHPGauge = fakeHP / fullHP;

	Transform* playerHPBarWhiteTr = playerHPBarWhite->GetComponent<Transform>();
	playerHPBarWhiteTr->SetScale(Vector3(fakeHPGauge*1.15f, 0.21f, 0.0f));
	playerHPBarWhiteTr->SetPosition(Vector3(-2.89f-(((1.0f-fakeHPGauge) * 1.15f)/2), 2.36f, 0.0f));

	Transform* playerHPBarRedTr = playerHPBarRed->GetComponent<Transform>();
	playerHPBarRedTr->SetScale(Vector3(realHPGauge*1.15f, 0.21f, 0.0f));
	playerHPBarRedTr->SetPosition(Vector3(-2.89f - (((1.0f-realHPGauge) * 1.15f)/2), 2.36f, 0.0f));

}

void PlayerHP::LateUpdate()
{
}

void PlayerHP::Render()
{
}

void PlayerHP::CreateHpBar()
{
	realHP = static_cast<snPlayer*>(GetOwner())->GetHP();
	fakeHP = realHP;
	fullHP = realHP;

	float HPGauge = fullHP / realHP;

	//WhiteBar刚历 积己
	sn::GameObject* mon = this->GetOwner();
	Transform* monTr = mon->GetComponent<Transform>();
	Vector3 monScale = monTr->GetScale();

	playerHPBarWhite = new sn::GameObject();
	playerHPBarWhite->SetName(L"playHPBarWhite");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::UI, playerHPBarWhite);
	Transform* playerHPBarWhiteTr = playerHPBarWhite->GetComponent<Transform>();
	playerHPBarWhiteTr->SetScale(Vector3(1.15f, 0.21f, 0.0f));
	playerHPBarWhiteTr->SetPosition(Vector3(-2.89f, 2.36f, 0.0f));

	MeshRenderer* barMeshRenderer01 = playerHPBarWhite->AddComponent<MeshRenderer>();
	barMeshRenderer01->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer01->SetMaterial(Resources::Find<Material>(L"PlayerHPBarWhite"));

	//WhiteBar刚历 积己
	playerHPBarRed = new sn::GameObject();
	playerHPBarRed->SetName(L"playHPBarWhite");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::UI, playerHPBarRed);
	Transform* playerHPBarRedTr = playerHPBarRed->GetComponent<Transform>();
	playerHPBarRedTr->SetScale(Vector3(1.15f, 0.21f, 0.0f));
	playerHPBarRedTr->SetPosition(Vector3(-2.89f, 2.36f, 0.0f));

	MeshRenderer* barMeshRenderer02 = playerHPBarRed->AddComponent<MeshRenderer>();
	barMeshRenderer02->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer02->SetMaterial(Resources::Find<Material>(L"PlayerHPBarRed"));

	//RedBar 积己
	playerHitScreen = new sn::GameObject();
	playerHitScreen->SetName(L"playerHitScreen");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::UI, playerHitScreen);
	Transform* playerHitScreenTr = playerHitScreen->GetComponent<Transform>();
	playerHitScreenTr->SetScale(Vector3(10.f, 10.f, 0.0f));
	playerHitScreenTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	MeshRenderer* barMeshRenderer03 = playerHitScreen->AddComponent<MeshRenderer>();
	barMeshRenderer03->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	barMeshRenderer03->SetMaterial(Resources::Find<Material>(L"PlayerHitScreen"));

	playerHitScreen->SetEnable(false);
}

void PlayerHP::PlayDamage(float _damage)
{
	fakeHP = realHP;
	realHP -= _damage;

	playerHitScreen->SetEnable(true);

	sn::Collider2D* playerCol = GetOwner()->GetComponent<sn::Collider2D>();
	playerCol->SetEnable(false);

	absoluteTime = 0.1f;

	if (realHP < 1.f)
		realHP = 1.f;
}
