
#include<iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "Functions.h"
#include "Part3.h"
#include "Part4.h"

using namespace std;

int main()
{
  // load 8-bit, 1 channel grayscale TIF image
  IplImage* image = cvLoadImage("Array-9.tif", 0);

  /**************************************************************************BINARY IMAGE***************************************/
  // function returns the threshold for binarizing the image
  int thr = compute_threshold(image);
  cout<<"####################################################"<<endl<<endl;;
  cout<<" computed threshold value is : "<<thr<<endl<<endl;
  cout<<"####################################################"<<endl<<endl;
  // function returns the binarized image using the threshold passed as parameter
  IplImage* img = binarize(image,thr);
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;



  /********************************************************************************GRID**********************************************/
IplImage* image1 = cvLoadImage("Array-9.tif", 1);
int *cols;
int *rows;
rows = new int[height];
cols = new int[width];

for(int i=0; i < width; i++)
{
	for(int j=0; j < height; j++)
    {
		cols[i] = 0;								// initialize array of columns to 0
		rows[j] = 0;								// initialize array of rows to 0
    }
}




int r1,r2,c1,c2,tRows,tCols;


 // calculating number of white pixels in image row wise and column wise
for(int i = 0; i < width; i++)
{
	for(int j = 0; j < height; j++)
    {
		if(data[i * step + j] == 255)
		{
			cols[i] = cols[i] + 1;
			rows[j] = rows[j] + 1;
		}
	}
}

// ignoring rows and columns with very few white pixels
for(int i = 0; i < width; i++)
{
	 if(cols[i] < 18)
	 {
		cols[i] = 0;
	 }
}

for(int i = 0; i < height; i++)
{
	 if(rows[i] < 18)
	 {
		rows[i] = 0;
	 }
}
// to store the first and last coordinate of image
for(int i = 1; i < width; i++)
{
	if(cols[i] != 0)
	{
		c1 = i;
		break;
	}
}
for(int i = 1; i < width; i++)
{
	if(cols[width - i] != 0)
	{
		c2 = width - i;
		break;
	}
}

for(int i = 1; i < height; i++)
{
	if(rows[i] != 0)
	{
		r1 = i;
		break;
	}
}
for(int i = 1; i < height; i++)
{
	if(rows[height - i] != 0)
	{
		r2 = height - i ;
		break;
	}
}
// drawing the line using first  and last coordinates 
cvLine(image1, cvPoint(r1,c1), cvPoint(r1,c2), cvScalar(0,255,0), 1);
cvLine(image1, cvPoint(r1,c1), cvPoint(r2,c1), cvScalar(0,255,0), 1);
cvLine(image1, cvPoint(r2,c1), cvPoint(r2,c2), cvScalar(0,255,0), 1);
cvLine(image1, cvPoint(r1,c2), cvPoint(r2,c2), cvScalar(0,255,0), 1);
tCols = 1;
for(int i = r1; i < r2; i++)
{
	
	if(rows[i] == 0)  //if the value of row is zero, that mean there is no circle in it
	{
		do
		{
			i = i + 1;
		}while(rows[i] == 0);
		tCols = tCols + 1;
	cvLine(image1, cvPoint(i-1,c1), cvPoint(i-1,c2), cvScalar(0,255,0), 1);
	}	
}
tRows = 1;
for(int i = c1; i < c2; i++)
{
	
	if(cols[i] == 0)
	{
		do
		{
			i = i + 1;

		}while(cols[i] == 0);
		tRows = tRows + 1;
	cvLine(image1, cvPoint(r1,i-1), cvPoint(r2,i-1), cvScalar(0,255,0), 1);
	}	
}
cout<<"Size Of the Grid"<<endl;
cout<<"Total nmuber of rows : " <<tRows<<endl;
cout<<"Total nmuber of columns : " <<tCols<<endl<<endl;
cout<<"####################################################"<<endl<<endl;



/**********************************************************MORPHOLGICAL OPERATION***************************/
int choice;
IplImage* final;
  cout<<" 1. open-close operation for 3x3 window"<<endl;
  cout<<" 2. close-open operation for 3x3 window"<<endl;
  cout<<" 3. open-close operation for 5x5 window"<<endl;
  cout<<" 4. close-open operation for 5x5 window"<<endl<<endl;
  cout<<"please choose one of the above options (1-4):";
  cin>>choice;
  cout<<endl<<endl<<"####################################################"<<endl<<endl;

  switch(choice)
  {
	case 1:
		final = open_close3(img);
	break;

	case 2:
		final = close_open3(img);
	break;

	case 3:
		final = open_close5(img);
	break;

	case 4:
		final = close_open5(img);
	break;

	default:
		cout<<endl<<" jason dude this is a wrong choice"<<endl;
		system("pause");
		exit(0);
  };
/************************************************************BLOB********************************/

  IplImage* colorimg;
  int** blob ;
  int choice1;
  cout<<endl<<"1. 4-connected window "<<endl<<"2. 8-connected window"<<endl<<"Enter your choice 1/2"<<endl<<endl;;
  

  cin>>choice1;
  cout<<endl<<endl<<"####################################################"<<endl<<endl;
  if(choice1 == 1)
  {
	blob =  connected_4(final);
  }
  else if(choice1 == 2)
  {
	blob =  connected_8(final);
  }
  else
  {
	  cout<<" Wrong choice "<<endl;
	  system("pause");
	  exit(0);
  }

  colorimg = connected_regions(final, blob); 

/*********************************************************CONTOUR******************************/
  IplImage* contImg;
  img = padding3(img);
  IplImage* tmpimg = dilation3(img);

tmpimg = erosion3(tmpimg);
contImg =  countor(tmpimg);
contImg  = unpad3(contImg);

/*********************************************************DISPLAY****************************/

  cvNamedWindow("InputImage", CV_WINDOW_AUTOSIZE);
  cvShowImage("InputImage", image);
  
  cvNamedWindow("BinaryImage", CV_WINDOW_AUTOSIZE);
  cvShowImage("BinaryImage", img);
 

  cvNamedWindow("GridImage");
  cvShowImage("GridImage",image1);

  cvNamedWindow("final image", CV_WINDOW_AUTOSIZE);
  cvShowImage("final image", final);

  cvNamedWindow("Blob Colored Image", CV_WINDOW_AUTOSIZE);
  cvShowImage("Blob Colored Image",colorimg);

 cvNamedWindow("contour image", CV_WINDOW_AUTOSIZE);
 cvShowImage("contour image", contImg);


  cvWaitKey(0);


  cvDestroyWindow("InputImage");
  cvDestroyWindow("BinaryImage");
  cvDestroyWindow("GridImage");
  cvDestroyWindow("final image");
  cvDestroyWindow("Blob Colored Image");
  cvDestroyWindow("contour image");

  cvReleaseImage(&colorimg);
  cvReleaseImage(&contImg);
  cvReleaseImage(&image);
  cvReleaseImage(&final);
  cvReleaseImage(&img);
  cvReleaseImage(&image1);



  return 0;
}