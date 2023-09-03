#include "HitEffect.h"

#include <random>

#include "snMeshRenderer.h"
#include "snResources.h"
#include "snAnimator.h"
#include "snTime.h"

using namespace sn;

HitEffect::HitEffect()
	:hitState(HitState::SWORD)
	, playTime(0.f)
{
	MeshRenderer* mr = this->AddComponent<MeshRenderer>();
	mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

	Animator* at = this->AddComponent<Animator>();
	std::shared_ptr<Texture> atlas
		= Resources::Load<Texture>(L"Sword_Effect01", L"..\\Resources\\Texture\\Dungeon\\HitEffect\\swordEffect1.png");
	at->Create(L"SWORD_EFFECT01", atlas, Vector2(0.0f, 0.0f), Vector2(128.f, 128.0f), 3);

	atlas
		= Resources::Load<Texture>(L"Sword_Effect02", L"..\\Resources\\Texture\\Dungeon\\HitEffect\\swordEffect2.png");
	at->Create(L"SWORD_EFFECT02", atlas, Vector2(0.0f, 0.0f), Vector2(128.f, 128.f), 3);

	atlas
		= Resources::Load<Texture>(L"bow_Effect01", L"..\\Resources\\Texture\\Dungeon\\HitEffect\\bowEffect1.png");
	at->Create(L"BOW_EFFECT01", atlas, Vector2(0.0f, 0.0f), Vector2(128.f, 128.f), 3);

	at->PlayAnimation(L"SWORD_EFFECT01", false);

	this->SetEnable(false);
}

HitEffect::~HitEffect()
{
}

void HitEffect::Initialize()
{
	this->SetEnable(false);

	GameObject::Initialize();
}

void HitEffect::Update()
{
	if (playTime != 0.f) {
		playTime += Time::DeltaTime();

		if (playTime >= 0.3f) {
			playTime = 0.f;
			this->SetEnable(false);
		}
	}

	GameObject::Update();
}

void HitEffect::LateUpdate()
{
	GameObject::LateUpdate();
}

void HitEffect::Render()
{
	GameObject::Render();
}

void HitEffect::OnCollisionEnter(sn::Collider2D* other)
{
}

void HitEffect::OnCollisionStay(sn::Collider2D* other)
{
}

void HitEffect::OnCollisionExit(sn::Collider2D* other)
{
}

void HitEffect::PlaySwordEffect()
{
	if (playTime == 0.f) {
		std::random_device rd;
		std::mt19937 gen(rd());  // Mersenne Twister 엔진 사용

		// 원하는 범위 내의 랜덤 정수 생성
		std::uniform_int_distribution<> distribution(1, 2);  // 1부터 100까지의 정수

		int randomNum = distribution(gen);

		Animator* at = this->GetComponent<Animator>();

		if (randomNum == 1) {
			at->PlayAnimation(L"SWORD_EFFECT01", false);
		}
		else if (randomNum == 2) {
			at->PlayAnimation(L"SWORD_EFFECT02", false);
		}

		playTime = 0.001f;
		this->SetEnable(true);
	}
	
}

void HitEffect::PlayBowEffect()
{
}
