#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

void onMouse(int, int, int, int, void*);
float dist(int, int, int, int);
uchar contrast(uchar);
uchar color(uchar);
uchar colorR(uchar);


Mat image, result;
const char window[] = "Movie Effect";
const char window_ori[] = "Original";
uchar *pixptr;

int corner_size = 0;
int edge_size = 0;

int main() {
	
	Mat ori = imread("image/hw1-3.jpg");   // Read the file
	if(ori.empty()) {
		cout << "Open image failed." << endl;
		system("pause");
		return 0;
	}
	image = ori.clone();
	
	edge_size = (image.rows-image.cols/16*9)/2;
	
	corner_size = image.cols > (image.rows-2*edge_size) ? image.cols/1.9 : (image.rows-2*edge_size)/1.9;
	
	for(int i = 0; i < image.rows; i++) {
		pixptr = image.ptr<uchar>(i);
		for(int j = 0; j < image.cols; j++) {
			// Color
			pixptr[0] = color(pixptr[0]);
			pixptr[1] = color(pixptr[1]);
			pixptr[2] = colorR(pixptr[2]);
			
			// Contrast
			pixptr[0] = contrast(pixptr[0]);
			pixptr[1] = contrast(pixptr[1]);
			pixptr[2] = contrast(pixptr[2]);
			
			// Vignette
			float scale =  (dist(j, i, image.cols/2, (image.rows-2*edge_size)/2)/corner_size - 0.7)/0.5;
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
			
			// Black Edge
			if(i < edge_size || i > image.rows-edge_size)
				pixptr[0] = pixptr[1] = pixptr[2] = 0;
			
			pixptr += 3;
		}
	}
	imwrite("image/hw1-3.png", image);
	
	namedWindow(window, WINDOW_NORMAL);
	imshow(window, image);
	namedWindow(window_ori, WINDOW_NORMAL);
	imshow(window_ori, ori);
	waitKey();
	return 0;
}

uchar contrast(uchar input) {
	float tmp = 30.0*sin((0-input)/14.5/M_PI) + input/1.25 + 30;
	if(tmp > 255) return 255;
	else return tmp;
}
uchar colorR(uchar input) {
	float tmp = 5.0*sin((0-input)/12/M_PI)*log10(input) + input/0.88 - 30;
	if(tmp > 255) return 255;
	else if(tmp < 0) return 0;
	else return tmp;
}
uchar color(uchar input) {
	float tmp = 5.0*sin((input)/12/M_PI)*log10(input) + input/1.08 + 10;
	if(tmp > 255) return 255;
	else return tmp;
}

float dist(int x, int y, int x_ori, int y_ori) {
	return sqrt(pow(x-x_ori, 2) + pow(y-y_ori, 2));
}