
#include <iostream>
#include <opencv/cv.h>
#include "opencv2/imgproc/imgproc.hpp"      
#include <opencv2/highgui.hpp> 
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	
	
	VideoCapture cap(-1);
	


	if (!cap.isOpened())
	{
		cout << "Brak kamerki" << endl;
		return -1;
	}
	do {

		Mat  edges;
		Mat frame;
		Mat contours;
		Mat seeline;
		cap >> frame;
		seeline = Mat::zeros(Size(200, 300), CV_8UC3);
		line(frame, Point(0, 50), Point(200, 50), Scalar(0, 255, 0), 2, 8, 0);
		imshow("norml", frame);

		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		
		
		Canny(frame, contours, 50, 350);
		Mat contoursInv;
		threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);
		imshow("line", contours);

	} while ((waitKey(1) & 0x0ff) != 27);

	return 0;
}
