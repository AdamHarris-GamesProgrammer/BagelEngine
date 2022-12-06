#pragma once

namespace Bagel {
	//Used for controlling what RendererAPI we are currently using. NOTE: Will be expanded in future
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
