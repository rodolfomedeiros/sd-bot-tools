# Knowledge: Windows API Usage

## Active Implementation
`WinApiWithRealMove.h` is the active `WinAPI` class (included by `SDBotTools.h`).  
`WinApi.h` is an alternative that uses only `SendMessage` (no real mouse movement).

## Window Management
```cpp
// Find window by title
HWND hwnd = FindWindowA(NULL, "WindowTitle");

// Enumerate all windows matching title (stores in SDConfig::sdWindows)
EnumWindows(SDConfig::enumWindowsCallback, (LPARAM)this);

// Show / minimize
ShowWindow(hwnd, SW_SHOWNORMAL);
ShowWindow(hwnd, SW_MINIMIZE);
```

## Click Simulation (within game window)
```cpp
// SendMessage approach — does not require real cursor movement
LPARAM coords = MAKELPARAM(x, y);
SendMessage(window, WM_MOUSEMOVE, 0, coords);
SendMessage(window, WM_LBUTTONDOWN, 0, coords);
SendMessage(window, WM_LBUTTONUP,   0, coords);
```
Used for: item clicks inside the game window, trash clicks (via `itemToTrash`).

## Real Mouse Movement (for actions requiring cursor presence)
```cpp
// Convert pixel coords to absolute (0–65535) for SendInput
INT GetAbsoluteCoordinate(INT PixelCoordinate, INT ScreenResolution);

// Move mouse to absolute screen position
INPUT input = { 0 };
ClientToScreen(window, &p);           // get window origin in screen coords
input.type = INPUT_MOUSE;
input.mi.dx = GetAbsoluteCoordinate((p.x - 1) + localX, SM_CXVIRTUALSCREEN);
input.mi.dy = GetAbsoluteCoordinate((p.y - 1) + localY, SM_CYVIRTUALSCREEN);
input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
SendInput(1, &input, sizeof(INPUT));
```
Used for: `suapItem` (item swap), `clickTab` (bank/bag tabs), `moveToEmpty` (cursor reset).

## Keyboard Input
```cpp
// F1 / F2 to switch bag tabs
SendMessage(window, WM_KEYDOWN, VK_F1, 0);
Sleep(10);
SendMessage(window, WM_KEYUP,   VK_F1, 0);

// Enter key (confirm dialog)
PostMessage(window, WM_KEYDOWN, VK_RETURN, 1);
```

## Pixel Color Reading
```cpp
COLORREF color = GetPixel(GetWindowDC(window), x, y);
```

## Important Notes
- The bot must run as **Administrator** for `SendInput` to work on elevated game processes.
- `ClientToScreen` is required to convert window-local coordinates to screen coordinates for `SendInput`.
- `moveToEmpty()` moves the cursor to a safe empty area (577, 440) and right-clicks to dismiss tooltips before each window iteration.
- Delays (`Sleep`) between input events are critical for game responsiveness — do not remove them.
