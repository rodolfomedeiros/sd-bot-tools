# Knowledge: OpenCV Integration

## Library
- **OpenCV 4.11+** (single-lib build: `opencv_world4110d.lib` debug / `opencv_world4110.lib` release)
- Linked via MSBuild `.vcxproj` additional dependencies
- Header: `#include <opencv2/opencv.hpp>`

## Screen Capture
```cpp
// Captures game window as a 800×600 BGRA cv::Mat
void captureScreenMat(HWND hwnd, cv::Mat& src);
```
- Uses `StretchBlt` + `GetDIBits` with a `BITMAPINFOHEADER` (32-bit, RGB).
- Result is stored in a `CV_8UC4` Mat.
- Always release GDI objects (`DeleteObject`, `DeleteDC`, `ReleaseDC`) to avoid leaks.

## Region Crop
```cpp
// Crops a rectangular region from an existing Mat
void createMatFromMatSrc(cv::Mat& src, cv::Mat& tmp, int xInit, int yInit, int width, int height);
```
- Uses `cv::Rect` and Mat ROI — no data copy.

## Template Matching
```cpp
cv::matchTemplate(matTmp, item->getMat(), matResult, cv::TM_CCOEFF_NORMED);
double score;
cv::minMaxLoc(matResult, 0, &score);
if (score > rate) { /* matched */ }
```
- Method: `TM_CCOEFF_NORMED` (range −1 to 1; higher = better match).
- Default threshold (`rate`): `0.6199`.

## Item Image Loading
```cpp
cv::Mat readMat(std::string code);
```
- Looks for `./items/<code>.png` using `cv::samples::findFile()`.
- Returns an empty Mat (`0×0`) if not found — callers check `matImg.data != NULL`.
- Images loaded with `cv::IMREAD_UNCHANGED` (preserves alpha channel).

## Image Saving (GetImgConfig / debug)
```cpp
bool saveMatToFile(cv::Mat src, cv::String file, cv::String filename);
```
- Uses `cv::imencode` + `cv::imwrite`.

## Important Notes
- Item template size: **32×34 px** (bag items), **35×37 px** (bank items).
- Templates must match the exact pixel region cropped from `matWindow`; coordinate accuracy is critical.
- `cv::Mat` is reference-counted — assignments are shallow copies. Use `.clone()` if independent copies are needed.
