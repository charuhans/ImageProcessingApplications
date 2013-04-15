#include<stdio.h>
#include<cv.h>
#include<cxcore.h>
#include<highgui.h>
#include <iostream>
#include <math.h>
#define PI 3.147

using namespace std;

/****************************************************************************
* Purpose:			To create a histogram of the input image    
* Input:			IplImage* img,IplImage* histImage
* Postcondition: returns IplImage holding the histogram of the input image
*****************************************************************************/ 
IplImage* histPlot(IplImage* img)
{

	uchar* data = (uchar*) img -> imageData;
    int width = img->width;
    int height = img->height;
    int step = img->widthStep;
	int size = 255;							//The number of the bins in the histogram
	float range[]={0,256};					//The range of values for the 0th dimension
	float* ranges[] = {range};				// The range of values for the 1st dimension 
	int i, bin_w;							//indeces
	float max_value = 0, min_value = 0;		//max and min value of the histogram
	int min_idx = 0, max_idx = 0;			//max and min index corresponding to max and min value
		
	//create an image to hold the histogram
	IplImage* histImage = cvCreateImage(cvSize(width,height), 8, 1);
	for(int i = 0; i<histImage->height;i++)
		for(int j = 0; j<histImage->width; j++)
			histImage->imageData[i*histImage->widthStep+j] = 255;


	// histogram structure

	CvHistogram* hist = cvCreateHist(1, &size, CV_HIST_ARRAY, ranges, 1);

	//calculate the histogram and apply to hist
	
	cvCalcHist( &img, hist, 0, NULL );

	//grab the min and max values and their indeces
	
	cvGetMinMaxHistValue( hist, &min_value, &max_value, &min_idx, &max_idx);

	//scale the bin values so that they will fit in the image representation
	
	cvScale( hist->bins, hist->bins, ((double)histImage->height)/max_value, 0 );

	bin_w = cvRound((double)histImage->width/size);


	for( i = 0; i < size; i++ ) 
	{
		cvRectangle( histImage, cvPoint(i*bin_w, histImage->height),cvPoint((i+1)*bin_w,histImage->height - cvRound(cvGetReal1D(hist->bins,i))),cvScalarAll(0), -1, 8, 0 );
	}
	
	return histImage;
}

int* histTri()
{
	int* A;
	A = new int[256];
	for (int i = 0; i < 256; i++)
	{
		A[i] = 0;
	}


	for (int i = 0; i < 128; i++)
	{
		A[128 - i] = A[128 + i] = (128 - i);
	}
	return A;
}
int* histGauss(double mean, double sigma)
{
	double denominator = 1/pow(2*PI,0.5);
	int* gauss;
	double exponent;
	gauss = new int[256];
	for(int i = 0; i < 256; i++)
	{   
		exponent = ((double)(i-mean)*(i-mean))/(double)(2*sigma*sigma);				//calculating the exponent		
		gauss[i] = (int)((exp(-1*exponent)/(sigma*denominator))*256*256);		//Creating a gaussian distribution function
	}	
return gauss;
}


	
double * compPdf(int *arr, int height, int width)
{
	double* pdf;
	pdf = new double[256];
	for(int i = 0; i<256; i++)
		pdf[i] = 0.0;

	for (int i=0; i < 256; i++)
	{
		pdf[i] = (double)((double)arr[i]/(double)(height*width));
	}
	return pdf;
}

double * compPdf_h(int *arr)
{
	double* pdf;
	pdf = new double[256];
	for(int i = 0; i<256; i++)
		pdf[i] = 0.0;


	int total = 0;
	for(int i = 0; i<256; i++)
		total = total + arr[i];

	for (int i=0; i < 256; i++)
	{
		pdf[i] = (double)((double)arr[i]/double(total));
	}

	return pdf;
}
double * compCdf(double *arr)
	{
		double sum = 0.0;	
		double* cdf;
		cdf = new double[256];
		for(int i = 0; i<256; i++)
			cdf[i] = 0.0;

		cdf[0] = arr[0];
		for (int i=1; i < 256; i++)

		{
			cdf[i] = cdf[i-1] + arr[i];
		}


		return cdf;
	}

