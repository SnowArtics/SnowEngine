#include "snCameraScript.h"
#include "snTransform.h"
#include "snGameObject.h"
#include "snTime.h"

namespace sn
{
	void CameraScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		pos.x += 1.0f * Time::DeltaTime();
		tr->SetPosition(pos);
	}
}