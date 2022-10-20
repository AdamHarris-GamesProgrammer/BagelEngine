#pragma once
#include "Core.h"

namespace Bagel {
	class BG_API BagelApplication
	{
	public:
		BagelApplication();
		virtual ~BagelApplication();

		void Run();


	};

	//Defined in client app
	BagelApplication* CreateApplication();
}



