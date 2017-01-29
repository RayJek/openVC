#include <iostream>
#include <opencv/cv.h>
#include "opencv2/imgproc/imgproc.hpp"      
#include <opencv2/highgui.hpp> 
#include <stdlib.h>
#include <stdio.h>
#include "opencv2/imgcodecs.hpp"



using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	
	VideoCapture cap(-1);



	if (!cap.isOpened())
	{
		cout << "Brak kamerki" << endl;
		return -1;
	}
	Mat frame;
	cap >> frame;

	do {
	
	
	Mat gray;


	if (frame.channels() == 3)
	{
		cvtColor(frame, gray, CV_BGR2GRAY);
	}
	else
	{
		gray = frame;
	}

	adaptiveThreshold(~gray, gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
	Mat vertical = gray.clone();
	int horizontalsize = gray.cols / 30;


	//Detekcja Pionowa
	Mat structure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));

	erode(gray, gray, structure, Point(-1, -1));
	dilate(gray, gray, structure, Point(-1, -1));

	imshow("Pionowe", gray);
	imwrite("../data/PionowaLinia.jpg", gray);


	//Detekcja Pozioma
	int verticalsize = vertical.rows / 30;

	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));

	erode(vertical, vertical, verticalStructure, Point(-1, -1));
	dilate(vertical, vertical, verticalStructure, Point(-1, -1));

	imshow("Poziome", vertical);
	imwrite("../data/PoziomaLinia.jpg", vertical);




	} while ((waitKey(1) & 0x0ff) != 27);

	return 0;
}
