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

	while (script != nullptr) {
		for (SDWindow * window : *script->sdWindows) {
		maximize:
			Sleep(500);
			ShowWindow(window->getWindow(), SW_SHOWNOACTIVATE);
			Sleep(2000);

		running:
			cout << "running next window..." << endl;
			script->run(window);

		minimize:
			cout << "Free for " << script->getSpeed() / 1000 << endl;
			Sleep(script->getSpeed());
			if (script->sdWindows->size() > 1) {
				cout << "minimized..." << endl;
				ShowWindow(window->getWindow(), SW_MINIMIZE);
			}
		}
	}

	return 0;
}