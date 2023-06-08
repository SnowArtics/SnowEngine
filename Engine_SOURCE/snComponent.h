#pragma once
#include "snEntity.h"

namespace sn
{
	using namespace sn::enums;

	class Component : public Entity //snEntity�� ��ӹ޴´�.
	{
	public:
		Component();
		~Component();

		virtual void Initialize();
		//virtual void FixedUpdate()//���� ���� ������Ʈ�� ����. ���� ������ ������� �������� ���д�.
		virtual void Update();//�׳� ������Ʈ�� ����
		virtual void LateUpdate();//������ ���� ������Ʈ�� ����
		virtual void Render();//update�Ϸ� �� ȭ�鿡 �׷���.

	private:
		const eComponentType mType;
	};
}