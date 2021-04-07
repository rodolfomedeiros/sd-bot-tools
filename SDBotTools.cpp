#include "SDBotTools.h"

bool Bag::upXY(int next) {

	if (next % 5 == 0) {
		x = xInit;
		y = +yJump;
		if (next % 15 == 0) {
			y = yInit;
			keyPressF2();
		}
	}
	return true;
}

bool Bank::upXY(int next) {
	if (next % 5 == 0) {
		x = xInit;
		y = +yJump;
		if (next % 40 == 0) {
			y = yInit;
			upXYTab();
			clickTab(xTab, yTab);
		}
	}
	return true;
}

bool Bag::restart() {
	keyPressF1();
	x = xInit;
	y = yInit;
	return true;
}

bool Bank::restart() {
	x = xInit;
	y = yInit;
	xTab = xTabInit;
	yTab = yTabInit;
	clickTab(xTab, yTab);
	return true;
}

std::vector<HWND>* SDConfig::hwnds;

SDConfig* SDConfig::getConfig() {

	int option;
	hwnds = new std::vector<HWND>();
	SDConfig* config = nullptr;

	cout << "" << "\n\n";
	cout << "README:\n";
	cout << "1 - Open SD Window 800x600\n";
	cout << "2 - Run as Admin\n";
	cout << "3 - Open the bag\n";
	cout << "4 - Place the bag in the upper right of the SD window\n\n";

	cout << "SCRIPT SELECTION\n";
	cout << "[1] - Drop" << endl;
	cout << "[2] - Gold Dragon Trade Box";
	cout << "Select script: ";
	cin >> option;

	switch (option) {
		//drop
	case SDDrop:
		config = new SDDropConfig();
		break;
		//trade box
	case SDGoldDragonTradeBox:
		break;
	default:
		break;
	}

	string strWindowInit = "Supreme Destiny";
	string strWindowFinal = strWindowInit.c_str();
	cout << "Search for " << strWindowInit << " window?" << "\n";
	cout << "[1] Yes or [2] Change: ";
	cin >> option;

	if (option == '2') {
		cout << "Insert the window name: ";
		cin >> strWindowInit;
	}

	cout << "Change window name: ";
	cin >> strWindowFinal;

	cout << "Unique or Multiple?\n";
	cout << "[1] U or [2] M: ";
	cin >> option;

	if (option == '1') {
		HWND tmpWindow = FindWindowA(NULL, strWindowInit.c_str());
		SetWindowTextA(tmpWindow, strWindowFinal.c_str());
		hwnds->push_back(tmpWindow);
	}
	else if (option == '2' && EnumWindows(verifyWindowTitle, (LPARAM)strWindowInit.c_str())) {
		for (HWND w : *hwnds) {
			SetWindowTextA(w, strWindowFinal.c_str());
		}
	}

	cout << "Supreme Destiny Windows Count: " << hwnds->size() << endl << endl;

	cout << "Insert the codes (use ,):" << "\n";
	std::string codes;
	cin >> codes;
	config->codesToItems(codes);

	cout << "Change script speed? (default " << config->getSpeed() << ")" << "\n";
	cout << "[1] No or [2] Yes: ";
	cin >> option;

	if (option == '2') {
		cout << "Speed: ";
		int speed;
		cin >> speed;
		config->setSpeed(speed);
	}

	cout << "\n\nConfig Initial finished...\n";
	cout << "Loading script selected config...\n";

	return config;
}

void SDConfig::codesToItems(std::string codes) {
	items = new std::vector<Item*>();
	size_t pos = 0;
	std::string delimiter = ",";
	items->push_back(new Item("000", false, OpenCVAPI::readMat("000")));
	items->back()->toString();
	bool tag;
	while ((pos = codes.find(delimiter)) != string::npos) {
		tag = false;
		if (codes.at(0) == '1') {
			tag = true;
		}
		items->push_back(new Item(codes.substr(1, pos - 1), tag, OpenCVAPI::readMat(codes.substr(1, pos - 1))));
		items->back()->toString();
		codes.erase(0, pos + delimiter.length());
	}
	tag = false;
	if (codes.at(0) == '1') {
		tag = true;
	}
	items->push_back(new Item(codes.substr(1, string::npos), tag, OpenCVAPI::readMat(codes.substr(1, string::npos))));
	items->back()->toString();
}

void SDDropConfig::init() {

}

void SDDropConfig::run(HWND window) {
	bag->restart();
	bank->restart();
	OpenCVAPI::captureScreenMat(window, matWindow);
	for (int pBag = 1; pBag <= bag->capacity; pBag++) {
		OpenCVAPI::createMatFromMatSrc(matWindow, matTmp, bag->getX()-13, bag->getY()-15, xTamItem, yTamItem);
		for (int i = 0; i < items->size(); i++) {
			cv::matchTemplate(matTmp, items->at(i)->getMat(), matResult, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(matResult, 0, &matResultScore);
			if (matResultScore > getRate()) {
				cout << "match with " << items->at(i)->getCode() << ": " << matResultScore << endl;
				cout << "found item: (" << pBag << ")" << endl;
				break;
			}
			else {

			}
		}

		if (!match) {
			
		}

		if (match && sdConfig.toExclude && (matchItem.compare("000") != 0)) {
			deleteItem(hwnd, xPos, yPos);
			std::cout << "item removed.." << std::endl;
		}
		else if (!match && !sdConfig.toExclude) {
			deleteItem(hwnd, xPos, yPos);
			std::cout << "not match: " << maxScore << std::endl;
		}
	}
}

