#ifndef BLOBCOLORING_H
#define BLOBCOLORING_H

#include<iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "Threshold.h"
#include <string.h>
#include <string>

using namespace std;

class Blobcoloring
{
public:
	// Constructor
	Blobcoloring();
	// Do all the blobcoloring operations and methods calls
	int** use8ConnectedNeighboring(IplImage *binaryImage);
	// Return the name of the region
	int getNumberOfRegions();
	// Return the blobcoloring image
	IplImage* getBlobcoloringImage();
	// Display the results in text file
	void reportResultsToTextFile();
	// Display the results in the console
	void reportResultsToConsole();
	// release the image
	void releaseImage();

private:
	// matrix used for the regions counting, naming...
	int **myBlobcoloringMatrix;
	// blobcoloring image
	IplImage *myBlobcoloringImage;
	// counter for nuber of regions discovered
	int regionCount;
	// before fixing the regions errors
	int maximumRegionsNumbers;
	// check all 8 neighbors pixels
	void check8Neighbors(int height, int width, IplImage *binaryImage);
	// do the initial computation for the blobcoloring
	void initialBlobcoloring(IplImage *binaryImage);
	// fix the naming and count number of the blobcoloring image
	void fixBlobcoloring(IplImage *binaryImage);
	// store final data
	void populateRegionData();

	// Full data for individual region
	struct RegionsData
	{
		int heightMin;
		int heightMax;
		int widthMin;
		int widthMax;
		int averageHeight;
		int averageWidth;
		int area;
		int regionNumber;
	};

	// List of all regions data
	RegionsData *myRegionsData;

	// Dowing all the writing on the image
	void writeTextOnImage();

};

//constructor
Blobcoloring::Blobcoloring()
{
	//starting with no object detected
	regionCount = 0;
}

// do and call all blobcoloring procedures
int** Blobcoloring::use8ConnectedNeighboring(IplImage *binaryImage)
{
	myBlobcoloringMatrix = new int*[binaryImage->height];

	// initializing all counts to zero
	for(int height = 0; height < binaryImage->height; height++)
	{
		myBlobcoloringMatrix[height] = new int[binaryImage->width];

		for(int width = 0; width < binaryImage->width; width++)
		{
			myBlobcoloringMatrix[height][width] = 0;
		}
	}

	regionCount = 1;

	// detecting all objects
	initialBlobcoloring(binaryImage);

	// merging object, removing object, reaming objects
	fixBlobcoloring(binaryImage);

	return myBlobcoloringMatrix;
}

// DO all check for the 8 neighbors
void Blobcoloring::initialBlobcoloring(IplImage *binaryImage)
{
	for(int height = 1; height < binaryImage->height-1; height++)
	{
		for(int width = 1; width < binaryImage->width-1; width++)
		{
			if(myBlobcoloringMatrix[height][width] != 0)
			{
				check8Neighbors(height, width, binaryImage);
			}
			else if((int)cvGet2D(binaryImage, height, width).val[0] == 255)
			{
				if(myBlobcoloringMatrix[height][width-1] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height][width-1];
				}
				else if(myBlobcoloringMatrix[height-1][width-1] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height-1][width-1];
				}
				else if(myBlobcoloringMatrix[height-1][width] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height-1][width];
				}
				else if(myBlobcoloringMatrix[height+1][width-1] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height+1][width-1];
				}
				else if(myBlobcoloringMatrix[height+1][width] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height+1][width];
				}
				else if(myBlobcoloringMatrix[height+1][width+1] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height+1][width+1];
				}
				else if(myBlobcoloringMatrix[height][width+1] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height][width+1];
				}
				else if(myBlobcoloringMatrix[height-1][width+1] != 0)
				{
					myBlobcoloringMatrix[height][width] = myBlobcoloringMatrix[height-1][width+1];
				}
				else
				{
					myBlobcoloringMatrix[height][width] = regionCount;

					check8Neighbors(height, width, binaryImage);

					regionCount++;
				}
			}
		}
	}

	regionCount--;
}

// Fixing problems with the initials checks.
void Blobcoloring::fixBlobcoloring(IplImage *binaryImage)
{
	int countTemp = 0;

	// merging objects if necessary
	for(int height = binaryImage->height-2; height > 0 ; height--)
	{
		for(int width = binaryImage->width-2; width > 0; width--)
		{
			if(myBlobcoloringMatrix[height][width] != 0)
			{
				check8Neighbors(height, width, binaryImage);
			}
		}
	}

	// finding number of objects afer merging
	for(int count = 1; count <= regionCount; count++)
	{
		bool found = false;

		for(int height = 1; height < binaryImage->height-1; height++)
		{
			for(int width = 1; width < binaryImage->width-1; width++)
			{
				if(myBlobcoloringMatrix[height][width] == count)
				{
					countTemp++;
					height = binaryImage->height+1;
					width = binaryImage->width+1;

					found = true;
				}

			}
		}

		if(found == true)
		{
			found = false;
		}


	}

	myBlobcoloringImage = cvCreateImage(cvSize(binaryImage->width, binaryImage->height), IPL_DEPTH_8U, 3);

	//checking if the object is less than 15
	for(int height = 0; height < binaryImage->height; height++)
	{
		for(int width = 0; width < binaryImage->width; width++)
		{
			if((int)cvGet2D(binaryImage, height, width).val[0] == 255)
			{
				CvScalar value;
				value.val[0] = 255;
				value.val[1] = 255;
				value.val[2] = 255;
				cvSet2D(myBlobcoloringImage, height, width, value);
			}
			else
			{
				cvSet2D(myBlobcoloringImage, height, width, cvGet2D(binaryImage, height, width));
			}
		}
	}

	maximumRegionsNumbers = regionCount;
	regionCount = countTemp;

	populateRegionData();

}

