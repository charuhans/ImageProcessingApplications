#include<stdio.h>
#include<cv.h>
#include<cxcore.h>
#include<highgui.h>
#include <iostream>
#include "carttopol.h"
#include "findradius.h"
#include "polartocortical.h"
#include <windows.h>
using namespace std;


IplImage *img;
int count;

void myMouse(int event, int x, int y, int flags, void * param)
{
	if(event == CV_EVENT_LBUTTONDOWN)
	{
		if(count != 0)
		{
			cvDestroyWindow("cortical Image");
			cvDestroyWindow("Polar Image");
			cvDestroyWindow("Polar Representation");
			cvDestroyWindow("Cortical Representation");
			system("cls");
		}

		cout<<"Point selected by the user is ("<<x<<","<<y<<")";
		int radius = find_radius( x,  y,  img->height, img->width);
		cout<<endl<<endl<<" Calling Cartesian to Polar function ....."<<endl;
		//Sleep(1000);
		int theta = 64;
		IplImage* polar_representation  = cart_to_pol(img,radius,x,y,theta);
		

		int** polar_matrix;
		polar_matrix = new int*[radius];
		for(int i = 0; i < radius; i++)
			polar_matrix[i] = new int[theta];

		cvNamedWindow("Polar Representation",0);
		cvShowImage("Polar Representation",polar_representation);
		//cvWaitKey(0);

		cout<<endl<<" Calling Polar to Cortical function ...."<<endl;
		//Sleep(1000);

		for(int i = 0; i < radius; i++)
			for(int j = 0; j < theta; j++)
				polar_matrix[i][j] = polar_representation->imageData[i*polar_representation->widthStep+j];

		//IplImage* cortical_image = 
		polar_to_cortical(polar_matrix,radius,x,y,theta);

		count++;

	}
}



int main(int argc, char** argv)
{
	img = cvLoadImage("boats.tif", CV_LOAD_IMAGE_COLOR);
	count = 0;
	cvNamedWindow("image",0);
	cvSetMouseCallback("image", myMouse);
	cvShowImage("image", img);
	
	cvWaitKey(0);
	
	cvDestroyWindow("image");
	cvReleaseImage(&img);
	
	return 0;
}

	

