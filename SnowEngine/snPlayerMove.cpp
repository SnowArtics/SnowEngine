#include "snPlayerMove.h"
#include "snTransform.h"
#include "snGameObject.h"
#include "snTime.h"
#include "snInput.h"
#include "snAnimator.h"

namespace sn {
	PlayerMove::PlayerMove()
	{
	}
	PlayerMove::~PlayerMove()
	{
	}
	void PlayerMove::Initialize()
	{
		Animator* at = GetOwner()->GetComponent<Animator>();
		at->CompleteEvent(L"Idle") = std::bind(&PlayerMove::Complete, this);
	}
	void PlayerMove::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		if (Input::GetKey(eKeyCode::LEFT))
		{
			pos.x -= 5.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::RIGHT))
		{
			pos.x += 5.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::DOWN))
		{
			pos.y -= 5.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::UP))
		{
			pos.y += 5.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}
	}
	void PlayerMove::LateUpdate()
	{
	}
	void PlayerMove::Render()
	{
	}
	void PlayerMove::Complete()
	{
		int a = 0;
	}
	void PlayerMove::OnCollisionEnter(Collider2D* other)
	{
	}
	void PlayerMove::OnCollisionStay(Collider2D* other)
	{
	}
	void PlayerMove::OnCollisionExit(Collider2D* other)
	{
	}
}