#include "SDBotTools.h"

void Bag::upXY(int next) {
	next += 1;
	x += xJump;
	// update: normal -> 41 // coluna 3 (pos 2) -> 40
	if ((next - 2) % 5 == 0) {
		x -= 1;
	}
	if (next % 5 == 0) {
		x = xInit;
		y += yJump;
		// update: 6-10 -> 42 // 10-15 -> 41
		if (next == 10 || next == 25) {
			y -= 1;
		}
		if (next % 15 == 0) {
			y = yInit;
			keyPressF2();
		}
	}
}

void Bag::upXYWithPBag(int pBag) {
	x = xInit;
	y = yInit;
	x += (xJump * (pBag % 5));
	if (tab != Bag::BagTab::F2 && pBag >= 15) {
		tab = Bag::BagTab::F2;
		keyPressF2();
	}
	if (tab == Bag::BagTab::F1) {
		y += (yJump * (pBag / 5));
	}
	else if(tab == Bag::BagTab::F2){
		y += (yJump * (pBag / 20));
	}
}

void Bank::upXY(int next) {
	next += 1;
	x += xJump;
	// update: col 3 (pos 2) -> 42
	if (((next - 2) % 5 == 0)) {
		x += 1;
	}
	if (next % 5 == 0) {
		x = xInit;
		y += yJump;
		// update: row 2, 5 e 7 (pos 1, 4 e 6) -> 41
		if (next == 5 || next == 20 || next == 30) {
			y -= 1;
		}
		if (next % 40 == 0) {
			y = yInit;
			this->upXYTab();
			clickTab(xTab, yTab);
		}
	}
}

void Bag::restart() {
	x = xInit;
	y = yInit;
	tab = Bag::BagTab::F1;
	keyPressF1();
}

void Bank::restart() {
	x = xInit;
	y = yInit;
	xTab = xTabInit;
	yTab = yTabInit;
	clickTab(xTab, yTab);
}

SDConfig* SDConfig::getConfig() {

	int option;

	cout << "" << "\n\n";
	cout << "README:\n";
	cout << "1 - Open SD Window 800x600\n";
	cout << "2 - Run as Admin\n";
	cout << "3 - Open the bag\n";
	cout << "4 - Place the bag in the upper right of the SD window\n\n";

	cout << "SCRIPT SELECTION\n";
	cout << "[1] - Drop" << endl;
	cout << "[2] - Gold Dragon Trade Box" << endl;
	cout << "[3] - Drop with Bank" << endl;
	cout << "[4] - Just Gather Items" << endl;
	cout << "[5] - Get Image from Bag" << endl;
	cout << "Select script: ";
	cin >> option;

	switch (option) {
	case SDDrop:
		return new SDDropConfig();
	case SDDropWithBank:
		return new SDDropWithBankConfig();
	case SDGather:
		return new SDGatherConfig();
	case GetImg:
		return new GetImgConfig();
	default:
		return nullptr;
	}
}

void SDConfig::codesToItems(std::string codes) {
	items = new std::vector<Item*>();
	size_t pos = 0;
	std::string delimiter = ",";
	//Read 000
	readItem("0000", false);
	bool tag;
	while ((pos = codes.find(delimiter)) != string::npos) {
		tag = false;
		if (codes.at(0) == '1') {
			tag = true;
		}
		readItem(codes.substr(1, pos - 1), tag);
		codes.erase(0, pos + delimiter.length());
	}
	tag = false;
	if (codes.at(0) == '1') {
		tag = true;
	}
	readItem(codes.substr(1, string::npos), tag);
}

void SDConfig::readItem(std::string code, bool tag) {
	int count = 48;
	cv::Mat matImg;
	//48=0 to 57=9 ASCII table
	while (count <= 57){
		cout << "put:" << code << "/"<< count << endl;
		matImg = readMat(code);

		if (matImg.data != NULL) {
			items->push_back(new Item(code, tag, matImg));
			items->back()->toString();
		}

		code.at(code.size() - 1) = ++count;
	}
}

