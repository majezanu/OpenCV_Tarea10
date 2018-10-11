#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstring>

using namespace cv;
using namespace std;
const String originalName = "Original";
const String BGRName = "BGR Trackbars";
const String HLSName = "HLS Trackbars";
Mat original, placeholder;
bool show = false;
void onTrackBarWindow(int, void*);
void point(int event, int x, int y, int flags, void* userdata);

int main(int argc, char **argv)
{
	original = imread("images/Fig0635(middle_row_left_chalk ).tif");
	namedWindow(originalName, WINDOW_AUTOSIZE);
	namedWindow(BGRName, WINDOW_NORMAL);
	namedWindow(HLSName, WINDOW_NORMAL);
	setMouseCallback(originalName, point);
	createTrackbar("RMax", BGRName, 0, 255, onTrackBarWindow);
	createTrackbar("GMin", BGRName, 0, 255, onTrackBarWindow);
	createTrackbar("GMax", BGRName, 0, 255, onTrackBarWindow);
	createTrackbar("BMin", BGRName, 0, 255, onTrackBarWindow);
	createTrackbar("BMax", BGRName, 0, 255, onTrackBarWindow);
	createTrackbar("HMin", HLSName, 0, 180, onTrackBarWindow);
	createTrackbar("HMax", HLSName, 0, 180, onTrackBarWindow);
	createTrackbar("LMin", HLSName, 0, 255, onTrackBarWindow);
	createTrackbar("LMax", HLSName, 0, 255, onTrackBarWindow);
	createTrackbar("SMin", HLSName, 0, 255, onTrackBarWindow);
	createTrackbar("SMax", HLSName, 0, 255, onTrackBarWindow);
	createTrackbar("RMin", BGRName, 0, 255, onTrackBarWindow);
	imshow(originalName, original);
	imshow(BGRName, original);
	imshow(HLSName, original);
	int BMin, GMin, RMin;
	int BMax, GMax, RMax;
	Scalar minBGR, maxBGR;
	int HMin, LMin, SMin;
	int HMax, LMax, SMax;
	Scalar minHLS, maxHLS;
	Mat BGRImage, imHLS;
	Mat mask_BGR, mask_HLS;
	Mat dstBGR, dstHLS;
	char k;
	while (1) {
		k = waitKey(1) & 0xFF;
		if (k == 27) {
			break;
		}
		if (show) {
			show = false;
			BMin = getTrackbarPos("BMin", BGRName);
			GMin = getTrackbarPos("GMin", BGRName);
			RMin = getTrackbarPos("RMin", BGRName);
			BMax = getTrackbarPos("BMax", BGRName);
			GMax = getTrackbarPos("GMax", BGRName);
			RMax = getTrackbarPos("RMax", BGRName);
			minBGR = Scalar(BMin, GMin, RMin);
			maxBGR = Scalar(BMax, GMax, RMax);
			HMin = getTrackbarPos("HMin", HLSName);
			LMin = getTrackbarPos("LMin", HLSName);
			SMin = getTrackbarPos("SMin", HLSName);
			HMax = getTrackbarPos("HMax", HLSName);
			LMax = getTrackbarPos("LMax", HLSName);
			SMax = getTrackbarPos("SMax", HLSName);
			minHLS = Scalar(HMin, LMin, SMin);
			maxHLS = Scalar(HMax, LMax, SMax);
			original.copyTo(BGRImage);
			cvtColor(original, imHLS, COLOR_BGR2HLS);
			inRange(BGRImage, minBGR, maxBGR, mask_BGR);
			dstBGR = Mat::zeros(original.rows, original.cols, CV_8UC3);
			bitwise_and(original, original, dstBGR, mask_BGR);
			inRange(imHLS, minHLS, maxHLS, mask_HLS);
			dstHLS = Mat::zeros(original.rows, original.cols, CV_8UC3);
			bitwise_and(original, original, dstHLS, mask_HLS);
			imshow(BGRName, dstBGR);
			imshow(HLSName, dstHLS);
		}
	}
	destroyAllWindows();
	return 0;
}

void onTrackBarWindow(int pos, void* userdata) {
	show = true;
	return;
}

void point(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_MOUSEMOVE)
	{
		if (y < original.rows && x < original.cols) {
			Vec3b bgrPixel(original.at<Vec3b>(y, x));
			Mat3b hls;
			Mat3b bgr(bgrPixel);
			cvtColor(bgr, hls, COLOR_BGR2HLS);
			Vec3b hlsPixel(hls.at<Vec3b>(0, 0));
			placeholder = Mat::zeros(original.rows, 400, CV_8UC3);
			putText(placeholder, format("RGB [%d, %d, %d]", bgrPixel[2], bgrPixel[1], bgrPixel[0]), Point(20, 140), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 1);
			putText(placeholder, format("HLS [%d, %d, %d]", hlsPixel[0], hlsPixel[1], hlsPixel[2]), Point(20, 210), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 1);
			Size sz1 = original.size();
			Size sz2 = placeholder.size();
			Mat combinedDst(sz1.height, sz1.width + sz2.width, CV_8UC3);
			Mat left(combinedDst, Rect(0, 0, sz1.width, sz1.height));
			original.copyTo(left);
			Mat right(combinedDst, Rect(sz1.width, 0, sz2.width, sz2.height));
			placeholder.copyTo(right);
			imshow(originalName, combinedDst);
		}
	}
}