
#include <iostream>
#include <opencv/cv.h>
#include "opencv2/imgproc/imgproc.hpp"      
#include <opencv2/highgui.hpp> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <deque>
#include "opencv/cxcore.h"
#include <opencv2/core/types_c.h>

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
		
		const int MAX_NUM_OBJECTS = 50;
		const int MAX_OBJECT_AREA = 20 * 20;
		Mat  edges;
		Mat frame;
		Mat contours;
		Mat seeline;
		cap >> frame;

		//rysowanie lini
		seeline = Mat::zeros(Size(100, 100), CV_8UC3);
		line(frame, Point(0, 50), Point(320, 50), Scalar(0, 255, 0), 4, 18, 0);
		//standardowy obraz
		imshow("norml", frame);
		
		

		// znieksztalcnie
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		//okno 1
		imshow("edges", edges);
		
		//mniejsze szumy
		Canny(frame, contours, 50, 350);
		Mat contoursInv;
		threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);
		//okno 2
		imshow("line", contours);

	} while ((waitKey(1) & 0x0ff) != 27);

	return 0;
}