void SDConfig::init() {
	int option = 1;

	windowTitle = "Dynasty of Loki";
	std::string strWindow = windowTitle.c_str();
	cout << "Search for " << windowTitle << " window?" << "\n";
	cout << "[1] Yes or [2] Change: ";
	cin >> option;

	if (option == 2) {
		cout << "Insert the window name: ";
		cin >> windowTitle;
	}

	cout << "Change window name: ";
	cin >> strWindow;

	cout << "Unique or Multiple?\n";
	cout << "[1] U or [2] M: ";
	cin >> option;

	if (option == 1) {
		HWND tmpWindow = FindWindowA(NULL, windowTitle.c_str());
		SetWindowTextA(tmpWindow, strWindow.c_str());
		sdWindows->push_back(new SDWindow(tmpWindow));
	}
	else if (option == 2 && EnumWindows(enumWindowsCallback, (LPARAM)this)) {
		for (SDWindow* w : *sdWindows) {
			SetWindowTextA(w->getWindow(), strWindow.c_str());
		}
	}

	windowTitle = strWindow;

	cout << "Windows Count: " << sdWindows->size() << endl << endl;

	cout << "Minimize? (Default " << (bool)minimize << ")\n";
	cout << "[1] True or [2] False: ";
	cin >> option;

	if (option == 2) minimize = false;

	cout << "Minimize Before? (Default " << (bool)minimizeBefore << ")\n";
	cout << "[1] True or [2] False: ";
	cin >> option;

	if (option == 2) minimizeBefore = false;

	option = 2;

	cout << "Insert Codes? (Default 2 - False)\n";
	cout << "[1] True or [2] False: ";
	cin >> option;

	if (option == 1) {
		cout << "Insert the codes (use ,):" << "\n";
		std::string codes;
		cin >> codes;
		codesToItems(codes);
	}

	cout << "Change script speed? (default " << getSpeed() << ")" << "\n";
	cout << "[1] No or [2] Yes: ";
	cin >> option;

	if (option == 2) {
		cout << "Speed: ";
		cin >> speed;
	}

	cout << "Change rate (0.0-100.0)? (default " << getRate() << ")" << "\n";
	cout << "[1] No or [2] Yes: ";
	cin >> option;

	if (option == 2) {
		cout << "Rate: ";
		cin >> rate;
	}

	cout << "\nConfig Initial finished...\n";
	cout << "Loading script selected config...\n";
}

void SDConfig::loop() {
	while (true) {
		for (SDWindow* window : *this->sdWindows) {
		maximize:
			Sleep(500);
			ShowWindow(window->getWindow(), SW_SHOWNORMAL);
			moveToEmpty();
			Sleep(2000);

		running:
			cout << "running next window..." << endl;
			this->run(window);

		minimize:
			cout << "Free for " << this->getSpeed() / 1000 << endl;
			if (this->minimize && this->minimizeBefore) {
				cout << "minimized before..." << endl;
				ShowWindow(window->getWindow(), SW_MINIMIZE);
			}
			for (int time = (this->getSpeed() / 1000); time > 0; time -= 5) {
				if (time < 5) {
					cout << time << " time left..." << endl;
					Sleep(time);
				}
				else {
					cout << time << " time left..." << endl;
					Sleep(5000);
				}
			}
			if (this->minimize && !this->minimizeBefore) {
				cout << "minimized after..." << endl;
				ShowWindow(window->getWindow(), SW_MINIMIZE);
			}
		}
	}
}

void SDDropConfig::start() {
	cout << "Drop script config finished...\n";
}

