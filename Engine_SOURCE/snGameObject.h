#pragma once
#include "snEntity.h"
#include "snComponent.h"

namespace sn
{
	class GameObject : public Entity
	{
	public:
		enum eState
		{
			Active,
			Paused,
			Dead,
		};

		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	private:
		eState mState;
		//std::vector<Component*> mComponents; 아직은 컴포넌트를 안 둘 것이다.
		//int y;	//원하는대로 좌표값을 두고 사용하자
		//int x;
	};
}
