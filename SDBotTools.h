#pragma once
#include "WinApiWithRealMove.h"
#include "OpenCVAPI.h"
#include <vector>
#include <string>
#include <memory>
#include <deque>

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
	std::unique_ptr<Pos> xyBkup;
public:
	Item() {}
	Item(std::string code, bool gather, cv::Mat mat) {
		this->code = code;
		this->gather = gather;
		this->mat = mat;
		this->xyBkup = std::make_unique<Pos>(false, 0, 0);
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
		return xyBkup.get();
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
		std::cout << "Item: " << code << " with TAG: " << (int)gather << " Mat size: " << mat.size << " INSERTED!" << std::endl;
	}
};

class Cargo : public WinAPI {
protected:
	int capacity;
	int xJump;
	int yJump;
	int xInit;
	int yInit;
	int x;
	int y;
	bool full;
public:
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
	int getCapacity() { return capacity; };
	virtual void restart() {};
	virtual void upXY(int next) {};
};

class Bank : public Cargo {
protected:
	int xTabJump;
	int xTabInit;
	int yTabInit;
	int xTab;
	int yTab;
public:
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
	enum Script { SDDrop = 1, SDGoldDragonTradeBox = 2, SDDropWithBank = 3, SDGather = 4, GetImg = 5, OcrImage = 6};

	int speed;
	double rate;
	std::vector<std::unique_ptr<Item>> items;
	cv::Mat matWindow, matTmp, matResult;
	double matResultScore = 0.0;

	void scanBag(Bag* bag, HWND hwnd, bool dropUnmatched);

public:
	SDConfig() {
		this->xTamItem = 32;
		this->yTamItem = 34;
		this->xTamItemBank = 35;
		this->yTamItemBank = 37;
	}
	~SDConfig() {}
	bool minimize = true;
	bool minimizeBefore = true;
	std::string windowTitle;
	int xTamItem, yTamItem, xTamItemBank, yTamItemBank;
	std::vector<std::unique_ptr<SDWindow>> sdWindows;
	static std::unique_ptr<SDConfig> getConfig();
	void init();
	virtual void start() { std::cout << "start method from SDConfig called..." << std::endl; };
	virtual void run(SDWindow* sdWindow) { std::cout << "run method from SDConfig called..." << std::endl; };
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
		for (auto& item : items) item->getXYBkup()->setBkup(false);
	}
	static BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lparam) {
		SDConfig* config = (SDConfig*)lparam;

		std::string title(config->windowTitle.size() + 1, '\0');
		GetWindowTextA(hwnd, &title[0], static_cast<int>(title.size()));
		title.resize(strlen(title.c_str()));

		if (title == config->windowTitle) {
			config->sdWindows.push_back(std::make_unique<SDWindow>(hwnd));
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
		bag = std::make_unique<Bag>(25, 555, 270, 41, 42);
		/*
			xInit --> 311 with square stating on 313 - 3px position window = 310px
			yInit --> 117 - 26px(window bar) = 91px
		*/
		bank = std::make_unique<Bank>(120, 310, 91, 41, 42);
		speed = 10000;
		rate = 0.6199;
		bagToBank = 15;
	}
	~SDDropConfig() {}
	std::unique_ptr<Bag> bag;
	std::unique_ptr<Bank> bank;
	int bagToBank;
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
		bag = std::make_unique<Bag>(25, 555, 270, 41, 42);
		speed = 600;
		rate = 0.6199;
	}
	~SDGatherConfig() {}
	std::unique_ptr<Bag> bag;
	void start() override;
	void run(SDWindow* sdWindow) override;
};

class GetImgConfig : public SDConfig {
public:
	GetImgConfig() {}
	~GetImgConfig() {}
	void start() override;
	void run(SDWindow* sdWindow) override;
};

class OcrImageConfig : public SDConfig {
public:
	OcrImageConfig() {}
	~OcrImageConfig() {}
	void start() override;
	void run(SDWindow* sdWindow) override;
};