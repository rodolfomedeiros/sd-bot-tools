# Knowledge: Project Architecture

## Overview
`sd-bot-tools` is a C++ automation bot for the SD MMORPG game.  
It automates bag management (drop, bank transfer, gather) using OpenCV template matching and Win32 API input simulation.

## Module Map

```
main.cpp
  └─ SDConfig::getConfig()        ← console menu, returns concrete script
       ├─ SDDropConfig
       ├─ SDDropWithBankConfig
       ├─ SDGatherConfig
       ├─ GetImgConfig
       └─ OcrImageConfig

SDConfig (base)
  ├─ inherits OpenCVAPI            ← screen capture, template matching, Mat I/O
  └─ inherits WinAPI               ← window handle, input simulation

Cargo (abstract)
  ├─ Bag                           ← player inventory, 2 tabs, 25 slots each
  └─ Bank                          ← bank storage, multi-tab, 120 slots
```

## Execution Flow
```
main()
  → script->init()     // find game window(s), set speed/rate/items
  → script->start()    // script-specific setup (print config, ask options)
  → script->loop()     // infinite loop over all SDWindow instances
       → run(window)   // one full pass per window (overridden per script)
```

## Class Inheritance
```
WinAPI ◄── Cargo ◄── Bag
                  └── Bank

OpenCVAPI ◄── SDConfig ◄── SDDropConfig ◄── SDDropWithBankConfig
WinAPI    ◄──           └── SDGatherConfig
                        └── GetImgConfig
                        └── OcrImageConfig
```

## Data Flow: SDDropConfig::run()
```
captureScreenMat(window) → matWindow (800×600 BGRA Mat)
  for each bag slot (pBag 0..24):
    createMatFromMatSrc → matTmp (32×34 crop at slot position)
    for each item template:
      matchTemplate(matTmp, item.mat) → score
      if score > rate:
        if gather → suapItem (consolidate)
        else      → bagToBankItems.push_back(pBag) or itemToTrash
    bag->upXY(pBag)   ← advance to next slot coordinates
```

## File Responsibilities
| File | Description |
|---|---|
| `main.cpp` | Entry point |
| `SDBotTools.h` | All domain classes: SDWindow, Pos, Item, Cargo, Bag, Bank, SDConfig and all script configs |
| `SDBotTools.cpp` | Implementation of Bag/Bank navigation, SDConfig methods, all script run() methods |
| `OpenCVAPI.h` | OpenCV wrapper: screen capture, Mat crop, template matching, file I/O |
| `WinApiWithRealMove.h` | Active WinAPI: real mouse movement + SendMessage click simulation |
| `WinApi.h` | Alternate WinAPI: SendMessage-only (no real mouse movement) |
| `OcrImageConfig2.h` | Stub class for future OCR config variant |
| `items/*.png` | Item template images (32×34 px, RGBA) named by 4-digit item code |
| `items/--drops.txt` | Drop code lists per farming location |
| `items/code-items-drop.txt` | Additional item code reference list |
