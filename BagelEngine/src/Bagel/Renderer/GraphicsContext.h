#pragma once

namespace Bagel {
	//Abstract class representing a graphics context for each type of graphics API
	class BG_API GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}
