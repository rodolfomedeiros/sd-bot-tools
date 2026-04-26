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

std::unique_ptr<SDConfig> SDConfig::getConfig() {

	int option;

	std::cout << "" << "\n\n";
	std::cout << "README:\n";
	std::cout << "1 - Open SD Window 800x600\n";
	std::cout << "2 - Run as Admin\n";
	std::cout << "3 - Open the bag\n";
	std::cout << "4 - Place the bag in the upper right of the SD window\n\n";

	std::cout << "SCRIPT SELECTION\n";
	std::cout << "[1] - Drop" << std::endl;
	//std::cout << "[2] - Gold Dragon Trade Box" << std::endl;
	std::cout << "[3] - Drop with Bank" << std::endl;
	std::cout << "[4] - Just Gather Items" << std::endl;
	std::cout << "[5] - Get Image from Bag" << std::endl;
	std::cout << "[6] - Ocr Image" << std::endl;
	std::cout << "Select script: ";
	std::cin >> option;

	switch (option) {
	case SDDrop:
		return std::make_unique<SDDropConfig>();
	case SDDropWithBank:
		return std::make_unique<SDDropWithBankConfig>();
	case SDGather:
		return std::make_unique<SDGatherConfig>();
	case GetImg:
		return std::make_unique<GetImgConfig>();
	case OcrImage:
		return std::make_unique<OcrImageConfig>();
	default:
		return nullptr;
	}
}

void SDConfig::codesToItems(std::string codes) {
	items.clear();
	size_t pos = 0;
	std::string delimiter = ",";
	readItem("0000", false);
	bool tag;
	while ((pos = codes.find(delimiter)) != std::string::npos) {
		tag = codes.at(0) == '1';
		readItem(codes.substr(1, pos - 1), tag);
		codes.erase(0, pos + delimiter.length());
	}
	tag = codes.at(0) == '1';
	readItem(codes.substr(1, std::string::npos), tag);
}

void SDConfig::readItem(std::string code, bool tag) {
	int count = 48;
	cv::Mat matImg;
	//48=0 to 57=9 ASCII table
	while (count <= 57) {
		std::cout << "put:" << code << "/" << count << std::endl;
		matImg = readMat(code);

		if (matImg.data != NULL) {
			items.push_back(std::make_unique<Item>(code, tag, matImg));
			items.back()->toString();
		}

		code.at(code.size() - 1) = ++count;
	}
}

void SDConfig::init() {
	windowTitle = "teste";
	std::string strWindow = windowTitle.c_str();
	std::cout << "Search for " << windowTitle << " window" << "\n";
	HWND tmpWindow = FindWindowA(NULL, windowTitle.c_str());
	SetWindowTextA(tmpWindow, strWindow.c_str());
	sdWindows.push_back(std::make_unique<SDWindow>(tmpWindow));

	std::cout << "Windows Count: " << sdWindows.size() << std::endl << std::endl;

	minimize = false;
	minimizeBefore = false;
	speed = 10000;

	std::cout << "\nConfig Initial finished...\n";
	std::cout << "Loading script selected config...\n";
}

void SDConfig::loop() {
	while (true) {
		for (auto& window : this->sdWindows) {
			Sleep(500);
			ShowWindow(window->getWindow(), SW_SHOWNORMAL);
			moveToEmpty();
			Sleep(2000);

			std::cout << "running next window..." << std::endl;
			this->run(window.get());

			std::cout << "Free for " << this->getSpeed() / 1000 << std::endl;
			if (this->minimize && this->minimizeBefore) {
				std::cout << "minimized before..." << std::endl;
				ShowWindow(window->getWindow(), SW_MINIMIZE);
			}
			for (int time = (this->getSpeed() / 1000); time > 0; time -= 5) {
				if (time < 5) {
					std::cout << time << " time left..." << std::endl;
					Sleep(time);
				}
				else {
					std::cout << time << " time left..." << std::endl;
					Sleep(5000);
				}
			}
			if (this->minimize && !this->minimizeBefore) {
				std::cout << "minimized after..." << std::endl;
				ShowWindow(window->getWindow(), SW_MINIMIZE);
			}
		}
	}
}

