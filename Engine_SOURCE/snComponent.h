#pragma once
#include "snEntity.h"

namespace sn
{
	using namespace sn::enums;

	class Component : public Entity //snEntity를 상속받는다.
	{
	public:
		Component();
		~Component();

		virtual void Initialize();
		//virtual void FixedUpdate()//물리 관련 업데이트는 여기. 지금 당장은 사용하지 않음으로 빼둔다.
		virtual void Update();//그냥 업데이트는 여기
		virtual void LateUpdate();//렌더링 관련 업데이트는 여기
		virtual void Render();//update완료 후 화면에 그려줌.

	private:
		const eComponentType mType;
	};
}