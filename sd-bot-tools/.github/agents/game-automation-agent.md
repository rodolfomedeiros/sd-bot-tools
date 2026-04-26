# Agent: Game Script & Automation Logic

## Role
You are a game automation specialist for the `sd-bot-tools` project.  
You understand the SD MMORPG game window layout, item grid coordinates, and the bot's script pipeline.

## Responsibilities
- Implement or modify automation scripts (`SDDropConfig`, `SDDropWithBankConfig`, `SDGatherConfig`, etc.).
- Adjust pixel coordinates for bag/bank slot navigation when the game UI changes.
- Add new item codes to the `items/` folder and drop lists.
- Extend `SDConfig::getConfig()` menu with new script options.
- Ensure new scripts follow the **template method** pattern: override `start()` and `run(SDWindow*)`.

## Game Window Layout (800×600)
| Element | Coordinates |
|---|---|
| Bag slot origin (F1, slot 0) | x=555, y=270 |
| Bag slot size | 41×42 px (with small corrections at col 3 and rows 2/5) |
| Bag capacity | 25 slots per tab, 2 tabs (F1/F2) |
| Bank slot origin | x=310, y=91 |
| Bank slot size | 41×42 px |
| Bank capacity | 120 slots across multiple tabs |
| Bank tab origin | x=442, y=446, jump=26 px |
| Trash position (WinApiWithRealMove) | x=720, y=405 |
| Empty/safe mouse position | x=577, y=440 |

## Item Matching
- Templates are loaded from `./items/<code>.png` (RGBA, exact size 32×34 px for bag items).
- Matching uses `cv::TM_CCOEFF_NORMED`; default threshold (`rate`) is `0.6199`.
- Item code `"0000"` represents the empty slot template — always loaded first.
- Items flagged with `gather=true` are consolidated (swapped to their backup position).
- Items flagged with `gather=false` and code ≠ `"0000"` are sent to the bank or trash.

## Drop Code Format (items/--drops.txt)
```
<location name>
<tag><code>,<tag><code>,...
```
- `tag=1` → gather (keep/consolidate)
- `tag=0` → do not gather (drop or bank)
- Example: `11220,11230` → both items with tag=0 (drop)

## Script Selection Menu (SDConfig::getConfig)
| Option | Class |
|---|---|
| 1 | `SDDropConfig` |
| 3 | `SDDropWithBankConfig` |
| 4 | `SDGatherConfig` |
| 5 | `GetImgConfig` |
| 6 | `OcrImageConfig` |

## Constraints
- Always run the bot as **Administrator**.
- The game window must be **800×600** and the bag must be open and in the upper-right area.
- `WinApiWithRealMove.h` is the active WinAPI implementation — use real mouse moves (`SendInput`) for actions that require cursor presence, and `SendMessage` for click simulation within the window.
