#pragma once
#include <Windows.h>
#include <opencv2/opencv.hpp>

class OpenCVAPI {
public:
	/**
	 * Create a Bitmap file header..
	 *
	 * @param hwindowDC : window handle.
	 * @param widht	    : image width.
	 * @param height    : image height.
	 *
	 * @return Bitmap header.
	 */
	BITMAPINFOHEADER createBitmapHeader(int width, int height){
		BITMAPINFOHEADER  bi;

		// create a bitmap
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = width;
		bi.biHeight = -height;  //this is the line that makes it draw upside down or not
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		return bi;
	}

	/**
	 * Capture a screen window as a matrix.
	 *
	 * @param hwnd : window handle.
	 *
	 * @return Mat (Mat of the captured image)
	 */
	void captureScreenMat(HWND hwnd, cv::Mat& src) {

		// get handles to a device context (DC)
		HDC hwindowDC = GetDC(hwnd);
		HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
		SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

		// define scale, height and width
		int scale = 1;
		int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
		int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
		int width = 800; //GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int height = 600; //GetSystemMetrics(SM_CYVIRTUALSCREEN);

		// create mat object
		src.create(height, width, CV_8UC4);

		// create a bitmap
		HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
		BITMAPINFOHEADER bi = createBitmapHeader(width, height);

		// use the previously created device context with the bitmap
		SelectObject(hwindowCompatibleDC, hbwindow);

		// copy from the window device context to the bitmap device context
		StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);  //change SRCCOPY to NOTSRCCOPY for wacky colors !
		GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);            //copy from hwindowCompatibleDC to hbwindow

		// avoid memory leak
		DeleteObject(hbwindow);
		DeleteDC(hwindowCompatibleDC);
		ReleaseDC(hwnd, hwindowDC);
	}

	void createMatFromMatSrc(cv::Mat& src, cv::Mat& tmp, int xInit, int yInit, int width, int height) {
		cv::Rect rectImg(xInit, yInit, width, height);
		tmp = src(rectImg);
	}

	bool saveMatToFile(cv::Mat src, cv::String file, cv::String filename) {
		std::vector<uchar> buf;
		cv::imencode(file, src, buf);
		cv::imwrite(filename + file, src);
		buf.clear();

		return true;
	}

	cv::Mat readMat(std::string code) {

		std::string strPath = cv::samples::findFile("./items/" + code + ".png", false, true);
		
		if (!strPath.empty()) {
			return cv::imread(strPath, cv::IMREAD_UNCHANGED);
		}

		return cv::Mat(0,0,1);
	}
};
