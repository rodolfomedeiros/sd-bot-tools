# Knowledge: Item Codes & Drop Configuration

## Item Code Format
- 4-digit numeric string: e.g., `"5212"`, `"0000"`, `"1400"`
- Code `"0000"` is the **empty slot** template — always loaded first, used as the no-match baseline.
- Item images are stored as `./items/<code>.png` (32×34 px RGBA).

## Loading Items via Code String
`SDConfig::codesToItems(std::string codes)` parses a comma-separated string:
```
<tag><code>,<tag><code>,...
```
- First character is the tag: `'1'` = gather (keep), `'0'` = drop/bank.
- Example: `"15212,05220"` → item 5212 tagged gather, item 5220 tagged drop.

## Drop Lists (items/--drops.txt)
Pre-configured code strings per farming location:
```
QUEST
11220,11230,14200,14210,14220,14230

MORLOCHS
13000,11080,14060,...
```
Copy the relevant line and paste it at runtime when prompted by `SDConfig::init()` (interactive mode).

## Adding a New Item
1. Take a screenshot of the item in the bag slot (exactly 32×34 px, RGBA).
2. Save as `items/<newcode>.png`.
3. Add the code to the relevant drop list in `items/--drops.txt`.
4. Prefix with `1` to gather or `0` to drop when using `codesToItems`.

## readItem() Behavior
```cpp
void SDConfig::readItem(std::string code, bool tag);
```
- Iterates ASCII `'0'` (48) to `'9'` (57) as the last character of the code.
- This loads all variants of an item code (e.g., `5210`, `5211`, ..., `5219`) if their images exist.
- Only items with valid image data (`matImg.data != NULL`) are added to the items list.

## Matching Threshold
- Default `rate = 0.6199` (TM_CCOEFF_NORMED score).
- Lower values increase sensitivity (more false positives); higher values increase precision (may miss items).
- Adjustable at runtime via `SDConfig::init()` interactive mode.
