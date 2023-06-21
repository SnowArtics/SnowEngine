#pragma once
#include "snComponent.h"
#include "snGraphics.h"

namespace sn
{
	class Camera : public Component
	{
	public:
		Camera();
		~Camera();

	private:
		static Matrix mView;
		static Matrix mProjection;
	};
}
