#include "SDBotTools.h"

int main()
{
	SDConfig* script = SDConfig::getConfig();

	if (script == nullptr) {
		cout << "No script selected..." << endl;
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