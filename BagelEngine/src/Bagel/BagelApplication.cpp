#include "BagelApplication.h"

#include "Bagel/Events/ApplicationEvent.h"
#include "Bagel/Log.h"

namespace Bagel {
	BagelApplication::BagelApplication()
	{
	}

	BagelApplication::~BagelApplication()
	{
	}

	void BagelApplication::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			BG_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput)) {
			BG_TRACE(e);
		}

		while (true);
	}

}
