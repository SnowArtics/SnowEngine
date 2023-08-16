#include "snCameraScript.h"
#include "snTransform.h"
#include "snGameObject.h"
#include "snTime.h"
#include "snInput.h"
#include "snSceneManager.h"

namespace sn
{
	void CameraScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		//if (SceneManager::GetActiveScene()->GetPlayer() != nullptr) {

		//	Transform* playerTr = SceneManager::GetActiveScene()->GetPlayer()->GetComponent<Transform>();
		//	Vector3 playerPos = playerTr->GetPosition();

		//	tr->SetPosition(Vector3(playerPos.x, playerPos.y, pos.z));
		//}

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
}