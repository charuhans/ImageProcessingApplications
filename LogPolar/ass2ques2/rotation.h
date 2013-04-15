#ifndef ROTATION_H
#define ROTATION_H

#include<iostream>
#include<cv.h>
#include<highgui.h>

using namespace std;

IplImage* rotate(IplImage* img)
{
	 
	int height = img->height;
	int width = img->width;
	int step = img->widthStep;
	int channels = img->nChannels;
	uchar* data = (uchar*)img->imageData;
	IplImage* J = cvCreateImage(cvSize(68,68),IPL_DEPTH_8U,1);			// Holds rotated
	int heightJ = J->height;
	int widthJ = J->width;
	int stepJ = J->widthStep;
	int channelsJ = J->nChannels;
	uchar* dataJ = (uchar*)J->imageData;	
	int i,j;
	double root2inv = 0.71;
	float x,y;
	int x1,y1,x2,y2,sum;
	//int b1,b2,b3,b4;
	
    // Displaying the image
	cvNamedWindow("Stripe",0);
	cvShowImage("Stripe",img);
	
	// Rotation by 45 degrees
	for(i =0; i<68; i++)
		for(j =0; j<68;j++)
		{
			x = (float)root2inv*(j-i)+23;
			y = (float)root2inv*(i+j)-24;
			if(x<0||x>47||y<0||y>47)
			{
				dataJ[i*stepJ+j*channelsJ] =0;
			}
			else
			{
				x1 = (int)(x-1);
				y1 = (int)(y-1);
				x2 = (int)(x+1);
				y2 = (int)(y+1);

				// giving same weight to all the pixels, pixel values will be in grayscale
				sum = data[x1*step+y1]+data[x1*step+y2]+data[x2*step+y1]+data[x2*step+y2];
				dataJ[i*stepJ+j] = data[(int)x*step+(int)y];		//((float)sum)/4;

			}
		 }
	// Displaying the rotated image
	cvNamedWindow("Rotated Stripe",0);
	cvShowImage("Rotated Stripe",J);

	cvWaitKey(0);
	cvDestroyWindow("Stripe");
	cvDestroyWindow("Rotated Stripe");
	return J;

}

IplImage* roatated_target()
{
	// an image with box in between
	IplImage* box_image = cvCreateImage(cvSize(48,48),IPL_DEPTH_8U,1);
	int height = box_image->height;
	int width = box_image->width;
	int step = box_image->widthStep;
	uchar* data = (uchar*)box_image->imageData;
	for(int i = 0; i < 48; i++)
		for(int j = 0; j < 48; j++)
		{
			if(i> 7 && i < 40 && j > 7 && j < 40)
			{
				data[i*step+j] = 255;
			}
			else
			{
				data[i*step+j] = 0;
			}
		}

	IplImage* J = cvCreateImage(cvSize(68,68),IPL_DEPTH_8U,1);			// Holds rotated
	int heightJ = J->height;
	int widthJ = J->width;
	int stepJ = J->widthStep;
	uchar* dataJ = (uchar*)J->imageData;	
	int i,j;
	double root2inv = 0.71;
	float x,y;
	int x1,y1,x2,y2,sum;
	//int b1,b2,b3,b4;
	
    // Displaying the image
	cvNamedWindow("Stripe");
	cvShowImage("Stripe",box_image);
	
	// Rotation by 45 degrees
	for(i =0; i<68; i++)
		for(j =0; j<68;j++)
		{
			x = (float)root2inv*(j-i)+23;
			y = (float)root2inv*(i+j)-24;
			if(x<0||x>47||y<0||y>47)
			{
				dataJ[i*stepJ+j] =0;
			}
			else
			{
				x1 = (int)(x-1);
				y1 = (int)(y-1);
				x2 = (int)(x+1);
				y2 = (int)(y+1);

				// giving same weight to all the pixels, pixel values will be in grayscale
				sum = data[x1*step+y1]+data[x1*step+y2]+data[x2*step+y1]+data[x2*step+y2];
				dataJ[i*stepJ+j] = ((float)sum)/4;

			}
		 }

	// Displaying the rotated image
	cvNamedWindow("Rotated Stripe",0);
	cvShowImage("Rotated Stripe",J);

	cvWaitKey(0);
	cvDestroyWindow("Stripe");
	cvDestroyWindow("Rotated Stripe");
	return J;
}
#endif