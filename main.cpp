#include <stdio.h>
#include<iostream>
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

string my_pic = "07.png";
Mat image = imread(my_pic, CV_LOAD_IMAGE_GRAYSCALE);

void on_trackbar(int p, void*) {
//************************Scale***************************
	Mat src = imread(my_pic, CV_LOAD_IMAGE_GRAYSCALE);
	int c = cvGetTrackbarPos("Width", "after change");
	int r = cvGetTrackbarPos("Height", "after change");
	int level = cvGetTrackbarPos("Level", "after change");
	int sh = src.rows;
	int sw = src.cols;
	int sx, sy;

	Mat dst(r, c, src.type());
	for (int i = 0; i < r; i ++) {
		for (int j = 0; j < c; j ++) {
			double x = i / (r + 0.0);
			double y = j / (c + 0.0);
			sx = sh * x;
			sy = sw * y;
			dst.at<uchar>(i, j) = src.at<uchar>(sx,sy);
		}
	}
//********************Quantization************************
	//使得level的值合法
	if (level <= 256 && level >= 128)level = 128;
	else if(level < 128 && level >= 64)level = 64;
	else if(level < 64 && level >= 32)level = 32;
	else if(level < 32 && level >= 16) level = 16;
	else if(level < 16 && level >= 8)level = 8;
	else if(level < 8 && level >= 4)level = 4;
	else level = 2;

	//每次操作都重新载入一次图片
	int channels = dst.channels();
	int nrows = dst.rows;
	int ncols = dst.cols * channels;

	uchar table[256];
	int degree = 255/(level - 1);
	int number = 256 / level;
	int count = 0;
	int value = 0;

	for (int i = 0; i < 256; i ++, count ++) {
		if (count < number) table[i] = value;
		else {
			count = 0;
			value += degree;
			table[i] = value;
		}
	}

	if (src.isContinuous()) {
		ncols *= nrows;
		nrows = 1;
	}

	for (int i = 0; i < nrows; i ++) {
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 0; j < ncols; j ++) {
			p[j] = table[p[j]];
		}
	}

	//cout << level << " " << c << " " << r << "\n";
    	imshow("after change", dst);
}

int main( int argc, char** argv) {
	imshow("before change", image);

	int width = 384;
	int height = 256;
	int level = 256;
	namedWindow("after change", CV_WINDOW_AUTOSIZE);
	createTrackbar( "Width", "after change", &width, 800, on_trackbar);
	createTrackbar( "Height", "after change", &height, 500, on_trackbar);
	createTrackbar( "Level", "after change", &level, 256, on_trackbar); //slider

	on_trackbar(0, 0);

	waitKey();
	return 0;
}
