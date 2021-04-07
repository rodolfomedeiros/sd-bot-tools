#include "SDBotTools.h"

int main()
{
	SDConfig* script = SDConfig::getConfig();

	script->init();

	while (script != nullptr) {
		for (HWND window : *SDConfig::hwnds) {
		maximize:
			ShowWindow(window, SW_SHOWNOACTIVATE);
			Sleep(2000);

		running:
			script->run(window);

		minimize:
			cout << "Free for " << script->getSpeed() / 1000 << endl;
			Sleep(script->getSpeed());
			if (SDConfig::hwnds->size() > 1) ShowWindow(window, SW_MINIMIZE);
		}
	}

	return 0;
}