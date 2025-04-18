#pragma once
#include "WinApiWithRealMove.h"
#include "OpenCVAPI.h"
#include <vector>
#include <string>

using namespace std;

class SDWindow {
protected:
	HWND hwnd;
	bool fullBank;
public:
	SDWindow(HWND hwnd) {
		this->hwnd = hwnd;
		this->fullBank = false;
	}
	~SDWindow() {}
	HWND getWindow() {
		return hwnd;
	}
	void setWindow(HWND hwnd) {
		this->hwnd = hwnd;
	}
	void setFullBank(bool fullBank) {
		this->fullBank = fullBank;
	}
	bool isFullBank() {
		return fullBank;
	}
};

class Pos {
private:
	bool bkup;
	int x, y;
public:
	Pos(bool bkup, int x, int y) {
		this->bkup = bkup;
		this->x = x;
		this->y = y;
	}
	~Pos() {}

	int getX() { return x; }
	int getY() { return y; }
	bool existBkup() { return bkup; }
	void setBkup(bool bkup) {
		this->bkup = bkup;
	}
	void setX(int x) {
		this->x = x;
	}
	void setY(int y) {
		this->y = y;
	}
	void setXY(int x, int y) {
		this->setX(x);
		this->setY(y);
	}
};

class Item {
private:
	std::string code;
	bool gather = false;
	cv::Mat mat;
	Pos* xyBkup;
public:
	Item() {}
	Item(std::string code, bool gather, cv::Mat mat) {
		this->code = code;
		this->gather = gather;
		this->mat = mat;
		this->xyBkup = new Pos(false, 0, 0);
	}
	~Item() {}

	std::string getCode() {
		return code;
	}
	bool getGather() {
		return gather;
	}
	cv::Mat getMat() {
		return mat;
	}
	Pos* getXYBkup() {
		return xyBkup;
	}
	void setCode(std::string code) {
		this->code = code;
	}
	void setMat(cv::Mat mat) {
		this->mat = mat;
	}
	void setGather(bool gather) {
		this->gather = gather;
	}
	void toString() {
		cout << "Item: " << code << " with TAG: " << (int)gather << " Mat size: " << mat.size << " INSERTED!" << endl;
	}
};

class Cargo : public WinAPI {
public:
	int capacity;
	int xJump;
	int yJump;
	int xInit;
	int yInit;
	int x;
	int y;
	bool full;
	Cargo(int capacity, int xInit, int yInit, int xJump, int yJump) {
		this->xInit = xInit;
		this->yInit = yInit;
		this->xJump = xJump;
		this->yJump = yJump;
		this->capacity = capacity;
		this->full = false;
	}
	~Cargo() {};
	int getX() { return x; };
	int getY() { return y; };
	virtual void restart() {};
	virtual void upXY(int next) {};
};

class Bank : public Cargo {
public:
	int xTabJump;
	int xTabInit;
	int yTabInit;
	int xTab;
	int yTab;
	/*
		@param yTabinit
		@param xTabInit
		@param xTabJump
		@param capacity
		@param xInit
		@param yInit
		@param xJump
		@param yJump
	*/
	Bank(int capacity, int xInit, int yInit, int xJump, int yJump)
		: Cargo(capacity, xInit, yInit, xJump, yJump) {
		this->xTabJump = 26;
		this->xTabInit = 442;
		this->yTabInit = 446;
	}
	~Bank() {}
	void restart();
	void upXYTab() {
		xTab += xTabJump;
	}
	void upXY(int next);
};

class Bag : public Cargo {
public:

	enum BagTab { F1 = 1, F2 = 2 };

	std::deque<int> bagToBankItems;
	BagTab tab;

	/*
		@param capacity
		@param xInit
		@param yInit
		@param xJump
		@param yJump
	*/
	Bag(int capacity, int xInit, int yInit, int xJump, int yJump)
		: Cargo(capacity, xInit, yInit, xJump, yJump) {
	}
	~Bag() {};
	void upXY(int next);
	void upXYWithPBag(int pBag);
	void restart();
};

