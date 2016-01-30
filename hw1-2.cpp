#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

void onMouse(int, int, int, int, void*);
float dist(int, int, int, int);
float light_dist(int, int, int, int);

Mat image, result;
const char window[] = "Add some light!";
const char window_ori[] = "Original";
uchar *pixptr;

int light_halfsize = 0;
int corner_size = 0;
const int MAX_BLUE = 200;
const int MAX_GREEN = 230;
const int MAX_RED = 255;


int main() {
	
	Mat ori = imread("image/hw1-2.jpg");   // Read the file
	if(ori.empty()) {
		cout << "Open image failed." << endl;
		system("pause");
		return 0;
	}
	image = ori.clone();
	
	
	corner_size = image.cols > image.rows ? image.cols/1.9 : image.rows/1.9;
	
	for(int i = 0; i < image.rows; i++) {
		pixptr = image.ptr<uchar>(i);
		for(int j = 0; j < image.cols; j++) {
			float scale =  (dist(j, i, image.cols/2, image.rows/2)/corner_size - 0.5)/0.5;
			if(scale > 1) scale = 1;
			else if(scale < 0) scale /= 10;
			if(pixptr[0]*(1-scale) > 255)
				pixptr[0] = 255;
			else
				pixptr[0] *= 1 - scale;
			if(pixptr[1]*(1-scale) > 255)
				pixptr[1] = 255;
			else
				pixptr[1] *= 1 - scale;
			if(pixptr[2]*(1-scale) > 255)
				pixptr[2] = 255;
			else
				pixptr[2] *= 1 - scale;
			pixptr += 3;
		}
	}
	cout << "Dark Corner Added." << endl;
	imwrite("image/hw1-2-1.png", image);
	
	namedWindow(window, WINDOW_NORMAL);
	imshow(window, image);
	namedWindow(window_ori, WINDOW_NORMAL);
	imshow(window_ori, ori);
	setMouseCallback(window, onMouse);
	waitKey();
	return 0;
}


void onMouse(int event, int x, int y, int flag, void* data) {
	if(event != EVENT_LBUTTONDOWN) return;
	
	result = image.clone();
	
	light_halfsize = result.cols < result.rows ? result.cols/4 : result.rows/4;
	
	int y_start = y-light_halfsize >= 0 ? y-light_halfsize : 0;
	int x_start = x-light_halfsize >= 0 ? x-light_halfsize : 0;
	
	for(int i = y_start; i < y+light_halfsize && i < result.rows; i++) {
		pixptr = result.ptr<uchar>(i);
		pixptr += x_start*3;
		for(int j = x_start; j < x+light_halfsize && j < result.cols; j++) {
			float scale = light_dist(j, i, x, y);
			pixptr[0] = pixptr[0]*(1-scale) + MAX_BLUE * scale;
			pixptr[1] = pixptr[1]*(1-scale) + MAX_GREEN * scale;
			pixptr[2] = pixptr[2]*(1-scale) + MAX_RED * scale;
			pixptr += 3;
		}
	}
	cout << "Light Added." << endl;
	imwrite("image/hw1-2-2.png", result);
	imshow(window, result);
}

float dist(int x, int y, int x_ori, int y_ori) {
	return sqrt(pow(x-x_ori, 2) + pow(y-y_ori, 2));
}

float light_dist(int x, int y, int x_ori, int y_ori) {
	float norm_dist = dist(x, y, x_ori, y_ori)/light_halfsize/0.8;
	return pow(20, 0-norm_dist);
}
