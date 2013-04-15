#ifndef DFT_H
#define DFT_H

# include <stdlib.h>
# include <stdio.h>
# include <fftw3.h>
# include <cv.h>
# include <highgui.h>
# include <math.h>

void mask(IplImage* image)
{
	int height = image->height;
	int width =  image->width;
    unsigned int i,j,k,n,k1;
	
	IplImage* IFFT_image    = cvCreateImage(cvSize(2*width,2*height),8,1);
	IplImage* Final_image   = cvCreateImage(cvSize(width,height),8,1);
	IplImage* stripe_image  = cvCreateImage(cvSize(2*width,2*height),8,1);
	IplImage* stripe1_image = cvCreateImage(cvSize(2*width,2*height),8,1);
	IplImage* stripe2_image = cvCreateImage(cvSize(2*width,2*height),8,1);
	IplImage* imageOutput1  = cvCreateImage(cvSize(2*width,2*height),8,1);
	fftwf_complex *input0;
	fftwf_complex *output0;
	fftwf_complex *input;
	fftwf_complex *output;
	fftwf_complex *input1;
	fftwf_complex *output1;
	fftwf_complex *out1;
	fftwf_plan forward2;
	fftwf_plan forward;
	fftwf_plan forward3;
	fftwf_plan backward;

	input0 =(fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*4*height*width);
	output0 =(fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*4*height*width);
	out1 =(fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*4*height*width);
	input1 =(fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*4*height*width);
	output1 =(fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*4*height*width);



	// Create an image of size 136*136 with a box in it which is the target image
	IplImage* testing = roatated_target();  // here we get the target image

	// this is padding the target image
	for ( i = 0; i <2*height; i++ )
		for ( j = 0; j <2*width; j++ )
		{  
			if(i < height && j < width)
				stripe_image->imageData[i*stripe_image->widthStep+j] = testing->imageData[i*testing->widthStep+j];
			else
				stripe_image->imageData[i*stripe_image->widthStep+j] = 0;
		}

	// creating the forward plan 
	forward3 = fftwf_plan_dft_2d(2*height,2*width,input1,output1,FFTW_FORWARD,FFTW_ESTIMATE);
    
	// putting the values of target image in fftwf_complex
	  k=0;
	  for ( i = 0; i <2*height; i++ )
		for ( j = 0; j <2*width; j++ )
		{
			input1[k][0] = (double)((uchar)stripe_image->imageData[i*stripe_image->widthStep+j]);
			input1[k][1] = 0.0;
			k++;
		}

	  // executing the forward plan
	  fftwf_execute(forward3);

	  // putting the magnitude value of padded box image  in frequency domain in an image for display 
		k=0;
		for ( i = 0; i <2*height; i++ )
			for ( j = 0; j <2*width; j++ )
			{   
				stripe1_image->imageData[i*stripe1_image->widthStep+j] =(uchar)(sqrt(output1[k][0]*output1[k][0]+output1[k][1]*output1[k][1]));
				k++;  
			}
	   
	// creating the mask for display
	for ( i = 0; i <2*height; i++ )
		for ( j = 0; j <2*width; j++ )
		{   
			if(stripe1_image->imageData[i*stripe1_image->widthStep+j] > 1)
				stripe1_image->imageData[i*stripe1_image->widthStep+j] =255;
			else
				stripe1_image->imageData[i*stripe1_image->widthStep+j] =0;
		}

		// forward plan for original image
		forward = fftwf_plan_dft_2d(2*height,2*width,input0,output0,FFTW_FORWARD,FFTW_ESTIMATE);
		k=0;
	
		//here i will padd the original image with extra zeros
		IplImage* padded_rotated = cvCreateImage(cvSize(2*width,2*height),IPL_DEPTH_8U,1);
		for ( i = 0; i <2*height; i++ )
			for ( j = 0; j <2*width; j++ )
			{
				if(i < height && j < width)
					padded_rotated->imageData[i*padded_rotated->widthStep+j] = image->imageData[i*image->widthStep+j];
				else
					padded_rotated->imageData[i*padded_rotated->widthStep+j] = 0;
			}

		// putting the value of the image into fftw_complex
		for ( i = 0; i <2*height; i++ )
			for ( j = 0; j <2*width; j++ )
			{
				input0[k][0] = (double)((uchar)padded_rotated->imageData[i*padded_rotated->widthStep+j]);
				input0[k][1] = 0;
				k++;
			}

		// executing forward plan for original image
		fftwf_execute(forward);
		k=0;
		for ( i = 0; i <2*height; i++ )
			for ( j = 0; j <2*width; j++ )
			{   stripe2_image->imageData[i*stripe2_image->widthStep+j] =(uchar)(sqrt(output0[k][0]*output0[k][0]+output0[k][1]*output0[k][1]));
				k++;  
			}
			
	  // plotting generated mask and DFT of original image
	  cvNamedWindow("Generated Mask",0);
	  cvShowImage("Generated Mask",stripe1_image);
	  cvNamedWindow("DFT of original Image",0);
	  cvShowImage("DFT of original Image",stripe2_image);

      // making IplImage of 1's and 0's for masking
	  for ( i = 0; i < 2*height; i++ )
		for ( j = 0; j < 2*width; j++ )
		{  
			k = (int)((uchar)stripe1_image->imageData[i*stripe1_image->widthStep+j]);
			if( k > 1)
				stripe1_image->imageData[i*stripe1_image->widthStep+j] = 1;
			else
				stripe1_image->imageData[i*stripe1_image->widthStep+j] = 0;
		}

	// Creating masked DFT for displaying
	for ( i = 0; i <2*height; i++ )
		for ( j = 0; j <2*width; j++ )
			stripe2_image->imageData[i*stripe2_image->widthStep+j]  = stripe2_image->imageData[i*stripe2_image->widthStep+j]*
																	  stripe1_image->imageData[i*stripe1_image->widthStep+j];
					  
	  //displays the DFT of the original image after masking
	  cvNamedWindow("DFT of original Image after masking",0);
	  cvShowImage("DFT of original Image after masking",stripe2_image);
		
	  // Masking in the frequency domain
	  k = 0;
	  for ( i = 0; i <2*height; i++ )
		for ( j = 0; j <2*width; j++ )
			{      
				if(abs((int)output1[k][0]) + abs((int)output1[k][1]) < 1 ) //output1[k][0] == 0 && output1[k][1] == 0)
					output0[k][0] = output0[k][1] = 0.0;					  
				k++;
			}

	for ( i = 0; i <2*height; i++ )
		for ( j = 0; j <2*width; j++ )
			stripe2_image->imageData[i*stripe2_image->widthStep+j] =0;  
	
    // backward plan for the masked image
	backward = fftwf_plan_dft_2d(2*height,2*width,output0,out1,FFTW_BACKWARD,FFTW_ESTIMATE);

	// executing the backward plan
	fftwf_execute(backward);

	// normalize the values
	for(int i=0; i< 4*height*width; i++)
	{
        out1[i][0] /= double(4*height*width);
		//out1[i][1] /= double(4*height*width);
	}
 
    // copy back to image output
    k=0;
    for(int i=0; i< 2*height;i++)
        for (int j=0; j< 2*width; j++)
		{
			if(out1[k][0] > 1.0 && out1[i][1] > -0.09)
			{
				imageOutput1->imageData[i*imageOutput1->widthStep + j] = (uchar)abs((int)out1[k][0]);
			}
			else
				imageOutput1->imageData[i*imageOutput1->widthStep + j] = 0;
            k++;
		}
		
		
	  cvNamedWindow("IDFT",0);
	  cvShowImage("IDFT",imageOutput1);
	  //cvWaitKey(0);
	  

	  // Putting the values of the image back into the image of original size
	  for(int i=0; i< height;i++)
        for (int j=0; j< width; j++)
			{			
				Final_image->imageData[i*Final_image->widthStep+j] = imageOutput1->imageData[i*imageOutput1->widthStep + j];
			}

	  cvNamedWindow("Final Image",0);
	  cvShowImage("Final Image",Final_image);

	  cvWaitKey(0);
	  cvReleaseImage(&stripe1_image);
	  cvDestroyWindow("Generated Mask");
	  cvDestroyWindow("Target Image");
	  cvDestroyWindow("DFT of Target Image");
	  cvDestroyWindow("IDFT");
	  cvDestroyWindow("DFT of Original Image");
	  cvDestroyWindow("DFT of Original Image after masking");
	  //cvDestroyWindow("Final Image");
	  cvReleaseImage(&stripe2_image);
	  cvReleaseImage(&Final_image);
	  cvReleaseImage(&imageOutput1);
	  cvReleaseImage(&stripe1_image);
	  cvReleaseImage(&stripe_image);
	  fftwf_destroy_plan(forward);
	  fftwf_destroy_plan(forward3);
	  fftw_free(input0); 
	  fftw_free(output0);
	  fftw_free(input1); 
	  fftw_free(output1);
}

