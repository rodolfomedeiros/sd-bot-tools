#pragma once
#include "Windows.h"

class SDWindow {
protected:
	HWND window;
public:

	void setWindow(HWND w) {
		window = w;
	}

	HWND getWindow() {
		return window;
	}

	void keyPressF1() {
		SendMessage(window, WM_KEYDOWN, VK_F1, 0);
		Sleep(10);
		SendMessage(window, WM_KEYUP, VK_F1, 0);
		Sleep(100);
	}

	void keyPressF2() {
		SendMessage(window, WM_KEYDOWN, VK_F2, 0);
		Sleep(10);
		SendMessage(window, WM_KEYUP, VK_F2, 0);
		Sleep(100);
	}

	void sendClick(UINT msg, int lparam, int sleep) {
		SendMessage(window, msg, 0, lparam);
		Sleep(sleep);
	}

	void itemToTrash(int x, int y) {
		// click item
		SendMessage(window, WM_LBUTTONDOWN, 0, (y << 16) + x);
		Sleep(50);
		SendMessage(window, WM_LBUTTONUP, 0, (y << 16) + x);
		Sleep(50);
		// click trash
		SendMessage(window, WM_LBUTTONDOWN, 0, (362 << 16) + 769);
		Sleep(50);
		SendMessage(window, WM_LBUTTONUP, 0, (362 << 16) + 769);
		Sleep(50);
		// send enter key
		//PostMessage(window, WM_KEYDOWN, VK_RETURN, 1);
		SendMessage(window, WM_KEYDOWN, VK_RETURN, 1);
		Sleep(100);
	}

	void clickTab(int xTab, int yTab) {
		SendMessage(window, WM_LBUTTONDBLCLK, 0, (yTab << 16) + xTab);
		Sleep(100);
	}
};
