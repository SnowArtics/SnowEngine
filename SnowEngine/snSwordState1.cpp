#include "snSwordState1.h"
#include "..\Engine_SOURCE\snGameObject.h"
#include "..\Engine_SOURCE\snTransform.h"
#include "..\Engine_SOURCE\snTime.h"
#include "..\Engine_SOURCE\snInput.h"
#include "snPlayerFSM.h"
#include "..\Engine_SOURCE\snCollider2D.h"
#include "snComponent.h"

using namespace sn;

SwordState1::SwordState1()
	:PlayerState(PLAYER_STATE::ATTSWORD1)
	, time(0.f)
	, attTime(0.45f)
	, flag(false)
{
}

SwordState1::~SwordState1()
{
}

void SwordState1::Update()
{
	time += Time::DeltaTime();

	PLAYER_DIR playerDir = GetPlayerFSM()->GetDirection();
	Transform* tr = GetPlayerFSM()->GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	std::vector<sn::Collider2D*> cd = GetPlayerFSM()->GetOwner()->GetComponents<sn::Collider2D>();

	if (playerDir == PLAYER_DIR::UP) {
		cd[1]->SetSize(Vector2(0.8f, 0.5f));
		cd[1]->SetCenter(Vector2(0.f, 0.2f));
	}
	else if (playerDir == PLAYER_DIR::DOWN) {
		cd[1]->SetSize(Vector2(0.8f, 0.5f));
		cd[1]->SetCenter(Vector2(0.f, -0.2f));
	}
	else if (playerDir == PLAYER_DIR::LEFT) {
		cd[1]->SetSize(Vector2(0.5f, 0.8f));
		cd[1]->SetCenter(Vector2(-0.2f, -0.1f));
	}
	else if (playerDir == PLAYER_DIR::RIGHT) {
		cd[1]->SetSize(Vector2(0.5f, 0.8f));
		cd[1]->SetCenter(Vector2(0.2f, -0.1f));
	}

	if (Input::GetKey(eKeyCode::W) && playerDir == PLAYER_DIR::UP && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.y += 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
		cd[1]->SetSize(Vector2(0.8f, 0.5f));
		cd[1]->SetCenter(Vector2(0.f, 0.2f));
	}
	else if (Input::GetKey(eKeyCode::S) && playerDir == PLAYER_DIR::DOWN && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.y -= 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
		cd[1]->SetSize(Vector2(0.8f, 0.5f));
		cd[1]->SetCenter(Vector2(0.f, -0.2f));
	}
	else if (Input::GetKey(eKeyCode::A) && playerDir == PLAYER_DIR::LEFT && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.x -= 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
		cd[1]->SetSize(Vector2(0.5f, 0.8f));
		cd[1]->SetCenter(Vector2(-0.2f, -0.1f));
	}
	else if (Input::GetKey(eKeyCode::D) && playerDir == PLAYER_DIR::RIGHT && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.x += 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
		cd[1]->SetSize(Vector2(0.5f, 0.8f));
		cd[1]->SetCenter(Vector2(0.2f, -0.1f));
	}

	if (time >= 0.1f && time < (attTime - 0.1f)) {
		cd[1]->SetEnable(true);
	}
	else {
		cd[1]->SetEnable(false);
	}

	if (Input::GetKeyDown(eKeyCode::J)&&flag == false) {
		flag = true;
	}
	else if (time > attTime) {
		if (flag == true) {
			GetPlayerFSM()->ChangeState(PLAYER_STATE::ATTSWORD2);
		}
		else {
			GetPlayerFSM()->ChangeState(PLAYER_STATE::IDLE);
		}
	}
}

void SwordState1::Enter()
{
}

void SwordState1::Exit()
{
	time = 0.f;
	flag = false;
}