// do extra checking for neighbors
void Blobcoloring::check8Neighbors(int height, int width, IplImage *binaryImage)
{
	if((int)cvGet2D(binaryImage, height, width-1).val[0] == 255)
	{
		myBlobcoloringMatrix[height][width-1] = myBlobcoloringMatrix[height][width];
	}

	if((int)cvGet2D(binaryImage, height-1, width-1).val[0] == 255)
	{
		myBlobcoloringMatrix[height-1][width-1] = myBlobcoloringMatrix[height][width];
	}

	if((int)cvGet2D(binaryImage, height-1, width).val[0] == 255)
	{
		myBlobcoloringMatrix[height-1][width] = myBlobcoloringMatrix[height][width];
	}

	if((int)cvGet2D(binaryImage, height+1, width-1).val[0] == 255)
	{
		myBlobcoloringMatrix[height+1][width-1] = myBlobcoloringMatrix[height][width];
	}

	if((int)cvGet2D(binaryImage, height+1, width).val[0] == 255)
	{
		myBlobcoloringMatrix[height+1][width] = myBlobcoloringMatrix[height][width];
	}

	if((int)cvGet2D(binaryImage, height+1, width+1).val[0] == 255)
	{
		myBlobcoloringMatrix[height+1][width+1] = myBlobcoloringMatrix[height][width];
	}

	if((int)cvGet2D(binaryImage, height, width+1).val[0] == 255)
	{
		myBlobcoloringMatrix[height][width+1] = myBlobcoloringMatrix[height][width];
	}

	if((int)cvGet2D(binaryImage, height-1, width+1).val[0] == 255)
	{
		myBlobcoloringMatrix[height-1][width+1] = myBlobcoloringMatrix[height][width];
	}
}

// return total count number of regions
int Blobcoloring::getNumberOfRegions()
{
	return regionCount;
}

// return the blobcoloring image
IplImage* Blobcoloring::getBlobcoloringImage()
{
	return myBlobcoloringImage;
}

