#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "Threshold.h"
#include "Binary.h"
#include "Blobcoloring.h"

using namespace std;

int main()
{
	

	// Determine if the user wants to exit the program.
	bool Exit = false;

	do
	{

//***************** Question 1

		//Defining the original image
		IplImage* myOriginalImage;

		// to read the name of the image if chosen to load different image.
		char imageName[100];

		// Displaying the main menu
		cout<<"Please choose carefully from the menue below by typing the corresponding number:\n";
		cout<<"(Example: Enter \"3\" to Exit the programe)\n";
		cout<<" 1. Load the default image, \"cells.png\"\n";
		cout<<" 2. Load New Different image.\n";
		cout<<" 3. Exit\n\n";
		cout<<" Your choice: ";

		// Reading input from the reader.
		int input;
		cin>>input;

		cout<<"\n\n";

		if(input == 1)
		{
			// Loading the image.
			myOriginalImage = cvLoadImage("cells.png", 0);

			cout<<" \"cells.png\" was succesfully loaded to the program :)"<<endl;
		}
		else if(input == 2)
		{
			cout<<" Please enter the name of the image that you want to load:\n";
			cout<<" (Example: \"cells.png\".\n\n";
			cout<<" Your input: ";

			// to read the name of the image if chosen to load different image.
			char imageName[100];
			cin>>imageName;

			cout<<"\n\n";

			// Loading the image.
			myOriginalImage = cvLoadImage(imageName, 0);

			cout<<" \""<<imageName<<"\" was succesfully loaded to the program :)"<<endl<<endl;
		}
		else if(input == 3)
		{
			Exit = true;
			
			cout<<"I'm sad that you want to leave my program :(\n";
		}
		else
		{
			cout<<"Invalid input: Default image, cells.png, will be loaded\n";

			// Loading the image.
			myOriginalImage = cvLoadImage("cells.png", 0);

			cout<<" \"cells.png\" was succesfully loaded to the program :)"<<endl<<endl;
		}

		if(input != 3)
		{
			// Defining threshold with default constructor - Default value = 128
			Threshold myThreshold;

			// Compute threshold
			myThreshold.computeThreshold(myOriginalImage);

			// Display threshold
			cout<<"\n*************************************************"<<endl;
			cout<<"* Calculated Threshold = "<< myThreshold.getThreshold()<<endl;
			cout<<"*************************************************"<<endl<<endl;

			cout<<" Do you want to enter different Threshold vlaue?\n";
			cout<<" (Auto calculated threshold is \""<<myThreshold.getThreshold()<<"\")\n\n";
			cout<<"    1. Yes, set different threshold value.\n";
			cout<<"    2. No, use auto calculated threshold.\n\n";

			cout<<" Your input: ";

			int input;
			cin>>input;

			cout<<"\n";

			if(input == 1)
			{
				cout<<"Please enter the desired threshold below:\n\n";

				cout<<"Your input: ";

				int thresholdInput;
				cin>>thresholdInput;

				cout<<endl;
				myThreshold.setThreshold(thresholdInput);

				// Display threshold
				cout<<"*************************************************"<<endl;
				cout<<"* NEW Threshold = "<< myThreshold.getThreshold()<<endl;
				cout<<"*************************************************"<<endl<<endl;
			}
			else if(input == 2)
			{
				cout<<"Threshold value was not changed :)\n\n";
			}
			else
			{
				cout<<"*** Invalid input, threshold value was not changed.\n\n";
			}

			// Defining binaryImage
			Binary myBinaryImage;

			// Binarize the original image with computed threshold. If we just passed the
			// image and not threshold, the binarize function will compute the threshold.
			// example myBinaryImage.binarzise(myOriginalImage) will result the same result
			// because inside the function, I call ..ComputeThreshold(image)
			myBinaryImage.binarize(myOriginalImage, myThreshold.getThreshold());

		//***************** END Question 1

		//***************** Question 2
			// Defining the blobcoloring with default regin count to be zero
			Blobcoloring myBlobcoloring;

			// Call the 8-connected neighbor blobcoloring method that finds how many objects less than 15
			// and find each object's centroid and display the binary image with the new data.
			myBlobcoloring.use8ConnectedNeighboring(myBinaryImage.getBinaryImage());

			// Get the number of found regions in the image that is greater than 15 and display it.
			cout<<"*************************************************"<<endl;
			cout<<"* Number of regions detected in the image = "<<myBlobcoloring.getNumberOfRegions()<<endl;
			cout<<"* (Only regions greater than 15 were detected)"<<endl;
			cout<<"*************************************************"<<endl<<endl;

			// Printing blobcoloring to a text file.
			myBlobcoloring.reportResultsToTextFile();

			cout<<"Report of the blobcoloring operations were exported in a text file, blobcoloringReport.text.\n\n";
			cout<<"   Do you want to also display the results here in the console?\n\n";
			cout<<"      1. Yes, I want to also display the report in the CMD.\n";
			cout<<"      2. No, I will look at the text file result, blobcoloringReport.txt.\n\n";

			cout<<"      Your input: ";

			int inputChoice;
			cin>>inputChoice;

			cout<<endl;

			if(inputChoice == 1)
			{
				myBlobcoloring.reportResultsToConsole();
				cout<<"\n*** END of REPORT\n\n";
			}
			else if(inputChoice == 2)
			{
				cout<<"Displaying blobcoloring report in the console was skipped.\n\n";
			}
			else
			{
				cout<<"*** Invalid input, displaying blobcoloring report in the console was skipped.\n\n";
			}

			cout<<"*** Please close ALL open images if you wish to load a new image to work with :)\n";

			// Reporting the blobcolring data: are, centroid locatins, and region #.
			//***************** Display Images
			// Create Windows
			cvNamedWindow("Original Image", CV_WINDOW_AUTOSIZE);
			cvNamedWindow("Binary Image", CV_WINDOW_AUTOSIZE);
			cvNamedWindow("Blobcoloring Image", CV_WINDOW_AUTOSIZE);

			// Show Images
			cvShowImage("Original Image", myOriginalImage);
			cvShowImage("Binary Image", myBinaryImage.getBinaryImage());
			cvShowImage("Blobcoloring Image", myBlobcoloring.getBlobcoloringImage());

			// Keep windows open
			cvWaitKey(0);

			// Destroy all windows
			cvDestroyWindow("Original Image");
			cvDestroyWindow("Binary Image");
			cvDestroyWindow("Blobcoloring Image");

			// Release all images
			cvReleaseImage(&myOriginalImage);
			myBinaryImage.releaseImage();
			myBlobcoloring.releaseImage();

			cout<<"Do you want to load another image?\n\n";
			cout<<"   1. Yes, I want to load another image.\n";
			cout<<"   2. No, I want to exit the program.\n\n";

			cout<<"   Your input: ";

			cin>>inputChoice;

			if(inputChoice == 1)
			{
				Exit = false;
			}
			else if(inputChoice == 2)
			{
				Exit = true;
				cout<<"I'm sad that you want to leave my program :(\n";
			}
			else
			{
				cout<<"*** Invalid input, program will exit.\n\n";
			}
		}

	}while(Exit == false);
	system("pause");

	return 0;
}