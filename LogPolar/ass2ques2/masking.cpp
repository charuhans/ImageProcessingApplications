/***************************************************************************
* Program:                fftwnmask.cpp
* Name:					  Charu Hans
* Course:                 Digital Image Processing
* PeopleSoft ID:          0816182
* Assignment:             Assignment 2 ,Question 4
*****************************************************************************/

# include <stdlib.h>
# include <stdio.h>
# include <fftw3.h>
# include <cv.h>
# include <highgui.h>
# include <math.h>
#include "rotation.h"
#include "dft.h"




int main ()
{ 
  IplImage* img = Stripe();
  int height_original = img->height;
  int width_original = img->width;

  IplImage* image = cvCreateImage(cvSize(2*width_original,2*height_original),8,1);
  IplImage* image1 = cvCreateImage(cvSize(2*width_original,2*width_original),8,1);

  IplImage* rotated_image = rotate(img);

  //int height_rotated = rotated_image->height;
  //int width_rotated = rotated_image->width;

  //IplImage* image  = cvCreateImage(cvSize(2*width_rotated,2*height_rotated),8,1);
  //IplImage* image1 = cvCreateImage(cvSize(2*width_rotated,2*height_rotated),8,1);

  for (int i = 0; i < 2*height_original; i++ )
		for (int j = 0; j < 2*width_original; j++ )
			{   if(i < height_original && j < width_original)
					{
						image->imageData[i*image->widthStep+j] = img->imageData[i*img->widthStep+j];
						image1->imageData[i*image1->widthStep+j] = img->imageData[i*img->widthStep+j];
					}
				else
					{
						image->imageData[i*image->widthStep+j] = 0;
						image1->imageData[i*image1->widthStep+j] = 0;
					}
			}
  dft_compute(rotated_image);

  // testing hain ji
  //IplImage* testing = roatated_target();

// masking and idft comes here
  mask(rotated_image);
  
  cvWaitKey(0);
  //cvDestroyWindow("Final Image");
  return 0;
}






        



