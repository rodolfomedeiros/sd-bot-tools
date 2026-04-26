#include "SDBotTools.h"

int main()
{
	std::unique_ptr<SDConfig> script = SDConfig::getConfig();

	if (script == nullptr) {
		std::cout << "No script selected..." << std::endl;
		return 0;
	}
	//init config general for all
	script->init();

	//starting config script
	script->start();

	//windows loop
	script->loop();

	return 0;
}