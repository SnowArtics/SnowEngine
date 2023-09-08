#pragma once
#include "guiComponent.h"


namespace gui
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		sn::math::Vector3 mPosition;
		sn::math::Vector3 mRotation;
		sn::math::Vector3 mScale;
	};
}



