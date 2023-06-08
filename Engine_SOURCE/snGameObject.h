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
		//std::vector<Component*> mComponents; ������ ������Ʈ�� �� �� ���̴�.
		//int y;	//���ϴ´�� ��ǥ���� �ΰ� �������
		//int x;
	};
}
