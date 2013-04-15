#ifndef THRESHOLD_H
#define THRESHOLD_H

#include<iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace std;

class Threshold
{
public:
	//constructors
	Threshold();
	Threshold(int threshold);

	//allow the user to set threshold;
	void setThreshold(int threshold);
	//get threshold
	int getThreshold();

	//compute threshold
	int computeThreshold(IplImage *image);
	int computeThreshold(IplImage *image, int threshold);

private:
	int myThreshold; //thershhold
	int numberOfGreyLevelOccurance[256]; // histogram values
	void reportHistogram(); // print all histogram values in a text file to be used fro the report

	//used to calculate the threshold
	struct Addition
	{
		int sum;
		int index;
		int average;
	};
};

//default constructor setting the defualt value of the threshold to 128
Threshold::Threshold()
{
	myThreshold = 128;
}

//second constructor that allows the user to change the default value of the threshold to new value to be used for the computaitons.
Threshold::Threshold(int threshold)
{
	myThreshold = threshold;
}

// allow the user to overide the threshold value to his/her desire
void Threshold::setThreshold(int threshold)
{
	myThreshold = threshold;
}

// get threshold value
int Threshold::getThreshold()
{
	return myThreshold;
}

// compute threshold value based on the default threshold
int Threshold::computeThreshold(IplImage *image)
{
	return computeThreshold(image, myThreshold);
}

// compute threshold value based on the user's choice of the default value of threshold
int Threshold::computeThreshold(IplImage *image, int threshold)
{
	myThreshold = threshold;

	for(int greyLevel = 0; greyLevel < 256; greyLevel++)
	{
		numberOfGreyLevelOccurance[greyLevel] = 0;
	}

	for(int height = 0; height < image->height; height++)
	{
		for(int width = 0; width < image->width; width++)
		{
			numberOfGreyLevelOccurance[(int)cvGet2D(image, height, width).val[0]] += 1;
		}
	}

	int newTempThreshold = 0;
	bool thresholdMatch = true;

	do
	{
		if(thresholdMatch == false)
		{
			myThreshold = newTempThreshold;
			thresholdMatch = true;
		}

		Addition part1;
		Addition part2;

		part1.sum = 0;
		part1.index = 0;
		part1.average = 0;

		part2.sum = 0;
		part2.index = 0;
		part2.average = 0;

		for(int greyLevel = 0; greyLevel < 256; greyLevel++)
		{
			if(greyLevel > myThreshold)
			{
				part1.sum += greyLevel * numberOfGreyLevelOccurance[greyLevel];
				part1.index += numberOfGreyLevelOccurance[greyLevel];
			}
			else
			{
				part2.sum += greyLevel * numberOfGreyLevelOccurance[greyLevel];
				part2.index += numberOfGreyLevelOccurance[greyLevel];
			}
		}

		part1.average = part1.sum/part1.index;
		part2.average = part2.sum/part2.index;

		newTempThreshold = (part1.average + part2.average)/2;

		if(myThreshold != newTempThreshold)
		{
			thresholdMatch = false;
		}

	}while(thresholdMatch == false);

	reportHistogram();

	return myThreshold;
}

// report histogram values to text file
void Threshold::reportHistogram()
{
	FILE *file;
	fopen_s(&file, "thresholdHistogramData.txt", "w");

	for(int iterations = 0; iterations < 255; iterations++)
	{
		fprintf(file, " %d, %d\n", iterations, numberOfGreyLevelOccurance[iterations]);
	}

	fclose(file);
}
#endif