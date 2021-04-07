#pragma once
#include <string>
#include "OpenCVTools.h"

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
	Item() {}
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
};

class Cargo {
protected:
	int xJump;
	int yJump;
	int xAba;
	int yAba;
	int xInit;
	int yInit;
	int x;
	int y;
	std::vector<Item>* items;

public:
	Cargo(int xInit, int yInit, int xJump, int yJump, int xAba, int yAba) {
		this->xInit = xInit;
		this->yInit = yInit;
		this->xJump = xJump;
		this->yJump = yJump;
		this->xAba = xAba;
		this->yAba = yAba;
		this->x = this->xInit;
		this->y = this->yInit;
	}
	~Cargo() {};
	int getX() { return x; };
	int getY() { return y; };
	bool restartXY() {
		x = xInit;
		y = yInit;
		return true;
	}
	bool upXY() {
		x = getX() + xJump;
		y = getY() + yJump;
		return true;
	}
	void setItems(std::vector<Item>* items) {
		this->items = items;
	}
	std::vector<Item>* getItems() {
		return items;
	}
	void clear();
	void gather();
};