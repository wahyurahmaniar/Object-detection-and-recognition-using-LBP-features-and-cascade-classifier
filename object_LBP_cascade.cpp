// object.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <Windows.h>

#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\video\tracking.hpp"

using namespace cv;
using namespace std;

int frame;

int main(int argc, const char** argv)
{
	 frame=0;
	 // prepare video input
	 VideoCapture cap("video.avi");

	 // prepare video output
	 VideoWriter outputVideo;

	 outputVideo.open("video.wmv", CV_FOURCC('W', 'M', 'V', '2'), cap.get(CV_CAP_PROP_FPS), Size(640, 480), true);

	 // prepare cascadeClassifier
	 CascadeClassifier detectorCar;
	 CascadeClassifier detectorHuman;

	 string cascadeName1 = "carLBP.xml";//car
	 string cascadeName2 = "humanLBP.xml";//human 

	 // Load cascade into CascadeClassifier
	 bool loaded1 = detectorCar.load(cascadeName1);
	 bool loaded2 = detectorHuman.load(cascadeName2);


	 for (;;)
	 {

		  bool Is = cap.grab();

		  if (Is == false) 
		  {
			   cout << "Video Capture Fail" << endl;
			   break;
		  }
		  else 
		  {
			  // Just for measure time   
			  const clock_t begin_time = clock();

			  // Store results in these 2 vectors
			  vector<Rect> car;
			  vector<Rect> human;

			  // prepare 2 Mat container
			  Mat img;
			  Mat ori;

			  // capture frame from video file
			  cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);
			  // Resize image if you want with same size as your VideoWriter
			  int w,h;
			  w = img.cols;
			  h = img.rows;
			  
			  // resize for faster computation time
			  // resize(img, img, Size(w*2/3, h*2/3));//(640.480)
			  
			  // Store original colored image
			  img.copyTo(ori);
			  // color to gray image
			  cvtColor(img, img, CV_BGR2GRAY);

			  // detect objects
			  detectorCar.detectMultiScale(img, car, 1.1, 2, 0 | 1, Size(20,20), Size(80, 300));
			  detectorHuman.detectMultiScale(img, human, 1.1, 2, 0 | 1, Size(20, 20), Size(100, 300));

			   // Draw results 
			   if (car.size() > 0) 
			   {
					for (int i = 0; i < car.size(); i++) 
					{
						rectangle(ori, car[i].tl(), car[i].br(), Scalar(0,0,255), 2, 8, 0);
					}
			   }

			   if (human.size() > 0) 
			   {
					for (int j = 0; j < human.size(); j++) 
					{
						rectangle(ori, human[j].tl(), human[j].br(),  Scalar(255,0,0), 2, 8, 0);
					}
			   }

			   // measure time as current - begin_time
			   clock_t diff = clock() - begin_time;
			   // convert time into string
			   char buffer[50];
			   sprintf(buffer, "%d",diff);
			   // display TIME ms on original image
			   putText(ori, buffer, Point(100, 20), 1, 2, Scalar(255, 255, 255), 2, 8, 0);
			   putText(ori, "ms" , Point(150, 20), 1, 2, Scalar(255, 255, 255), 2, 8, 0);

			   // draw results
			   namedWindow("Object", WINDOW_AUTOSIZE);
			   imshow("Object", ori);

			   // make video output
			   // outputVideo << ori; 
			   frame++;
			   waitKey(1);

		  }
	 }
	
 	 return 0;
}