class SDConfig : public OpenCVAPI, public WinAPI {
protected:
	enum Script { SDDrop = 1, SDGoldDragonTradeBox = 2, SDDropWithBank = 3, SDGather = 4, GetImg = 5};

	//proprerties
	int speed;
	double rate;
	std::vector<Item*>* items;

public:
	SDConfig() {
		this->xTamItem = 32;
		this->yTamItem = 34;
		this->xTamItemBank = 35;
		this->yTamItemBank = 37;
		sdWindows = new vector<SDWindow*>();
	}
	~SDConfig() {}
	bool minimize = true;
	bool minimizeBefore = true;
	std::string windowTitle;
	int xTamItem, yTamItem, xTamItemBank, yTamItemBank;
	std::vector<SDWindow*>* sdWindows;
	static SDConfig* getConfig();
	void init();
	virtual void start() { cout << "start method from SDConfig called..." << endl; };
	virtual void run(SDWindow* sdWindow) { cout << "run method from SDConfig called..." << endl; };
	void loop();
	void codesToItems(std::string codes);
	void readItem(std::string code, bool tag);
	int getSpeed() {
		return speed;
	}
	void setSpeed(int speed) {
		this->speed = speed;
	}
	double getRate() {
		return rate;
	}
	void setRate(double rate) {
		this->rate = rate;
	}
	void clearItemBkup() {
		for (Item* item : *items) item->getXYBkup()->setBkup(false);
	}
	static BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lparam) {
		SDConfig* config = (SDConfig*)lparam;

		const int tamTitle = strlen(config->windowTitle.c_str()) + 1;
		LPSTR windowTitle = new char[tamTitle];
		GetWindowTextA(hwnd, windowTitle, tamTitle);

		if (strcmp(windowTitle, config->windowTitle.c_str()) == 0) {
			config->sdWindows->push_back(new SDWindow(hwnd));
		}

		return true;
	}
};

class SDDropConfig : public SDConfig {
public:
	SDDropConfig() {
		/*
			xInit --> XXX with square stating on 539 + 16px = 539 px
			yInit --> square starting on 253 + 17px = 270px NOTE: 296-26 px tab = 270px
			square size => 32x34px
		*/
		bag = new Bag(25, 555, 270, 41, 42);
		/*
			xInit --> 311 with square stating on 313 - 3px position window = 310px
			yInit --> 117 - 26px(window bar) = 91px
		*/
		bank = new Bank(120, 310, 91, 41, 42);
		speed = 600;
		rate = 0.7199;
		bagToBank = 15;

		matTmp = cv::Mat();
		matWindow = cv::Mat();
		matResult = cv::Mat();
		match = false;
		matResultScore = 0.0000;
	}
	~SDDropConfig() {}
	Bag* bag;
	Bank* bank;
	int bagToBank;
	cv::Mat matWindow, matTmp, matResult;
	double matResultScore;
	bool match;
	void start() override;
	void run(SDWindow* sdWindow) override;
};

class SDDropWithBankConfig : public SDDropConfig {
public:
	typedef SDDropConfig super;
	SDDropWithBankConfig(){}
	~SDDropWithBankConfig(){}
	void start() override;
	void run(SDWindow* w) override;
};

class SDGatherConfig : public SDConfig {
public:
	SDGatherConfig() {
		bag = new Bag(25, 555, 270, 41, 42);
		speed = 600;
		rate = 0.7199;

		matTmp = cv::Mat();
		matWindow = cv::Mat();
		matResult = cv::Mat();
		matResultScore = 0.0000;
	}
	~SDGatherConfig() {}
	Bag* bag;
	cv::Mat matWindow, matTmp, matResult;
	double matResultScore;
	void start() override;
	void run(SDWindow* sdWindow) override;
};

class GetImgConfig : public SDConfig {
public:
	GetImgConfig() {
		matTmp = cv::Mat();
		matWindow = cv::Mat();
		matResult = cv::Mat();
	}
	~GetImgConfig() {}
	cv::Mat matWindow, matTmp, matResult;
	void start() override;
	void run(SDWindow* sdWindow) override;
};