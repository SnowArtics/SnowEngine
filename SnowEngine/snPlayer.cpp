#include "snPlayer.h"

#include "Monster.h"
#include "snRigidBody.h"
#include "snTransform.h"

using namespace sn;

snPlayer::snPlayer()
	: HP(100.f)
	, UnStiffness(10.f)
{
}

snPlayer::~snPlayer()
{
}

void snPlayer::Initialize()
{
	sn::GameObject::Initialize();
}

void snPlayer::Update()
{
	sn::GameObject::Update();
}

void snPlayer::LateUpdate()
{
	sn::GameObject::LateUpdate();
}

void snPlayer::Render()
{
	sn::GameObject::Render();
}

void snPlayer::OnCollisionEnter(sn::Collider2D* other)
{
	
}

void snPlayer::OnCollisionStay(sn::Collider2D* other)
{
}

void snPlayer::OnCollisionExit(sn::Collider2D* other)
{
}
