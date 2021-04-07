#pragma once
#include "WinAPI.h"
#include "OpenCVAPI.h"
#include <vector>
#include <string>

using namespace std;

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
	void setBkup(bool bkyp) {
		this->bkup = bkup;
	}
	void setX(int x) {
		this->x = x;
	}
	void setY(int y) {
		this->y = y;
	}
};

class Item {
private:
	std::string code;
	bool gather = false;
	cv::Mat mat;
	Pos xyBkup = Pos(false, 0, 0);
public:
	Item(){}
	Item(std::string code, bool gather, cv::Mat mat) {
		this->code = code;
		this->gather = gather;
		this->mat = mat;
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
	Pos getXYBkup() {
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

class Cargo : public SDWindow{
public:
	int capacity;
	int xJump;
	int yJump;
	int xInit;
	int yInit;
	int x;
	int y;
	Cargo(int capacity, int xInit, int yInit, int xJump, int yJump) {
		this->xInit = xInit;
		this->yInit = yInit;
		this->xJump = xJump;
		this->yJump = yJump;
		this->capacity = capacity;
	}
	~Cargo() {};
	int getX() { return x; };
	int getY() { return y; };
	virtual bool restart() {};
	virtual bool upXY(int next) {};
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
	Bank(int yTabInit, int xTabInit, int xTabJump, int capacity, int xInit, int yInit, int xJump, int yJump)
		: Cargo(capacity, xInit, yInit, xJump, yJump) {
		this->xTabJump = xTabJump;
		this->xTabInit = xTabInit;
		this->yTabInit = yTabInit;
	}
	~Bank(){}
	bool restart();
	bool upXYTab() {
		xTab =+ xTabJump;
		return true;
	}
	/*
		@param next = pos + 1
	*/
	bool upXY(int next);
};

class Bag : public Cargo {
public:
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
	/*
		@param next = pos + 1
	*/
	bool upXY(int next);
	bool restart();
};

class SDConfig {
protected:
	enum Script {SDDrop = 1, SDGoldDragonTradeBox = 2};
	static BOOL CALLBACK verifyWindowTitle(HWND hwnd, LPARAM lparam) {
		const int tamTitle = strlen((char*)lparam) + 1;
		LPSTR windowTitle = new char[tamTitle];
		GetWindowTextA(hwnd, windowTitle, tamTitle);

		if (strcmp(windowTitle, (char*)lparam) == 0) {
			hwnds->push_back(hwnd);
		}

		return true;
	}

	//proprerties
	int speed;
	std::vector<Item*>* items;

public:
	SDConfig() {
		this->xTamItem = 37;
		this->yTamItem = 38;
		matTmp = cv::Mat();
		matWindow = cv::Mat();
		matResult = cv::Mat();
		matchItem 
	}
	~SDConfig(){}
	int xTamItem, yTamItem;
	cv::Mat matWindow, matTmp, matResult;
	double matResultScore;
	bool matchItem;
	static std::vector<HWND>* hwnds;
	static SDConfig* getConfig();
	virtual void init() { cout << "init method from SDConfig called..." << endl; };
	virtual void run(HWND window) { cout << "run method from SDConfig called..." << endl; };
	void codesToItems(std::string codes);
	int getSpeed() {
		return speed;
	}
	void setSpeed(int speed) {
		this->speed = speed;
	}
};

class SDDropConfig : public SDConfig {
private:
	double rate;

	Bag* bag;
	Bank* bank;
public:
	SDDropConfig() {
		this->bag = new Bag(25, 599, 299, 41, 42);
		this->speed = 600;
		this->rate = 0.7199;
	}
	~SDDropConfig(){}
	void init() override;
	void run(HWND window) override;
	double getRate() {
		return rate;
	}
	void setRate(double rate) {
		this->rate = rate;
	}
};