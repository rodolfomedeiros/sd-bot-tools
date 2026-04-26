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
		Sleep(400);
	}

	void keyPressF2() {
		SendMessage(window, WM_KEYDOWN, VK_F2, 0);
		Sleep(10);
		SendMessage(window, WM_KEYUP, VK_F2, 0);
		moveToEmpty();
		Sleep(400);
	}

	void itemToTrash(int x, int y) {
		Sleep(100);
		
		// ITEM
		// Define as coordenadas dentro da janela
		LPARAM coordenadas = MAKELPARAM(x, y); 
		// Envia a mensagem de movimento do mouse
		SendMessage(window, WM_MOUSEMOVE, 0, coordenadas);
		// Simula um clique na posição desejada
		SendMessage(window, WM_LBUTTONDOWN, 0, coordenadas);
		SendMessage(window, WM_LBUTTONUP, 0, coordenadas);
		Sleep(100);

		// TRASH
		INPUT input = { 0 };
		POINT p = { 0,0 };
		ClientToScreen(window, &p);
		input.type = INPUT_MOUSE;
		input.mi.dx = GetAbsoluteCoordinate((p.x - 1) + 720, GetSystemMetrics(SM_CXVIRTUALSCREEN)); // desired X coordinate
		input.mi.dy = GetAbsoluteCoordinate((p.y - 1) + 405, GetSystemMetrics(SM_CYVIRTUALSCREEN)); ; // desired Y coordinate
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		SendInput(1, &input, sizeof(INPUT));
		coordenadas = MAKELPARAM(720, 405);
		// Envia a mensagem de movimento do mouse
		SendMessage(window, WM_MOUSEMOVE, 0, coordenadas);
		// Simula um clique na posição desejada
		SendMessage(window, WM_LBUTTONDOWN, 0, coordenadas);
		SendMessage(window, WM_LBUTTONUP, 0, coordenadas);
		Sleep(100);
		
		//ENTER
		PostMessage(window, WM_KEYDOWN, VK_RETURN, 1);
		Sleep(500);
	}

	INT GetAbsoluteCoordinate(INT PixelCoordinate, INT ScreenResolution)
	{
		INT AbsoluteCoordinate = MulDiv(PixelCoordinate, 65535, ScreenResolution);
		return AbsoluteCoordinate;
	}

	void clickTab(int xTab, int yTab) {
		// Define as coordenadas dentro da janela
		LPARAM coordenadas = MAKELPARAM(xTab, yTab);
		for (int i = 0; i < 2; i++) {
			// Envia a mensagem de movimento do mouse
			SendMessage(window, WM_MOUSEMOVE, 0, coordenadas);
			// Simula um clique na posição desejada
			SendMessage(window, WM_LBUTTONDOWN, 0, coordenadas);
			SendMessage(window, WM_LBUTTONUP, 0, coordenadas);
			Sleep(50);
		}
		Sleep(300);
	}

	void suapItem(int xBkup, int yBkup, int posX, int posY, int delay) {
		// Define as coordenadas dentro da janela
		LPARAM coordenadas = MAKELPARAM(xBkup, yBkup);
		// Envia a mensagem de movimento do mouse
		SendMessage(window, WM_MOUSEMOVE, 0, coordenadas);
		// Simula um clique na posição desejada
		SendMessage(window, WM_LBUTTONDOWN, 0, coordenadas);
		SendMessage(window, WM_LBUTTONUP, 0, coordenadas);
		Sleep(150);

		INPUT input = { 0 };
		POINT p = { 0,0 };
		ClientToScreen(window, &p);
		input.type = INPUT_MOUSE;
		input.mi.dx = GetAbsoluteCoordinate((p.x - 1) + posX, GetSystemMetrics(SM_CXVIRTUALSCREEN)); // desired X coordinate
		input.mi.dy = GetAbsoluteCoordinate((p.y - 1) + posY, GetSystemMetrics(SM_CYVIRTUALSCREEN)); ; // desired Y coordinate
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		SendInput(1, &input, sizeof(INPUT));

		// next pos
		// Define as coordenadas dentro da janela
	    coordenadas = MAKELPARAM(posX, posY);
		// Envia a mensagem de movimento do mouse
		SendMessage(window, WM_MOUSEMOVE, 0, coordenadas);
		// Simula um clique na posição desejada
		SendMessage(window, WM_LBUTTONDOWN, 0, coordenadas);
		SendMessage(window, WM_LBUTTONUP, 0, coordenadas);
		Sleep(delay);
	}

	void moveToEmpty() {
		INPUT input = { 0 };
		POINT p = { 0,0 };
		ClientToScreen(window, &p);
		input.type = INPUT_MOUSE;
		input.mi.dx = GetAbsoluteCoordinate((p.x - 1) + 577, GetSystemMetrics(SM_CXVIRTUALSCREEN)); // desired X coordinate
		input.mi.dy = GetAbsoluteCoordinate((p.y - 1) + 440, GetSystemMetrics(SM_CYVIRTUALSCREEN)); ; // desired Y coordinate
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		SendInput(1, &input, sizeof(INPUT));
		LPARAM coordenadas = MAKELPARAM(577, 440);
		// Simula um clique na posição desejada
		SendMessage(window, WM_RBUTTONDOWN, 0, coordenadas);
		SendMessage(window, WM_RBUTTONUP, 0, coordenadas);
		Sleep(50);
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
