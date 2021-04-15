#pragma once
#include "Windows.h"
#include <iostream>

class WinAPI {
public:
	HWND window;
	void setWindow(HWND w) {
		this->window = w;
	}

	HWND getWindow() {
		return window;
	}

	void keyPressF1() {
		SendMessage(window, WM_KEYDOWN, VK_F1, 0);
		Sleep(10);
		SendMessage(window, WM_KEYUP, VK_F1, 0);
		Sleep(300);
	}

	void keyPressF2() {
		SendMessage(window, WM_KEYDOWN, VK_F2, 0);
		Sleep(10);
		SendMessage(window, WM_KEYUP, VK_F2, 0);
		Sleep(300);
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
		PostMessage(window, WM_KEYDOWN, VK_RETURN, 1);
		Sleep(150);
	}

	INT GetAbsoluteCoordinate(INT PixelCoordinate, INT ScreenResolution)
	{
		INT AbsoluteCoordinate = MulDiv(PixelCoordinate, 65535, ScreenResolution);
		return AbsoluteCoordinate;
	}

	void clickTab(int xTab, int yTab) {
		INPUT input = {0};

		POINT p = { 0,0 };
		ClientToScreen(window, &p);
		for (int i = 0; i < 2; i++) {
			input.type = INPUT_MOUSE;
			input.mi.dx = GetAbsoluteCoordinate((p.x - 1) + xTab, GetSystemMetrics(SM_CXVIRTUALSCREEN)); // desired X coordinate
			input.mi.dy = GetAbsoluteCoordinate((p.y - 1) + yTab, GetSystemMetrics(SM_CYVIRTUALSCREEN)); ; // desired Y coordinate
			input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
			SendInput(1, &input, sizeof(INPUT));
			input = { 0 };
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &input, sizeof(INPUT));
			Sleep(50);
			input = { 0 };
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &input, sizeof(INPUT));
			Sleep(50);
		}
		Sleep(300);
	}

	void suapItem(int xBkup, int yBkup, int posX, int posY, int delay) {
		sendClick(WM_LBUTTONDOWN, (yBkup << 16) + xBkup, 10);
		sendClick(WM_LBUTTONUP, (yBkup << 16) + xBkup, 100);
		//
		sendClick(WM_LBUTTONDOWN, (posY << 16) + posX, 10);
		sendClick(WM_LBUTTONUP, (posY << 16) + posX, delay);
	}

	COLORREF getPixel(int x, int y) {
		return GetPixel(GetWindowDC(window), x, y);
	}
};
