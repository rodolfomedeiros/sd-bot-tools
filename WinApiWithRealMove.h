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
		moveToEmpty();
		Sleep(300);
	}

	void keyPressF2() {
		SendMessage(window, WM_KEYDOWN, VK_F2, 0);
		Sleep(10);
		SendMessage(window, WM_KEYUP, VK_F2, 0);
		moveToEmpty();
		Sleep(300);
	}

	void itemToTrash(int x, int y) {
		POINT p = { 0,0 };
		ClientToScreen(window, &p);
		
		INPUT input = {0};

		//ITEM
		x = GetAbsoluteCoordinate((p.x - 1) + x, GetSystemMetrics(SM_CXVIRTUALSCREEN));
		y = GetAbsoluteCoordinate((p.y - 1) + y, GetSystemMetrics(SM_CYVIRTUALSCREEN));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(50);
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(50);

		//TRASH
		x = GetAbsoluteCoordinate((p.x - 1) + 720, GetSystemMetrics(SM_CXVIRTUALSCREEN));
		y = GetAbsoluteCoordinate((p.y - 1) + 405, GetSystemMetrics(SM_CYVIRTUALSCREEN));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(50);
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(50);
		
		//ENTER
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
		POINT p = { 0,0 };
		ClientToScreen(window, &p);

		INPUT input = { 0 };

		// backup
		xBkup = GetAbsoluteCoordinate((p.x - 1) + xBkup, GetSystemMetrics(SM_CXVIRTUALSCREEN));
		yBkup = GetAbsoluteCoordinate((p.y - 1) + yBkup, GetSystemMetrics(SM_CYVIRTUALSCREEN));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = xBkup;
		input.mi.dy = yBkup;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = xBkup;
		input.mi.dy = yBkup;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(10);
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = xBkup;
		input.mi.dy = yBkup;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(100);

		// next pos
		posX = GetAbsoluteCoordinate((p.x - 1) + posX, GetSystemMetrics(SM_CXVIRTUALSCREEN));
		posY = GetAbsoluteCoordinate((p.y - 1) + posY, GetSystemMetrics(SM_CYVIRTUALSCREEN));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = posX;
		input.mi.dy = posY;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = posX;
		input.mi.dy = posY;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(10);
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = posX;
		input.mi.dy = posY;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(delay);
	}

	void moveToEmpty() {
		POINT p = { 0,0 };
		ClientToScreen(window, &p);

		// CANTO NEUTRO
		int posX = GetAbsoluteCoordinate((p.x - 1) + 577, GetSystemMetrics(SM_CXVIRTUALSCREEN));
		int posY = GetAbsoluteCoordinate((p.y - 1) + 440, GetSystemMetrics(SM_CYVIRTUALSCREEN));
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = posX;
		input.mi.dy = posY;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
	}

	int getWindowX() {
		return 0;
	}

	int getWindowY() {
		return 0;
	}

	COLORREF getPixel(int x, int y) {
		return GetPixel(GetWindowDC(window), x, y);
	}
};
