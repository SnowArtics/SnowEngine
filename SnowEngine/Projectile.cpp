#include "Projectile.h"

#include "AI.h"
#include "snTransform.h"
#include "snTime.h"
#include "snAnimator.h"
#include "snPlayer.h"
#include "PlayerHP.h"

Projectile::Projectile()
	:dir(MonDir::UP)
	, speed(1.0f)
	, dead(false)
	, deadTime(0.f)
{
}

Projectile::~Projectile()
{
}

void Projectile::Initialize()
{
	GameObject::Initialize();
}

void Projectile::Update()
{
	Transform* tr = this->GetComponent<Transform>();
	Vector3 projectilePos = tr->GetPosition();
	Animator* at = this->GetComponent<Animator>();	

	if (!dead) {
		switch (dir)
		{
		case MonDir::UP:
			projectilePos.y += speed * Time::DeltaTime();
			break;
		case MonDir::DOWN:
			projectilePos.y -= speed * Time::DeltaTime();
			break;
		case MonDir::RIGHT:
			projectilePos.x += speed * Time::DeltaTime();
			break;
		case MonDir::LEFT:
			projectilePos.x -= speed * Time::DeltaTime();
			break;
		default:
			break;
		}

		tr->SetPosition(projectilePos);
	}
	else {
		//if (deadTime == 0.f) {
		//	switch (dir)
		//	{
		//	case MonDir::UP:
		//		at->PlayAnimation(L"TURRET_PROJECTILE_COLLISION_UP", false);
		//		break;
		//	case MonDir::DOWN:
		//		at->PlayAnimation(L"TURRET_PROJECTILE_COLLISION_DOWN", false);
		//		break;
		//	case MonDir::RIGHT:
		//		at->PlayAnimation(L"TURRET_PROJECTILE_COLLISION_RIGHT", false);
		//		break;
		//	case MonDir::LEFT:
		//		at->PlayAnimation(L"TURRET_PROJECTILE_COLLISION_LEFT", false);
		//		break;
		//	default:
		//		break;
		//	}
		//}		
		deadTime += Time::DeltaTime();
		if (deadTime >= 0.5f) {
			this->SetState(eState::Dead);
			deadTime = 0.f;
		}
	}

	GameObject::Update();
}

void Projectile::LateUpdate()
{
	GameObject::LateUpdate();
}

void Projectile::Render()
{
	GameObject::Render();
}

void Projectile::OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me)
{
	/*if(other->GetOwner() != this->GetComponent<Transform>()->GetParent()->GetOwner())*/
	if (other->GetName() == L"FisrtCollider") {
		snPlayer* player = static_cast<snPlayer*>(other->GetOwner());
		float playerHP = player->GetHP();
		playerHP -= 30.f;
		player->SetHP(playerHP);
		PlayerHP* playerHPComponent = player->GetComponent<PlayerHP>();
		playerHPComponent->PlayDamage(30.f);

		dead = true;
		this->SetState(eState::Dead);
	}
	else if (other->GetName() != L"ProjectileCollider") {
		dead = true;
		this->SetState(eState::Dead);
	}
}

void Projectile::OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me)
{
}

void Projectile::OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me)
{
	std::wstring name = other->GetOwner()->GetName();
}