void SDDropConfig::run(SDWindow * w) {
	bag->setWindow(w->getWindow());
	bank->setWindow(w->getWindow());
	// clear bag
	bag->restart();
	bag->bagToBankItems.clear();
	clearItemBkup();
	captureScreenMat(w->getWindow(), matWindow);
	for (int pBag = 0; pBag < bag->capacity; pBag++) {
		if (pBag % 15 == 0) {
			captureScreenMat(w->getWindow(), matWindow);
			clearItemBkup();
		}
		createMatFromMatSrc(matWindow, matTmp, bag->getX() - 16, bag->getY() - 17, xTamItem, yTamItem);
		match = false;
		for (Item* item : *items) {
			cv::matchTemplate(matTmp, item->getMat(), matResult, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(matResult, 0, &matResultScore);
			if (matResultScore > getRate()) {
				std::cout << "Match with " << item->getCode() << ": " << matResultScore << std::endl;
				match = true;

				if (item->getGather()) {
					if (item->getXYBkup()->existBkup()) {
						bag->suapItem(item->getXYBkup()->getX(), item->getXYBkup()->getY(), bag->getX(), bag->getY(), 800);
					}
					else {
						item->getXYBkup()->setBkup(true);
					}
					item->getXYBkup()->setXY(bag->getX(), bag->getY());
				}else if (!item->getGather() && item->getCode().compare("0000") != 0) {
					bag->bagToBankItems.push_back(pBag);
					//cout << "bag to bank: (" << pBag << ")" << endl;
				}

				break;
			}
		}

		if (!match) {
			std::cout << "To trash: (" << pBag << ") - Score: " << matResultScore  << " removed..." << std::endl;
			bag->itemToTrash(bag->getX(), bag->getY());
		}

		bag->upXY(pBag);
	}
	bag->restart();
}

void SDDropWithBankConfig::start() {
	super::start();

	int option;

	cout << "Change BagToBank (0-25)? (default " << bagToBank << ")" << "\n";
	cout << "[1] No or [2] Yes: ";
	cin >> option;

	if (option == 2) {
		cout << "BagToBank: ";
		cin >> bagToBank;
	}

	cout << "DropWithBank script config finished...\n";
}

void SDDropWithBankConfig::run(SDWindow* w) {
	super::run(w);

	//count items and verify bag capacity
	if (w->isFullBank()) return;
	if (bag->bagToBankItems.size() >= bagToBank) {
		bag->restart();
		bank->restart();
		captureScreenMat(w->getWindow(), matWindow);
		for (int pBank = 0; pBank < bank->capacity; pBank++) {
			if (bag->bagToBankItems.size() == 0) break;
			if (pBank % 40 == 0) captureScreenMat(w->getWindow(), matWindow);
			createMatFromMatSrc(matWindow, matTmp, bank->getX(), bank->getY(), xTamItemBank, yTamItemBank);
			cv::matchTemplate(matTmp, items->at(0)->getMat(), matResult, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(matResult, 0, &matResultScore);
			if (matResultScore > 0.9199) {
				cout << "match: empty bank slot (" << pBank << ")" << endl;
				bag->upXYWithPBag(bag->bagToBankItems.front());
				bag->bagToBankItems.pop_front();
				bag->suapItem(bag->getX()
					, bag->getY()
					, bank->getX()+(xTamItemBank/2)
					, bank->getY()+(yTamItemBank/2)
					, 800);
			}
			else {
				cout << "Bank item found: (" << pBank << ")(" << matResultScore << ")" << endl;
			}

			bank->upXY(pBank);
		}
		if (bag->bagToBankItems.size() > 0) w->setFullBank(true);
	}

	bag->restart();
}

void SDGatherConfig::start(){
	cout << "Gather script config finished...\n";
}

void SDGatherConfig::run(SDWindow* w){
	bag->setWindow(w->getWindow());
	// clear bag
	bag->restart();
	clearItemBkup();
	captureScreenMat(w->getWindow(), matWindow);
	for (int pBag = 0; pBag < bag->capacity; pBag++) {
		if (pBag % 15 == 0) {
			captureScreenMat(w->getWindow(), matWindow);
			clearItemBkup();
		}
		createMatFromMatSrc(matWindow, matTmp, bag->getX() - 16, bag->getY() - 17, xTamItem, yTamItem);
		for (Item* item : *items) {
			cv::matchTemplate(matTmp, item->getMat(), matResult, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(matResult, 0, &matResultScore);
			if (matResultScore > getRate()) {
				std::cout << "Match with " << item->getCode() << ": " << matResultScore << std::endl;

				if (item->getGather()) {
					if (item->getXYBkup()->existBkup()) {
						bag->suapItem(item->getXYBkup()->getX(), item->getXYBkup()->getY(), bag->getX(), bag->getY(), 800);
					}
					else {
						item->getXYBkup()->setBkup(true);
					}
					item->getXYBkup()->setXY(bag->getX(), bag->getY());
				}

				break;
			}
		}

		bag->upXY(pBag);
	}
	bag->restart();
}

void GetImgConfig::start() {
	cout << "Get Img script config finished...\n";
}

void GetImgConfig::run(SDWindow* w) {
	captureScreenMat(w->getWindow(), matWindow);

	createMatFromMatSrc(matWindow, matTmp, 539, 253, 32, 34);

	saveMatToFile(matTmp, ".png", "result");
}