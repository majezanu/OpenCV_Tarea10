//Este proyecto utiliza una clase llamada Img creada por Manuel Jes�s Zavala N��ez
//Esta clase engloba a la clase de OpenCV Mat para hacer m�s sencilla la implementaci�n de sus funciones


#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "Img.h" 
#include <iostream>
#include <stdlib.h>

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;
using namespace cv;

void OnTrackbar(int, void*);
const int MAX_IMAGE_SIZE = 400;
int g_slider;
Mat g_img;
Mat g_noisy_img;
Mat g_padded_img;
Mat g_denoised_img;
Mat g_noise;
float g_threshold = 0;




int main(int argc, char **argv)
{
	
	g_img = imread("images/lapices.jpg");
	assert(g_img.data);
	int max_dim = max(g_img.rows, g_img.cols);
	int padded_width = 0;
	int padded_height = 0;
	if (max_dim > MAX_IMAGE_SIZE) {
		resize(g_img, g_img, Size(g_img.cols*MAX_IMAGE_SIZE / max_dim, g_img.rows*MAX_IMAGE_SIZE / max_dim));
	}

	padded_width = pow(2, ceil(log(g_img.cols) / log(2)));
	padded_height = pow(2, ceil(log(g_img.rows) / log(2)));

	Img noisy;
	noisy.setMat(g_img);
	g_noisy_img = noisy.Gauss(0, 50);

	copyMakeBorder(g_noisy_img, g_padded_img, 0, padded_height - g_img.rows, 0, padded_width - g_img.cols, BORDER_CONSTANT);
	namedWindow("Haar wavelet denoising");
	createTrackbar("Coef.", "Haar wavelet denoising", &g_slider, 100, OnTrackbar);
	OnTrackbar(g_threshold, 0);
	waitKey(0);
	return 0;
}

void OnTrackbar(int, void*)
{
	Img denoise;
	g_threshold = 0.1 * g_slider / 100.0;
	if (g_padded_img.channels() == 3) {
		Mat bgr[3];
		split(g_padded_img, bgr);
		
		denoise.setMat(bgr[0]);
		bgr[0] = denoise.Denoise(g_threshold);
		denoise.setMat(bgr[1]);
		bgr[1] = denoise.Denoise(g_threshold);
		denoise.setMat(bgr[2]);
		bgr[2] = denoise.Denoise(g_threshold);
		
		merge(bgr, 3, g_denoised_img);
	}
	else {
		denoise.setMat(g_denoised_img);
		g_denoised_img = denoise.Denoise(g_threshold);
		
	}
	g_denoised_img = g_denoised_img(Range(0, g_img.rows), Range(0, g_img.cols));
	denoise.setMat(g_denoised_img);
	denoise.Show("Haar wavelet denoising");
}



