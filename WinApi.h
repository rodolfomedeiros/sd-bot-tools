#pragma once
#include "Windows.h"

void keyPressF1(HWND hwnd) {
	SendMessage(hwnd, WM_KEYDOWN, VK_F1, 0);
	Sleep(10);
	SendMessage(hwnd, WM_KEYUP, VK_F1, 0);
	Sleep(100);
}

void keyPressF2(HWND hwnd) {
	SendMessage(hwnd, WM_KEYDOWN, VK_F2, 0);
	Sleep(10);
	SendMessage(hwnd, WM_KEYUP, VK_F2, 0);
	Sleep(100);
}

void sendClick(HWND window, UINT msg, int lparam, int sleep) {
	SendMessage(window, msg, 0, lparam);
	Sleep(sleep);
}

void itemToTrash(HWND window, int x, int y) {
	// click item
	SendMessage(window, WM_LBUTTONDOWN, 0, (y << 16) + x);
	Sleep(10);
	SendMessage(window, WM_LBUTTONUP, 0, (y << 16) + x);
	Sleep(50);
	// click trash
	SendMessage(window, WM_LBUTTONDOWN, 0, (362 << 16) + 769);
	Sleep(100);
	SendMessage(window, WM_LBUTTONUP, 0, (362 << 16) + 769);
	Sleep(100);
	// send enter key
	PostMessage(window, WM_KEYDOWN, VK_RETURN, 1);
	Sleep(100);
}
