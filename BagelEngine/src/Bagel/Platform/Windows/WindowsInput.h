#pragma once
#include "Bagel/Input.h"



namespace Bagel {
	class WindowsInput : public Input {
	protected:
		// Inherited via Input
		virtual bool IsKeyPressImpl(int keycode) override;
		virtual bool IsKeyUpImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMousePositionImpl() override;

	};
}