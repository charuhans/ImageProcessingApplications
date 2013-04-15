#ifndef CARTTOPOL_H
#define CARTTOPOL_H


#include<iostream>
#include<cv.h>
#include<highgui.h>

#define PI 22/7;

using namespace std;

IplImage* cart_to_pol(IplImage* img, int radius, int x, int y, int theta)
{

	int height = img->height;
	int width = img->width;

	int centerX = x;
	int centerY = y;
	double step = 64.0/360.0;
	double angle;

	//int radius = 128;
	//int theta = 64;

	IplImage* polar = cvCreateImage(cvSize(theta,radius),IPL_DEPTH_8U,1);

	double **no_of_times;
	double** polar_matrix;

	no_of_times = new double*[radius];
	polar_matrix = new double*[radius];
	for(int i = 0; i < radius; i++)
	{
		polar_matrix[i] = new double[theta];
		no_of_times[i] = new double[theta];
	}


	for(int i = 0; i < radius; i++)
		for(int j = 0; j < theta; j++)
		{
			polar->imageData[i*polar->widthStep+j] = 0;
			no_of_times[i][j] = 0.0;
			polar_matrix[i][j] = 0.0;
		}



	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
		{
			int rho = (int)(sqrt(pow((double)(i-centerX),2) + pow((double)(j-centerY),2))+0.5);
			if(rho < radius)
			{
                    angle = atan2((double)(j-centerY), (double)(i-centerX));

					if(angle < 0)
							angle = angle + 2*PI;

					double temp = 2*PI;
					temp = temp/(double)theta;
					angle = floor(angle/temp)+1;
					no_of_times[rho][(int)angle] = no_of_times[rho][(int)angle] + 1;
					polar_matrix[rho][(int)angle] =  (double)img->imageData[i*img->widthStep+j] + polar_matrix[rho][(int)angle] ;
			}
		}

	for(int i = 0; i < radius; i++)
		for(int j = 0; j < theta; j++)
		{
			if(no_of_times[i][j] != 0)
				polar_matrix[i][j] = polar_matrix[i][j]/no_of_times[i][j];

			polar->imageData[i*polar->widthStep+j] = (uchar)((int)(polar_matrix[i][j]));
		}

		IplImage* LOG_POLAR_Image = cvCreateImage(cvSize(2*radius,2*radius),IPL_DEPTH_8U,1);

		for(int i = 0; i < 2*radius; i++)
			for(int j = 0; j < 2*radius; j++)
				LOG_POLAR_Image->imageData[i*LOG_POLAR_Image->widthStep+j] = 0;

		for(int i = 0; i < 2*radius; i++)
			for(int j = 0; j < 2*radius; j++)
			{
				int rho = (int)(sqrt((double)((i-radius)*(i-radius) + (j-radius)*(j-radius))) + 0.5);
				if (rho < radius)
				{
					angle = atan2((double)(j-radius), (double)(i-radius));
				
					if (angle < 0) 
						angle = angle + 2*PI;

					double tempo = (double)(1/32.0)*PI;
					angle = floor(angle/tempo)+ 1;
					LOG_POLAR_Image->imageData[i*LOG_POLAR_Image->widthStep+j] = polar->imageData[rho*polar->widthStep+(int)angle];
				}	
			}

		cvNamedWindow("Polar Image",0);
		cvShowImage("Polar Image",LOG_POLAR_Image);

	delete no_of_times;
	delete polar_matrix;
	return polar;

}



#endif