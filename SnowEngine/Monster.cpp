#include "Monster.h"
#include "../Engine_SOURCE/AI.h"
#include "../Engine_SOURCE/snCollider2D.h"
#include <snMeshRenderer.h>
#include "snTime.h"
#include "snRigidBody.h"
#include "HitEffect.h"
#include "snSceneManager.h"

using namespace sn;

Monster::Monster()
	:m_tInfo{}
	, monsterMapPos({ -1,-1 })
	, time(0.f)
	,hitTime(0.f)
	, deadTime(0.f)
{
	AddHitEffect(Vector3(0.25f,0.25f,0.25f));
}

Monster::~Monster()
{
}

void Monster::Initialize()
{
	GameObject::Initialize();
}

void Monster::Update()
{
	int monsterState = this->GetComponent<MeshRenderer>()->GetMonsterCB().state;

	if (m_tInfo.fHP <= 0.f) {
		for (int i = 0; i < GetComponents<sn::Collider2D>().size(); i++) {
			GetComponents<sn::Collider2D>()[i]->SetEnable(false);
		}
		if (hitEffect != nullptr) {
			//delete hitEffect;
			hitEffect->SetState(eState::Dead);
		}
		monsterState = 2;
	}

	//0 is normal, 1 is hit, 2 is dead

	if (monsterState == 1) {
		hitTime += Time::DeltaTime();
		MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
		renderer::MonsterCB monsterCB = {};
		monsterCB.state = 1;
		monsterCB.color = Vector4(0.8f,0.0f,0.0f,1.0f);
		meshRenderer->SetMonsterCB(monsterCB);

		if (hitTime > 0.1f) {
			MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
			renderer::MonsterCB monsterCB = {};
			monsterCB.state = 1;
			monsterCB.color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
			meshRenderer->SetMonsterCB(monsterCB);
		}

		if (hitTime > 0.2f) {
			hitTime = 0.f;
			MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
			renderer::MonsterCB monsterCB = {};
			monsterCB.state = 0;
			monsterCB.color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
			meshRenderer->SetMonsterCB(monsterCB);
		}
	}
	else if (monsterState == 2) {
		deadTime += Time::DeltaTime();
		MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
		renderer::MonsterCB monsterCB = {};
		monsterCB.state = 2;
		monsterCB.color = Vector4(0.8f, 0.0f, 0.0f, 1.0f);
		meshRenderer->SetMonsterCB(monsterCB);

		if (deadTime > 0.2f) {
			MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
			renderer::MonsterCB monsterCB = {};
			monsterCB.state = 2;
			monsterCB.color = Vector4(0.8f, 0.4f, 0.4f, 1.0f);
			meshRenderer->SetMonsterCB(monsterCB);
		}

		if (deadTime > 0.4f) {
			MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
			renderer::MonsterCB monsterCB = {};
			monsterCB.state = 2;
			monsterCB.color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
			meshRenderer->SetMonsterCB(monsterCB);
		}

		if (deadTime > 1.f) {
			deadTime = 0.f;
			this->SetState(eState::Dead);
		}
	}

	GameObject::Update();
}

void Monster::LateUpdate()
{
	GameObject::LateUpdate();
}

void Monster::Render()
{
	GameObject::Render();
}

void Monster::OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me)
{
	if (other->GetName() == L"SecondCollider") {
		MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
		renderer::MonsterCB monsterCB = {};
		monsterCB.state = 1;
		meshRenderer->SetMonsterCB(monsterCB);

		Monster* monster = this;

		if (monster) {
			RigidBody* rigidBody = monster->GetComponent<RigidBody>();
			if (rigidBody) {
				Transform* playerTr = other->GetOwner()->GetComponent<Transform>();
				Vector3 playerPos = playerTr->GetPosition();

				Transform* monTr = monster->GetComponent<Transform>();
				Vector3 monPos = monTr->GetPosition();

				Vector3 dir = monPos - playerPos;
				float stiffness = monster->GetMonsterInfo().fUnStiffness;

				RigidBody* rigidBody = monster->GetComponent<RigidBody>();
				rigidBody->SetVelocity(dir * stiffness);
			}
		}

		if (hitEffect!= nullptr) {
			hitEffect->PlaySwordEffect();
		}
	}
}

void Monster::OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me)
{
}

void Monster::OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me)
{
	//if (other->GetName() == L"SecondCollider") {		
	//	MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
	//	renderer::MonsterCB monsterCB = {};
	//	monsterCB.state = 0;
	//	meshRenderer->SetMonsterCB(monsterCB);
	//}
}

void Monster::AddHitEffect(Vector3 size)
{
	if (hitEffect != nullptr)
		return;
	hitEffect = new HitEffect;
	hitEffect->SetName(L"HitEffect");
	SceneManager::GetActiveScene()->AddGameObject(eLayerType::HitEffect, static_cast<GameObject*>(hitEffect));
	Transform* hitEffectTr = hitEffect->GetComponent<Transform>();
	Transform* monTr = this->GetComponent<Transform>();
	Vector3 monPos = monTr->GetPosition();

	hitEffectTr->SetParent(monTr);
	hitEffectTr->SetPosition(Vector3(monPos.x-0.05f, monPos.y, monPos.z));
	
	hitEffectTr->SetScale(size);
}
