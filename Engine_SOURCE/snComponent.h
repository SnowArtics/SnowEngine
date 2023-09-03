#pragma once
#include "snEntity.h"

namespace sn
{
	using namespace sn::enums;
	using namespace sn::math;

	class GameObject;
	class Component : public Entity //snEntity�� ��ӹ޴´�.
	{
	public:
		Component(eComponentType type);
		virtual ~Component();

		virtual void Initialize();
		//virtual void FixedUpdate()//���� ���� ������Ʈ�� ����. ���� ������ ������� �������� ���д�.
		virtual void Update();//�׳� ������Ʈ�� ����
		virtual void LateUpdate();//������ ���� ������Ʈ�� ����
		virtual void Render();//update�Ϸ� �� ȭ�鿡 �׷���.

		GameObject* GetOwner() { return mOwner; }
		void SetOwner(GameObject* owner) { mOwner = owner; }

	private:
		const eComponentType mType;
		GameObject* mOwner;
	};
}