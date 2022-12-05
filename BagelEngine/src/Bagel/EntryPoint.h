#pragma once

#ifdef BG_PLATFORM_WINDOWS

extern Bagel::BagelApplication* Bagel::CreateApplication();

int main(int argc, char** argv) 
{
	Bagel::Log::Init();

	BG_CORE_WARNING("Initalized Log!");
	BG_WARNING("Initialized App");

	auto app = Bagel::CreateApplication();

	app->Run();
	delete app;
}
#else

#endif
