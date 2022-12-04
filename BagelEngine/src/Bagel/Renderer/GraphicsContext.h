#pragma once

namespace Bagel {
	class BG_API GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};
}
