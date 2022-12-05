#pragma once

namespace Bagel {

	enum class RendererAPI {
		None = 0,
		OpenGL = 1,
		Direct3D = 2
	};

	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return _sRendererAPI; }

	private:
		static RendererAPI _sRendererAPI;
	};

}
