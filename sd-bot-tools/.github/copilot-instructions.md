# Copilot Instructions — sd-bot-tools

## Project Overview
`sd-bot-tools` is a **C++ automation bot** for the SD MMORPG game (Shadow Dynasty / similar).  
It uses **OpenCV** for image recognition and **Windows API** for window and input automation.  
The goal is to automate repetitive in-game tasks such as dropping unwanted items, depositing items to the bank, gathering items, and capturing screen images.

## Tech Stack
- **Language:** C++20
- **IDE:** Visual Studio 2022+ (v145 toolset)
- **Libraries:** OpenCV 4.11+, Windows API (Win32)
- **Platform:** Windows x64 only
- **Build system:** MSBuild via `.vcxproj`

## Architecture

### Entry Point
- `main.cpp` — selects a script via `SDConfig::getConfig()` then calls `init()`, `start()`, and `loop()`.

### Core Classes (SDBotTools.h / SDBotTools.cpp)
| Class | Responsibility |
|---|---|
| `SDWindow` | Wraps a game `HWND` window handle |
| `Pos` | Stores an (x, y) position with a backup flag |
| `Item` | Represents a game item: code, gather flag, OpenCV `cv::Mat` template |
| `Cargo` | Abstract base for inventory containers (bag/bank); manages grid navigation |
| `Bag : Cargo` | Player inventory bag; navigates slots, supports 2 tabs (F1/F2) |
| `Bank : Cargo` | Bank container; navigates slots across multiple tabs |
| `SDConfig` | Base script config: manages windows, items, speed, rate, loop |
| `SDDropConfig : SDConfig` | Script to drop unwanted items from bag |
| `SDDropWithBankConfig : SDDropConfig` | Script to drop items and also transfer selected items to bank |
| `SDGatherConfig : SDConfig` | Script to gather/consolidate items within the bag |
| `GetImgConfig : SDConfig` | Utility to capture and save bag images |
| `OcrImageConfig : SDConfig` | Utility for OCR-based image config |

### Support Headers
| File | Responsibility |
|---|---|
| `OpenCVAPI.h` | Screen capture (`captureScreenMat`), Mat region crop, template matching helpers, item image loading from `./items/` |
| `WinApiWithRealMove.h` | `WinAPI` class — real mouse movement via `SendInput` + `SendMessage`, keyboard input, pixel color reading |
| `WinApi.h` | Alternative `WinAPI` class using only `SendMessage` (no real mouse move) |

### Item Images
- Stored in `./items/<code>.png` (e.g., `items/5212.png`)
- Item codes are 4-digit numeric strings
- Items are loaded via `SDConfig::readItem()` → `OpenCVAPI::readMat()`
- `items/code-items-drop.txt` and `items/--drops.txt` contain pre-configured drop code lists per farming location

## Key Design Patterns
- **Template method:** `SDConfig` defines `init()` / `loop()`; subclasses override `start()` and `run(SDWindow*)`.
- **Factory / menu:** `SDConfig::getConfig()` presents a console menu and returns the appropriate subclass instance.
- **Grid navigation:** `Bag::upXY()` and `Bank::upXY()` advance slot coordinates with pixel-accurate offsets, including column/row correction factors.
- **Item matching:** `cv::matchTemplate` with `TM_CCOEFF_NORMED`; items above `rate` threshold are matched.
- **Window enumeration:** `enumWindowsCallback` finds all game windows by title using `EnumWindows`.

## Coding Conventions
- Headers use `#pragma once`.
- Use `static_cast<int>()` when converting `size_t` to integer types.
- Labels (`goto`) are **not used** — avoid them entirely.
- Class members follow camelCase; methods follow camelCase.
- `cout` / `cin` used directly (no logging framework).
- The `WinApiWithRealMove.h` version of `WinAPI` is the **active** one (included by `SDBotTools.h`); `WinApi.h` is an alternative.

## Build Requirements
- OpenCV must be installed and `opencv_world4110d.lib` (debug) / `opencv_world4110.lib` (release) must be on the library path.
- Build target: `x64`, Debug or Release.
- Run as **Administrator** (required for `SendInput` to game windows).

## Important Notes
- The game window must be **800×600**.
- The bag must be **open** and positioned in the **upper-right** of the game window before running any script.
- `SDConfig::init()` currently has a hardcoded window title (`"teste"`); the full interactive `init()` is commented out above it.
- Item slot coordinates use pixel offsets with small correction values tuned for the specific game UI layout.
