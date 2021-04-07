#pragma once
#include <Windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>

/**
 * Create a Bitmap file header..
 *
 * @param hwindowDC : window handle.
 * @param widht	    : image width.
 * @param height    : image height.
 *
 * @return Bitmap header.
 */
BITMAPINFOHEADER createBitmapHeader(int width, int height);

/**
 * Capture a screen window as a matrix.
 *
 * @param hwnd : window handle.
 *
 * @return Mat (Mat of the captured image)
 */
void captureScreenMat(HWND hwnd, cv::Mat& src);

void createMatFromMatSrc(cv::Mat& src, cv::Mat& tmp, int xInit, int yInit, int width, int height);

boolean saveMatToFile(cv::Mat src, cv::String file, cv::String filename);