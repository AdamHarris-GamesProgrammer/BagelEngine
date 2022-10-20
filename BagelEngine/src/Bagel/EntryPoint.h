#pragma once

#ifdef BG_PLATFORM_WINDOWS

extern Bagel::BagelApplication* Bagel::CreateApplication();

int main(int argc, char** argv) {
	//printf("Boop");
	auto app = Bagel::CreateApplication();

	app->Run();
	delete app;

}
#else

#endif