// get the data for each region
void Blobcoloring::populateRegionData()
{
	RegionsData* myTempRegionsData = new RegionsData[maximumRegionsNumbers];

	// assigning regions data
	for(int iterations = 0; iterations < maximumRegionsNumbers; iterations++)
	{
		myTempRegionsData[iterations].area = 0;
		myTempRegionsData[iterations].heightMax = 0;
		myTempRegionsData[iterations].heightMin = 100000;
		myTempRegionsData[iterations].widthMax = 0;
		myTempRegionsData[iterations].widthMin = 100000;
		myTempRegionsData[iterations].averageHeight = 0;
		myTempRegionsData[iterations].averageWidth = 0;
		myTempRegionsData[iterations].regionNumber = 0;

		//assinging new names to regions
		//find min and max for both width and height
		for(int height = 0; height < myBlobcoloringImage->height; height++)
		{
			for(int width = 0; width < myBlobcoloringImage->width; width++)
			{
				if(myBlobcoloringMatrix[height][width] == iterations+1)
				{
					myTempRegionsData[iterations].area += 1;
					myTempRegionsData[iterations].regionNumber = iterations+1;
					if(width < myTempRegionsData[iterations].widthMin)
						myTempRegionsData[iterations].widthMin = width;
					else if(width > myTempRegionsData[iterations].widthMax)
						myTempRegionsData[iterations].widthMax = width;

					if(height < myTempRegionsData[iterations].heightMin)
						myTempRegionsData[iterations].heightMin = height;
					else if(height > myTempRegionsData[iterations].heightMax)
						myTempRegionsData[iterations].heightMax = height;
				}
			}
		}

		//calculate the centroid
		myTempRegionsData[iterations].averageHeight = (myTempRegionsData[iterations].heightMax+myTempRegionsData[iterations].heightMin)/2;
		myTempRegionsData[iterations].averageWidth = (myTempRegionsData[iterations].widthMax + myTempRegionsData[iterations].widthMin)/2;

		//removing small objects from the blobcooring image
		if(myTempRegionsData[iterations].area < 15 & myTempRegionsData[iterations].area > 0)
		{
			for(int height = 0; height < myBlobcoloringImage->height; height++)
			{
				for(int width = 0; width < myBlobcoloringImage->width; width++)
				{
					if(myBlobcoloringMatrix[height][width] == iterations+1)
					{
						CvScalar value;
						value.val[0] = 0;
						value.val[1] = 0;
						value.val[2] = 0;
						cvSet2D(myBlobcoloringImage, height, width, value);

						myTempRegionsData[iterations].area = 0;
						myTempRegionsData[iterations].heightMax = 0;
						myTempRegionsData[iterations].heightMin = 0;
						myTempRegionsData[iterations].widthMax = 0;
						myTempRegionsData[iterations].widthMin = 0;
						myTempRegionsData[iterations].averageHeight = 0;
						myTempRegionsData[iterations].averageWidth = 0;
						myTempRegionsData[iterations].regionNumber = -1;
					}
				}
			}

			regionCount--;
		}
	}

	myRegionsData = new RegionsData[regionCount];

	int count = 1;

	//storing the final data
	for(int iterations = 0; iterations < maximumRegionsNumbers; iterations++)
	{
		if(myTempRegionsData[iterations].regionNumber > 0)
		{
			myRegionsData[count-1].area = myTempRegionsData[iterations].area;
			myRegionsData[count-1].heightMax = myTempRegionsData[iterations].heightMax;
			myRegionsData[count-1].heightMin = myTempRegionsData[iterations].heightMin;
			myRegionsData[count-1].widthMax = myTempRegionsData[iterations].widthMax;
			myRegionsData[count-1].widthMin = myTempRegionsData[iterations].widthMin;
			myRegionsData[count-1].averageHeight = myTempRegionsData[iterations].averageHeight;
			myRegionsData[count-1].averageWidth = myTempRegionsData[iterations].averageWidth;
			myRegionsData[count-1].regionNumber = count;

			count++;
		}
	}

	//printing the pixel beloning data name to text file
	FILE *file;
	fopen_s(&file, "ObjectsPixelRegionRepresntation.txt", "w");

	for(int height = 0; height < myBlobcoloringImage->height; height++)
	{
		for(int width = 0; width < myBlobcoloringImage->width; width++)
		{
			if(myBlobcoloringMatrix[height][width] != 0)
			fprintf(file, "%1.2d, ", myBlobcoloringMatrix[height][width]);
			else
				fprintf(file, "    ");
		}
		fprintf(file, "\n");
	}

	fclose(file);

	writeTextOnImage();
}

// Display information on the image
void Blobcoloring::writeTextOnImage()
{
	for(int iterations = 0; iterations < regionCount; iterations++)
	{
		CvFont font;
		double hScale=1;
		double vScale=1;
		int    lineWidth=1;

		string temp;
        string temp2 = " ";
        string temp3 = ", ";

        stringstream sstm;
        sstm<<temp2<<myRegionsData[iterations].regionNumber<<temp3<<myRegionsData[iterations].area;
        temp = sstm.str();

		cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);
		cvPutText(myBlobcoloringImage, "*", cvPoint(myRegionsData[iterations].averageWidth-8, myRegionsData[iterations].averageHeight+8), &font, cvScalar(0,0,255));
		 hScale=.3;
		vScale=.3;

		cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0, 1,lineWidth);
		cvPutText(myBlobcoloringImage, temp.c_str(), cvPoint(myRegionsData[iterations].averageWidth, myRegionsData[iterations].averageHeight), &font, cvScalar(0,150,0));
	}
}

// Display result to a text file
void Blobcoloring::reportResultsToTextFile()
{
	FILE *file;
	fopen_s(&file, "blobcoloringReport.txt", "w");

	fprintf(file, "COUNT | Region Number | AREA | Centroid (X, Y)\n");

	for(int iterations = 0; iterations < regionCount; iterations++)
	{
		fprintf(file, " %1.3d. |      %1.3d      | %1.4d |  (%1.4d, %1.4d)\n", iterations+1, myRegionsData[iterations].regionNumber, myRegionsData[iterations].area, myRegionsData[iterations].averageWidth, myRegionsData[iterations].averageHeight);
	}

	fclose(file);
}

// display results to consule
void Blobcoloring::reportResultsToConsole()
{
	printf("COUNT | Region Number | AREA | Centroid (X, Y)\n");

	for(int iterations = 0; iterations < regionCount; iterations++)
	{
		printf(" %1.3d. |      %1.3d      | %1.4d |  (%1.4d, %1.4d)\n", iterations+1, myRegionsData[iterations].regionNumber, myRegionsData[iterations].area, myRegionsData[iterations].averageWidth, myRegionsData[iterations].averageHeight);
	}
}

// release image
void Blobcoloring::releaseImage()
{
	cvReleaseImage(&myBlobcoloringImage);
}

#endif