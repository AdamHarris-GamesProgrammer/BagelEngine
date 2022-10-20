#include <Bagel.h>

class SandboxApplication : public Bagel::BagelApplication {
public:
	SandboxApplication() {

	}

	~SandboxApplication() {

	}

};

Bagel::BagelApplication* Bagel::CreateApplication() {
	return new SandboxApplication();
}