#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

float dist(int, int, int, int);

Mat ori,image;
const char window[] = "Add some light!";
//const char window_ori[] = "Original";
uchar *pixptr;

int r_scale = 100, g_scale = 100, b_scale = 100;

void onChange(int pos, void* data) {
	image = ori.clone();
	
	for(int i = 0; i < image.rows; i++) {
		pixptr = image.ptr<uchar>(i);
		for(int j = 0; j < image.cols; j++) {
			if(pixptr[0]* b_scale/100.0 > 255)
				pixptr[0] = 255;
			else
				pixptr[0] *= b_scale/100.0;
			if(pixptr[1]*g_scale/100.0 > 255)
				pixptr[1] = 255;
			else
				pixptr[1] *= g_scale/100.0;
			if(pixptr[2]*r_scale/100.0 > 255)
				pixptr[2] = 255;
			else
				pixptr[2] *= r_scale/100.0;
			pixptr += 3;
		}
	}
	imshow(window, image);
}

int main() {
	
	ori = imread("hw1-1.jpg");   // Read the file
	if(ori.empty()) {
		cout << "Open image failed." << endl;
		system("pause");
		return 0;
	}
	namedWindow(window, WINDOW_NORMAL);
	
	createTrackbar("R", window, &r_scale, 200, onChange);
	createTrackbar("G", window, &g_scale, 200, onChange);
	createTrackbar("B", window, &b_scale, 200, onChange);
	
	imshow(window, ori);
	//namedWindow(window_ori, WINDOW_NORMAL);
	//imshow(window_ori, ori);
	waitKey();
	imwrite("hw1-1.png", image);
	return 0;
}
