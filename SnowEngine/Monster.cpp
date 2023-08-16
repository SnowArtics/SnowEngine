#include "Monster.h"
#include "../Engine_SOURCE/AI.h"
#include "../Engine_SOURCE/snCollider2D.h"

using namespace sn;

Monster::Monster()
	:m_tInfo{}
	,monsterMapPos({-1,-1})
{
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
	if (m_tInfo.fHP <= 0.f) {
		SetState(eState::Dead);
		return;
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

void Monster::OnCollisionEnter(sn::Collider2D* other)
{
}

void Monster::OnCollisionStay(sn::Collider2D* other)
{
}

void Monster::OnCollisionExit(sn::Collider2D* other)
{
}
