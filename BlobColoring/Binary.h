#ifndef BINARY_H
#define BINARY_H

#include<iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "Threshold.h"

using namespace std;

class Binary
{
public:
	Binary(); // constructor

	IplImage* binarize(IplImage *image); //produce binary image
	IplImage* binarize(IplImage *image, int threshold); //produce binary image
	IplImage *getBinaryImage(); // get binary image
	void releaseImage();

private:
	IplImage *myBinaryImage; // binary image

};

//constructor
Binary::Binary()
{
}


//if no threshold was set by the user, an auto computation of threshold will be done by the Threshold class
IplImage* Binary::binarize(IplImage *image)
{
	Threshold myThreshold;

	return binarize(image, myThreshold.computeThreshold(image));
}

// if the user decides to choose a specific threshold
IplImage* Binary::binarize(IplImage *image, int threshold)
{
	myBinaryImage = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);

	for(int height = 0; height < image->height; height++)
	{
		for(int width = 0; width < image->width; width++)
		{
			CvScalar value;

			if(cvGet2D(image, height, width).val[0] > threshold)
			{
				value.val[0] = 0;
				cvSet2D(myBinaryImage, height, width, value);
			}
			else
			{
				value.val[0] = 255;
				cvSet2D(myBinaryImage, height, width, value);
			}
		}
	}

	return myBinaryImage;
}

// get binary image
IplImage* Binary::getBinaryImage()
{
	return myBinaryImage;
}

// release binary image
void Binary::releaseImage()
{
	cvReleaseImage(&myBinaryImage);
}

#endif