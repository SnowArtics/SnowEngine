#include "GolemSmashWave.h"

#include "snGameObject.h"
#include "snTime.h"
#include "snPlayer.h"
#include "snSceneManager.h"

using namespace sn;

GolemSmashWave::GolemSmashWave()
	: time(0.0f)
{
}

GolemSmashWave::~GolemSmashWave()
{
}

void GolemSmashWave::Initialize()
{
	GameObject::Initialize();
}

void GolemSmashWave::Update()
{
	time += Time::DeltaTime();

	if (time >= 10.f) {
		SetState(sn::GameObject::eState::Dead);
	}

	Vector3 scaleIncrease = Vector3(3.0f * Time::DeltaTime(), 3.0f * Time::DeltaTime(), 0.f);

	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();
	Vector3 scale = tr->GetScale();

	tr->SetScale(scale + scaleIncrease);

	Transform* playerTr = SceneManager::GetActiveScene()->GetPlayer()->GetComponent<Transform>();
	Vector3 playerPos = playerTr->GetPosition();

	Vector3 playerDir = playerPos - pos;


	GameObject::Update();
}

void GolemSmashWave::LateUpdate()
{
	GameObject::LateUpdate();
}

void GolemSmashWave::Render()
{
	GameObject::Render();
}

void GolemSmashWave::OnCollisionEnter(sn::Collider2D* other)
{
}

void GolemSmashWave::OnCollisionStay(sn::Collider2D* other)
{
}

void GolemSmashWave::OnCollisionExit(sn::Collider2D* other)
{
}
