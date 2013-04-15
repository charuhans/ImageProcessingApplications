#include<iostream>
#include<cv.h>
#include<highgui.h>

#define PI 3.14;

using namespace std;


void main()
{
	IplImage* img;
	img = cvLoadImage("Boats.tif",0);
	//img = cvLoadImage("Books.tif",0);
	//img = cvLoadImage("Cameraman.tif",0);

	if(img == NULL)
	{
		cout<<"Unable to load the image"<<endl;
		system("pause");
		exit(0);
	}

	int height = img->height;
	int width = img->width;

	int centerX = height/2;
	int centerY = width/2;
	double step = 64.0/360.0;
	double angle;

	int radius = 128;
	int theta = 64;
	IplImage* LOG_POLAR_Image = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);

	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			LOG_POLAR_Image->imageData[i*LOG_POLAR_Image->widthStep+j] = 0;




	IplImage* polar = cvCreateImage(cvSize(theta,radius),IPL_DEPTH_8U,1);
	double no_of_times[128][64];
	double polar_matrix[128][64];

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
					{
							angle = angle + 2*PI;
					}

					double temp = 2*PI;
					temp = temp/(double)theta;
					angle = floor(angle/temp)+1;
					no_of_times[rho][(int)angle] = no_of_times[rho][(int)angle] + 1;
					polar_matrix[rho][(int)angle] =  (double)img->imageData[i*img->widthStep+j] + polar_matrix[rho][(int)angle] ;
			}
		}

	for(int i = 0; i < 128; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			if(no_of_times[i][j] != 0)
			{
				polar_matrix[i][j] = polar_matrix[i][j]/no_of_times[i][j];
			}
			polar->imageData[i*polar->widthStep+j] = (uchar)(polar_matrix[i][j]);
		}
	}

	for(int i = 0; i < 256; i++)
		for(int j = 0; j < 256; j++)
		{
			int rho = (int)(sqrt((double)((i-128)*(i-128) + (j-128)*(j-128))) + 0.5);
			if (rho < 128)
			{
				angle = atan2((double)(j-128), (double)(i-128));
			
				if (angle < 0) 
				{
					angle = angle + 2*PI;
				}

				double tempo = (double)(1/32.0)*PI;
				angle = floor(angle/tempo)+ 1;
				LOG_POLAR_Image->imageData[i*LOG_POLAR_Image->widthStep+j] = polar->imageData[rho*polar->widthStep+(int)angle];
			}
			
		}

	cvNamedWindow("Original Image",0);
	cvShowImage("Original Image",img);
	cvNamedWindow("Polar Representation Image",0);
	cvShowImage("Polar Representation Image",polar);
	cvNamedWindow("LOG_POLAR Image",0);
	cvShowImage("LOG_POLAR Image",LOG_POLAR_Image);
	cvWaitKey(0);
	cvDestroyWindow("Polar Representation");
	cvDestroyWindow("Original Image");
	cvDestroyWindow("LOG_POLAR Image");
	cvReleaseImage(&img);
	cvReleaseImage(&polar);
	cvReleaseImage(&LOG_POLAR_Image);

}