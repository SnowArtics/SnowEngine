#pragma once
#include "snComponent.h"


namespace sn
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
	};
}