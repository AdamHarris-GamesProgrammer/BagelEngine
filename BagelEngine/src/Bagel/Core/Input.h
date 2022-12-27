#pragma once

#include "Bagel/Core/Core.h"

namespace Bagel {

	class BG_API Input {

	public:
		inline static bool IsKeyPress(int keycode) { return _instance->IsKeyPressImpl(keycode); }
		inline static bool IsKeyUp(int keycode) { return _instance->IsKeyUpImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return _instance->IsMouseButtonPressedImpl(button); }
		inline static float GetMouseX() { return _instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return _instance->GetMouseYImpl(); }
		inline static std::pair<float, float> GetMousePosition() { return _instance->GetMousePositionImpl(); }


	protected:
		virtual bool IsKeyPressImpl(int keycode) = 0;
		virtual bool IsKeyUpImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;

	private:
		static Input* _instance;

	};
}
