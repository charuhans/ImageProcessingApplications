#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace std;



int compute_threshold(IplImage* image)
{
  uchar *data_or = (uchar*)image -> imageData;
  int height_or = image -> height;  
  int width_or  = image -> width;  
  int step_or   = image -> widthStep;  

// create a new image for storing a binary image


  
int temp[256];  // temp stores the number of occorance of each grey scale value
for(int i=0; i< 256; i++ )
{
	temp[i]=0;
}

for(int i=0; i< height_or; i++)
{
	for(int j=0; j< width_or; j++)
	{
		int x = data_or[i * step_or + j];
		temp[x]+=1;
	}
}

int int_thr = 128;  // Assumed thresholed value
int new_thr;       //  Calculated thresholed value
int flag = 0;

do
{
	if (flag == 1)
	{
		int_thr = new_thr;
		flag = 0;
	}

	int sum1=0;
	int sum2=0;
	int sum1_index=0;
	int sum2_index=0;
	int avg1 = 0;
	int avg2 = 0;
	new_thr = 0;
	for(int i=0; i < 256;i++)
		{
			if(i < int_thr)
			{
				sum1 = sum1 + (i*temp[i]);
				sum1_index = sum1_index + temp[i];
			}
			else
			{
				sum2 = sum2 + (i*temp[i]);
				sum2_index = sum2_index + temp[i];
			}
		}
	avg1 = sum1/sum1_index;
	avg2 = sum2/sum2_index;
	new_thr = (avg1 + avg2)/2;
	if(new_thr != int_thr)
		{
			flag = 1;
		}
	
}while(new_thr != int_thr );
            
return int_thr;

}

//================
//================
IplImage* binarize(IplImage* image, int thr)
{
  IplImage* img = cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
	for(int i = 0; i < height; i++)
	{
	for(int j = 0; j < width; j++)
	{
		if(image->imageData[i * image->widthStep + j] < thr)
		{
			data[i * step + j] = 0;
		}
		else
		{
			data[i * step + j] = 255;
		}
	}
}

	return img;
}

#endif