void SDConfig::scanBag(Bag* bag, HWND hwnd, bool dropUnmatched) {
	bag->restart();
	clearItemBkup();
	captureScreenMat(hwnd, matWindow);
	for (int pBag = 0; pBag < bag->getCapacity(); pBag++) {
		if (pBag % 15 == 0) {
			captureScreenMat(hwnd, matWindow);
			clearItemBkup();
		}
		createMatFromMatSrc(matWindow, matTmp, bag->getX() - 16, bag->getY() - 17, xTamItem, yTamItem);
		bool matched = false;
		for (auto& item : items) {
			cv::matchTemplate(matTmp, item->getMat(), matResult, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(matResult, 0, &matResultScore);
			if (matResultScore > getRate()) {
				std::cout << "Match with " << item->getCode() << ": " << matResultScore << std::endl;
				matched = true;

				if (item->getGather()) {
					if (item->getXYBkup()->existBkup()) {
						bag->suapItem(item->getXYBkup()->getX(), item->getXYBkup()->getY(), bag->getX(), bag->getY(), 800);
					}
					else {
						item->getXYBkup()->setBkup(true);
					}
					item->getXYBkup()->setXY(bag->getX(), bag->getY());
				}
				else if (dropUnmatched && item->getCode().compare("0000") != 0) {
					bag->bagToBankItems.push_back(pBag);
				}

				break;
			}
		}

		if (!matched && dropUnmatched) {
			std::cout << "To trash: (" << pBag << ") - Score: " << matResultScore << " removed..." << std::endl;
			bag->itemToTrash(bag->getX(), bag->getY());
		}

		bag->upXY(pBag);
	}
	bag->restart();
}

void SDDropConfig::start() {
	std::cout << "Drop script config finished...\n";
}

void SDDropConfig::run(SDWindow* w) {
	bag->setWindow(w->getWindow());
	bank->setWindow(w->getWindow());
	bag->bagToBankItems.clear();
	scanBag(bag.get(), w->getWindow(), true);
}

void SDDropWithBankConfig::start() {
	super::start();

	int option;

	std::cout << "Change BagToBank (0-25)? (default " << bagToBank << ")" << "\n";
	std::cout << "[1] No or [2] Yes: ";
	std::cin >> option;

	if (option == 2) {
		std::cout << "BagToBank: ";
		std::cin >> bagToBank;
	}

	std::cout << "DropWithBank script config finished...\n";
}

void SDDropWithBankConfig::run(SDWindow* w) {
	super::run(w);

	if (w->isFullBank()) return;
	if (bag->bagToBankItems.size() >= static_cast<size_t>(bagToBank)) {
		bag->restart();
		bank->restart();
		captureScreenMat(w->getWindow(), matWindow);
		for (int pBank = 0; pBank < bank->getCapacity(); pBank++) {
			if (bag->bagToBankItems.empty()) break;
			if (pBank % 40 == 0) captureScreenMat(w->getWindow(), matWindow);
			createMatFromMatSrc(matWindow, matTmp, bank->getX(), bank->getY(), xTamItemBank, yTamItemBank);
			cv::matchTemplate(matTmp, items.at(0)->getMat(), matResult, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(matResult, 0, &matResultScore);
			if (matResultScore > 0.9199) {
				std::cout << "match: empty bank slot (" << pBank << ")" << std::endl;
				bag->upXYWithPBag(bag->bagToBankItems.front());
				bag->bagToBankItems.pop_front();
				bag->suapItem(bag->getX()
					, bag->getY()
					, bank->getX() + (xTamItemBank / 2)
					, bank->getY() + (yTamItemBank / 2)
					, 800);
			}
			else {
				std::cout << "Bank item found: (" << pBank << ")(" << matResultScore << ")" << std::endl;
			}

			bank->upXY(pBank);
		}
		if (!bag->bagToBankItems.empty()) w->setFullBank(true);
	}

	bag->restart();
}

void SDGatherConfig::start(){
	std::cout << "Gather script config finished...\n";
}

void SDGatherConfig::run(SDWindow* w){
	bag->setWindow(w->getWindow());
	scanBag(bag.get(), w->getWindow(), false);
}

void GetImgConfig::start() {
	std::cout << "Get Img script config finished...\n";
}

void GetImgConfig::run(SDWindow* w) {
	captureScreenMat(w->getWindow(), matWindow);
	createMatFromMatSrc(matWindow, matTmp, 539, 253, 32, 34);
	saveMatToFile(matTmp, ".png", "result");
}

void OcrImageConfig::start() {
	std::cout << "OcrImageConfig started...\n";
}

void OcrImageConfig::run(SDWindow* w) {
	std::cout << "OcrImageConfig running...\n";
}