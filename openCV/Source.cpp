#include <iostream>
#include <opencv/cv.h>
#include "opencv2/imgproc/imgproc.hpp"      
#include <opencv2/highgui.hpp> 
#include "opencv2/core.hpp"
#include <math.h>


using namespace std;
using namespace cv;


int thresh = 50, N = 11;
const char* wndname = "OBRAZ";

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}



static void findSquares(const Mat& image, vector<vector<Point> >& squares)
{
	squares.clear();

	Mat pyr, timg, gray0(image.size(), CV_8U), gray;

	pyrDown(image, pyr, Size(image.cols / 2, image.rows / 2));
	pyrUp(pyr, timg, image.size());
	vector<vector<Point> > contours;

	// find squares
	for (int c = 0; c < 3; c++)
	{
		int ch[] = { c, 0 };
		mixChannels(&timg, 1, &gray0, 1, ch, 1);

		for (int l = 0; l < N; l++)
		{

			if (l == 0)
			{

				Canny(gray0, gray, 0, thresh, 5);

				dilate(gray, gray, Mat(), Point(-1, -1));
			}
			else
			{
				//    apply threshold if l!=0:
				//    tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
				gray = gray0 >= (l + 1) * 255 / N;
			}
		
//#############################################################################################################

			findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

			vector<Point> approx;
			
			
				
			
			for (size_t i = 0; i < contours.size(); i++)
			{

				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);


				if (approx.size() == 4 &&
					fabs(contourArea(Mat(approx))) > 1000 &&
					isContourConvex(Mat(approx)))
				{
					double maxCosine = 0;

					for (int j = 2; j < 5; j++)
					{

						double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
						maxCosine = MAX(maxCosine, cosine);

					}
					approx.size();
					{
						
						Point_<double> a = approx[0].x;
						Point_<double> b = approx[1].x;
						Point_<double> c = approx[2].y;
						Point_<double> d = approx[3].y;

						if((approx[0].x - approx[2].x) > (approx[0].y - approx[3].y))
						{
							cout << "Pionowy" << endl;
						}
						else
							cout << "Poziomy" << endl;
						

						
		
						/*cout << "a" << a << endl;
						cout << "b" << b << endl;
						cout << "c" << c << endl;
						cout << "d" << d << endl;*/

						/*if (bokb < boka)
						{
							cout << "Poziomy" << endl;
						}
						else
							cout << "pionowy" << endl;*/
						
						/*cout << a + d<<endl;
						cout << a + b<<endl;*/
						


					}
					
					if (maxCosine < 0.05)
						squares.push_back(approx);
				
					

					}
				}
			}
//##############################################################################################################			
			/*findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
			vector<Point> checkapp;

			for (size_t i = 0; i < contours.size(); i++)
			{

				approxPolyDP(Mat(contours[i]), checkapp, arcLength(Mat(contours[i]), true)*0.02, true);


				if (checkapp.size() == 4 &&
					fabs(contourArea(Mat(checkapp))) > 1000 &&
					isContourConvex(Mat(checkapp)))
				{
					double maxCosine = 0;

					for (int j = 2; j < 5; j++)
					{
						double cosine = fabs(angle(checkapp[j % 4], checkapp[j - 2], checkapp[j - 1]));
						maxCosine = MAX(maxCosine, cosine);
					}

					if (maxCosine < 0.3)
						squares.push_back(checkapp);
					
					cout << "Jest Poziomy" << endl;
				}
			}*/




		}
	}



// the function draws all the squares in the image
static void drawSquares(Mat& image, const vector<vector<Point> >& squares)
{
	for (size_t i = 0; i < squares.size(); i++)
	{
		const Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
		
	}

	
						
	imshow(wndname, image);
}


int main(int argc, char** argv)
{
	VideoCapture cap(-1);

	if (!cap.isOpened())
	{
		cout << "Brak kamerki" << endl;
		return -1;
	}
	Mat frame;
	do {

		

		bool bSuccess = cap.read(frame); 
		if (!bSuccess) 
		{
			cout << "brak kamerki" << endl;
			break;
		}

		namedWindow(wndname, 1);
		vector<vector<Point> > squares;
		//vector<Point>  squarescheck;
		//squarescheck.push_back(Point(1, 2));
		//squarescheck.push_back(Point(1, 2));

		
		
		//double poziomy(cv::Point2f punt1, cv::Point2f punt2);
		

		findSquares(frame, squares);
		drawSquares(frame, squares);
		
	
		

	} while ((waitKey(1) & 0x0ff) != 27);

	return 0;
	}


//int main(int argc, char** argv) {
//
//	
//
//	VideoCapture cap(-1);
//
//	if (!cap.isOpened())
//	{
//		cout << "Brak kamerki" << endl;
//		return -1;
//	}
//	Mat frame;
//	namedWindow("pionowe", 1);
//	do {
//
//		Mat gray;
//
//		bool bSuccess = cap.read(frame); // wczytanie frame
//
//		if (!bSuccess) // przerwanie pêtli 
//		{
//			cout << "brak kamerki" << endl;
//			break;
//		}
//
//		if (frame.channels() == 3)
//		{
//			cvtColor(frame, gray, CV_BGR2GRAY);
//		}
//
//		else
//		{
//			gray = frame;
//		}
//		//Orginalny Obraz
//		imshow("normal", frame);
//		//zmiana obrazu na graya
//		adaptiveThreshold(~gray, gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
//		Mat vertical = gray.clone();
//		int horizontalsize = gray.cols / 30;
//
//		//Detekcja Pionowa
//		Mat structureOrigin = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
//		Mat gray1 = frame;
//		erode(gray, gray1, structureOrigin, Point(-1, -1));
//		dilate(gray, gray1, structureOrigin, Point(-1, -1));
//
//		imshow("pionowe", gray1);
//
//		//Detekcja Linii Pionowa-Pozioma
//		Mat structure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
//
//		erode(gray, gray, structure, Point(-1, -1));
//		dilate(gray, gray, structure, Point(-1, -1));
//
//		vector<Vec2f> lines;
//		
//		//HougeLinesP
//		HoughLines(gray, lines, 1, CV_PI / 180, 150, 0, 0);
//		Canny(frame, gray, 50, 200, 3);
//
//		// rysownaie linii 
//		for (size_t i = 0; i < lines.size(); i++)
//		{
//			float rho = lines[i][0], theta = lines[i][1];
//			Point pt1, pt2;
//			double a = cos(theta), b = sin(theta);
//			double x0 = a*rho, y0 = b*rho;
//			pt1.x = cvRound(x0 + 1000 * (-b));
//			pt1.y = cvRound(y0 + 1000 * (a));
//			pt2.x = cvRound(x0 - 1000 * (-b));
//			pt2.y = cvRound(y0 - 1000 * (a));
//			line(gray, pt1, pt2, Scalar(255, 255, 255), 3, CV_AA);
//		}
//
//		imshow("Detekcja Lini", gray);
//
//		//Detekcja Pozioma
//		int verticalsize = vertical.rows / 30;
//
//		Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));
//
//		erode(vertical, vertical, verticalStructure, Point(-1, -1));
//		dilate(vertical, vertical, verticalStructure, Point(-1, -1));
//
//		imshow("Poziome", vertical);
//
//	} while ((waitKey(1) & 0x0ff) != 27);
//
//	return 0;
//}
