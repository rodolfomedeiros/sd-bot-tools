# Knowledge: Build & Development Setup

## Requirements
| Tool | Version |
|---|---|
| Visual Studio | 2022+ (MSVC v145 toolset) |
| Windows SDK | 10.0 |
| OpenCV | 4.11+ (single-lib build) |
| Platform | Windows x64 only |
| C++ Standard | C++20 (`/std:c++20`) |

## OpenCV Setup
1. Download OpenCV 4.11+ from https://opencv.org/releases/
2. Extract to a known path (e.g., `C:\opencv`)
3. In Visual Studio project properties:
   - **C/C++ → General → Additional Include Directories:** `C:\opencv\build\include`
   - **Linker → General → Additional Library Directories:** `C:\opencv\build\x64\vc16\lib`
   - **Linker → Input → Additional Dependencies (Debug):** `opencv_world4110d.lib`
   - **Linker → Input → Additional Dependencies (Release):** `opencv_world4110.lib`
4. Copy `opencv_world4110d.dll` / `opencv_world4110.dll` to the output directory or add to `PATH`.

## Build Configurations
| Configuration | Notes |
|---|---|
| Debug x64 | PDB output, incremental linking, `/MDd`, `/Od`, `/ZI` |
| Release x64 | Optimized, `/MD` |

## Key Compiler Flags
| Flag | Purpose |
|---|---|
| `/std:c++20` | C++20 standard |
| `/permissive-` | Strict conformance mode |
| `/W3` | Warning level 3 |
| `/WX-` | Warnings are NOT treated as errors |
| `/sdl` | Additional security checks |
| `/EHsc` | C++ exception handling |
| `/GS` | Buffer security checks |

## Running the Bot
1. **Run Visual Studio (or the built `.exe`) as Administrator.**
2. Open the game window at **800×600** resolution.
3. Open the in-game bag and place it in the **upper-right** of the game window.
4. Launch `sd-bot-tools.exe` — select a script from the console menu.

## Common Build Issues
| Warning | Cause | Fix |
|---|---|---|
| C4102 | Unreferenced label (`goto` target that is never jumped to) | Remove the label |
| C4267 | `size_t` → `int` implicit narrowing | Use `static_cast<int>(...)` |
| C4244 | `double` → `float` narrowing | Use explicit cast |

## Project Files
| File | Purpose |
|---|---|
| `sd-bot-tools.sln` | Solution file |
| `sd-bot-tools.vcxproj` | MSBuild project (includes, libs, flags) |
| `sd-bot-tools.vcxproj.filters` | Source file grouping in Solution Explorer |
