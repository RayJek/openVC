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
	Mat frame;
	namedWindow("pionowe", 1);
	do {

		Mat gray;

		bool bSuccess = cap.read(frame); // wczytanie frame

		if (!bSuccess) // przerwanie pêtli 
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		if (frame.channels() == 3)
		{
			cvtColor(frame, gray, CV_BGR2GRAY);
		}

		else
		{
			gray = frame;
		}
		//Orginalny Obraz
		imshow("normal", frame);
		//zmiana obrazu na graya
		adaptiveThreshold(~gray, gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
		Mat vertical = gray.clone();
		int horizontalsize = gray.cols / 30;

		//Detekcja Pionowa
		Mat structureOrigin = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
		Mat gray1 = frame;
		erode(gray, gray1, structureOrigin, Point(-1, -1));
		dilate(gray, gray1, structureOrigin, Point(-1, -1));

		imshow("pionowe", gray1);

		//Detekcja Lini Pionowa-Pozioma
		Mat structure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));

		erode(gray, gray, structure, Point(-1, -1));
		dilate(gray, gray, structure, Point(-1, -1));

		vector<Vec2f> lines;
		
		//HougeLinesP
		HoughLines(gray, lines, 1, CV_PI / 180, 150, 0, 0);
		Canny(frame, gray, 50, 200, 3);

		// rysownaie lini 
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(gray, pt1, pt2, Scalar(255, 255, 255), 3, CV_AA);
		}

		imshow("Detekcja Lini", gray);

		//Detekcja Pozioma
		int verticalsize = vertical.rows / 30;

		Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));

		erode(vertical, vertical, verticalStructure, Point(-1, -1));
		dilate(vertical, vertical, verticalStructure, Point(-1, -1));

		imshow("Poziome", vertical);

	} while ((waitKey(1) & 0x0ff) != 27);

	return 0;
}