int* matArr(IplImage* img)
{
  int height = img -> height;  
  int width  = img -> width;   
  
int* temp;
temp = new int[256];  // temp stores the number of occorance of each grey scale value
for(int i=0; i< 256; i++ )
	temp[i]=0;

for(int i=0; i< height; i++)
{
	for(int j=0; j< width; j++)
	{
		int x = (uchar)img->imageData[i*img->widthStep+j];
		temp[x]+=1;
	}
}
return temp;
}


IplImage* histShape(IplImage* img,int* hist)
{
    uchar* data = (uchar*) img -> imageData;
    int width = img->width;
    int height = img->height;
    int step = img->widthStep;
	IplImage* shapeImg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);


int * arrimg;
double *icdf, *ipdf, *hcdf, *hpdf;
hcdf = new double[256];
hpdf = new double[256];

for(int i1 = 0; i1 < height; i1++)
	for(int j1 = 0; j1 < width; j1++)
			shapeImg->imageData[i1*shapeImg->widthStep+j1] = 0.0;


arrimg = matArr(img);
ipdf = compPdf(arrimg, height, width);
icdf = compCdf(ipdf);
double** icdfmat;
icdfmat= new double*[height];

for(int i = 0; i<height; i++)
	icdfmat[i] = new double[width];

hpdf = compPdf_h(hist);
hcdf = compCdf(hpdf);


for(int i1 = 0; i1 < height; i1++)
	for(int j1 = 0; j1 < width; j1++)
	{
		int value = (uchar)img->imageData[i1*img->widthStep+j1];
		for(int i=0; i<256; i++)
		{
			if(icdf[value] <= hcdf[i])
			{
				shapeImg->imageData[i1*shapeImg->widthStep+j1] = i;    // storing the values in IplImage
				break;
			}
		}
	}

return shapeImg;
}


int main()
{
	//IplImage *img1 = cvLoadImage("Cameraman.tif",1);						// loading images
	//IplImage *img1 = cvLoadImage("Books.tif",1);
	IplImage *img = cvLoadImage("Boats.tif",0);
	int height = img->height;
	int width = img->width;
	IplImage* histImage = cvCreateImage(cvSize(256,256),8,1);	
	IplImage* SphistImage = cvCreateImage(cvSize(256,256),8,1);
	int choice;
	double sigma;
	int* A;
	A = new int[256];
	IplImage* shapeImg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);

	cout<<"Enter the type of Histogram:"<<endl<< "1.Tringular"<<endl<<"2. Gaussian"<<endl;
	cout<<"Enter your choice (1/2) : ";
	cin>>choice;
	if(choice == 1)
	{	
		A = histTri();
		shapeImg = histShape(img,A);
	}
	else
	{
		cout<<endl<<"Enter the value of standard deviation : ";
		cin>>sigma;
		double mean = 0.0;
		A = histGauss(mean, sigma);
		shapeImg = histShape(img,A);
	}

	IplImage* temp = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	histImage = histPlot(img);
	SphistImage = histPlot(shapeImg);
	
	cvNamedWindow("Gray Scale Image",CV_WINDOW_AUTOSIZE);
	cvShowImage("Gray Scale Image",img);
	cvNamedWindow("Original Image Histogram",CV_WINDOW_AUTOSIZE);
	cvShowImage("Original Image Histogram",histImage);
	cvNamedWindow("Shaped Image",CV_WINDOW_AUTOSIZE);						
	cvShowImage("Shaped Image",shapeImg);
	cvNamedWindow("Shaped Image Histogram",CV_WINDOW_AUTOSIZE);
	cvShowImage("Shaped Image Histogram",SphistImage);


	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Gray Scale Image");
	cvReleaseImage(&histImage);
	cvDestroyWindow("Original Image Histogram");
	cvReleaseImage(&shapeImg);
	cvDestroyWindow("Shaped Image");
	cvReleaseImage(&SphistImage);
	cvDestroyWindow("Shaped Image Histogram");
	
	return 0;
	
}

	

