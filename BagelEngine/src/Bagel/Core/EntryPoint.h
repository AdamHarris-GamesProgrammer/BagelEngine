#pragma once

#ifdef BG_PLATFORM_WINDOWS

extern Bagel::BagelApplication* Bagel::CreateApplication();

int main(int argc, char** argv) 
{
	Bagel::Log::Init();

	BG_PROFILE_BEGIN_SESSION("Startup", "BagelProfile-Startup.json");
	auto app = Bagel::CreateApplication();
	BG_PROFILE_END_SESSION();

	BG_PROFILE_BEGIN_SESSION("Runtime", "BagelProfile-Runtime.json");
	app->Run();
	BG_PROFILE_END_SESSION();

	BG_PROFILE_BEGIN_SESSION("Shutdown", "BagelProfile-Shutdown.json");
	delete app;
	BG_PROFILE_END_SESSION();
}
#else

#endif