/*******************************************************************************************
* Purpose:			To create the toeplitz image    
* Input:			None
* Postcondition:    returns toeplitz image 
********************************************************************************************/
IplImage* Stripe(void)
{
	IplImage* img = cvCreateImage(cvSize(48,48),IPL_DEPTH_8U,1);
	int height = img->height;
	int width = img->width;
	int step = img->widthStep;
	int channels = img->nChannels;
	uchar* data = (uchar*)img->imageData;
	int i,j;
	
	// Creating the Striped Image
	for(i = 0; i<height; i++)
		for(j = 0; j<width;j++)
		{
			if((68-i+j)%8==0) 
				data[i*step+j*channels] =255;
			 else              
				 data[i*step+j*channels] =0;
		}

	// Putting a Box in between
	for(i = 8; i<40; i++) 
		for(j =8; j<40;j++) 
			data[i*step+j*channels] =255;

	return img;
}


/*******************************************************************************************
* Purpose:			To compute the DFT then plot the magnitude and phase plot 
* Input:			IplImage* img
* Postcondition:    Displays magnitude and phase plot in the frequency domain
********************************************************************************************/
void dft_compute(IplImage* img)
{
	int i,j,k;
	fftwf_complex *in;
	fftwf_complex *out;
	IplImage* in_image = cvCreateImage(cvSize(2*img->width,2*img->height),8,1);
	IplImage* mag_image = cvCreateImage(cvSize(2*img->width,2*img->height),8,1);
	int stepout = mag_image->widthStep;
	uchar* dataout = (uchar*)mag_image->imageData;
	IplImage* phase_image = cvCreateImage(cvSize(2*img->width,2*img->height),8,1);
	int phase_step = phase_image->widthStep;
	uchar* phase_data = (uchar*)phase_image->imageData;
	int height = img->height;
	int width = img->width;
	int step = img->widthStep;
	int channels = img->nChannels;
	int temp1;
	float min,max,max_phase,min_phase;
	uchar* data = (uchar*)img->imageData;
	float *M;
	M = new float[4*img->width*img->height];
	double *Phase;
	Phase = new double[4*img->width*img->height];
    fftwf_plan forward;

	cvNamedWindow("Original Image",0);
    cvShowImage("Original Image",img);
    
	// creating the padded image 
	  for ( i = 0; i < 2*img->height; i++ )
		  for ( j = 0; j < 2*img->width; j++ )
		  {   
			  if(i < img->height && j < img->width)
				{
					in_image->imageData[i*in_image->widthStep+j] = img->imageData[i*step+j];
			    }
			  else
				{
					in_image->imageData[i*in_image->widthStep+j] = 0;
			    }
	    }
	// displays the padded image
	cvNamedWindow("Padded Image",0);
	cvShowImage("Padded Image",in_image);

	// malloc 
	in =(fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*4*height*width);
	out =(fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*4*height*width);
	  
	// forward plan for the original padded image
	forward = fftwf_plan_dft_2d(2*img->height,2*img->width,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
	  k=0;
	  for ( i = 0; i < 2*img->height; i++ )
			for ( j = 0; j < 2*img->width; j++ )
				{
					in[k][0] = pow(float(-1),float(i+j))*(in_image->imageData[i*in_image->widthStep+j]);
					in[k][1] = 0;
					k++;
				}

		for ( i = 0; i <4*img->height*img->width; i++ )
			{  
				M[i]=0.0;
			}

	  // executing the forward plan
	  fftwf_execute(forward);

	  k=0;
		for ( i = 0; i < 2*img->height; i++ )
			for ( j = 0; j < 2*img->width; j++ )
			{   // putting magnitude in array
				mag_image->imageData[i*stepout+j] =(uchar)(sqrt(out[k][0]*out[k][0]+out[k][1]*out[k][1]));
				// Putting phase into the array
				Phase[k] = atan((double)(out[k][1]/out[k][0]));
			  k++;  
			}

	  // displaying the centered frequency plot
	  cvNamedWindow("Centred Frequency Plot",0);
	  cvShowImage("Centred Frequency Plot",mag_image);

	// Logarithmic compression and full contrast strech 
	  min =255.0;
	  max = 0.0;
	  min_phase = 255.0;
	  max_phase = 0.0;
	  k = 0;

	// log compression
      for(i=0; i< 2*img->height;i++)
		for (j=0; j< 2*img->width; j++)
			{   temp1 =dataout[i*stepout+j];
				M[k] = (float)log((double)(1+temp1));
				k++;
			}

	// finding max and min
	    k = 0;
		for ( i = 0; i < 2*img->width; i++ ) 
			for ( j = 0; j < 2*img->height; j++ )
				{ 
					if(M[k]<=min) { min = M[k]; }
					if(M[k]>=max) { max = M[k]; }
					if(Phase[k]<=min_phase) { min_phase = (float)Phase[k]; }
					if(Phase[k]>=max_phase) { max_phase = (float)Phase[k]; }
					k=k+1;
				}
	
	// contrast strech done here
		k=0;
		for ( i = 0; i < 2*img->height; i++ )
			for ( j = 0; j < 2*img->width; j++ )
			{ 
				  mag_image->imageData[i*stepout+j] = (int)((255/(max-min))*(M[k]-min));
				  phase_image->imageData[i*phase_step+j] = (int)((255/(max_phase-min_phase))*(Phase[k]-min_phase));
				  k=k+1;
			}

	  cvNamedWindow("After Log Compression and Contrast Strech",0);
	  cvShowImage("After Log Compression and Contrast Strech",mag_image);
	  cvNamedWindow("Phase Plot",0);
	  cvShowImage("Phase Plot",phase_image);
	  cvWaitKey(0);
      cvDestroyWindow("Original Image");
	  cvReleaseImage(&mag_image);
	  cvDestroyWindow("After Log Compression and Contrast Strech");
	  cvDestroyWindow("Centred Frequency Plot");
	  cvReleaseImage(&phase_image);
	  cvDestroyWindow("Phase Plot");
	  cvDestroyWindow("I");
	  cvDestroyWindow("Padded Image");
	  fftwf_destroy_plan(forward);
	  fftw_free(in); 
	  fftw_free(out);

}
#endif