#ifndef POLARTOCORTICAL
#define POLARTOCORTICAL
#include<cv.h>
#include<highgui.h>
#include<iostream>
#include <math.h>

#define pi 22/7;

//IplImage* 
void polar_to_cortical(int** polar,int radius,int x, int y, int theta)
{
	
	int m =  radius;
	int n =  theta;

	double **cortical_matrix, **count;

	cortical_matrix = new double*[m];
	count = new double*[m];
	for(int i = 0; i <= m; i++)
	{
		cortical_matrix[i] = new double[n];
		count[i] = new double[n];
	}

	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
		{
			cortical_matrix[i][j] = 0.0;
			count[i][j]= 0.0;
		}


	IplImage* cortical_image = cvCreateImage(cvSize(2*m+4,2*m+4),IPL_DEPTH_8U,1);
	IplImage* cortical_representation = cvCreateImage(cvSize(n,m),IPL_DEPTH_8U,1);

	for(int i = 0; i < 2*m+4; i++)
	{
		for(int j = 0; j < 2*m+4; j++)
		{
			cortical_image->imageData[i*cortical_image->widthStep+j] = 0;
		}
	}

	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			cortical_representation->imageData[i*cortical_representation->widthStep+j] = 0;
		}
	}


	double *logarray1, *logarray;
	logarray1 = new double[m];
	logarray = new double[m];

	double a = 0.3;

	double stepx =  (log10(m+a) - log10(1+a))/m;

	for(int i = 0; i< m; i++){
		logarray1[i] = 0.0;
		logarray[i] = 0.0;
	}

	for(int i = 0; i < m; i++)
		logarray1[i] = log10(1+a)+ i*stepx;

	for(int i = 0; i< m; i++)
		logarray[i] = pow(10.0,logarray1[i]);

	for (int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)		
		{
			int temp = (int)(logarray[i]-0.5);
			double inter = (double)(polar[temp][j]);
			cortical_matrix[i][j] = cortical_matrix[i][j] + inter;
            count[i][j] = count[i][j] + 1; 
		}

	for (int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			if (count[i][j] != 0)
				cortical_matrix[i][j] = (double)((int)((cortical_matrix[i][j]/count[i][j])+0.5));


	double curvalue = 0.0;

	for (int j = n-1 ; j >= 0; j--)
		for(int i = 0; i < m; i++)
		{
			if(cortical_matrix[i][j] != 0)
			{
				curvalue = cortical_matrix[i][j];
				continue;
			}

			if(cortical_matrix[i][j] == 0);
				cortical_matrix[i][j] = curvalue;
		}



    // till here i have the cortical matrix. i will try to visualize it 

		for(int i = 0; i < m; i++)
		{
			for(int j = 0; j < n; j++)
			{
				cortical_representation->imageData[i*cortical_representation->widthStep+j] = cortical_matrix[i][j];
			}
		}

	cvNamedWindow("Cortical Representation",0);
	cvShowImage("Cortical Representation",cortical_representation);



	double curtheta;
	double rho;
	for(int i = 0; i < 2*m; i++){
		for(int j = 0; j < 2*m; j++)
		{
			rho = sqrt((double)((i-m)*(i-m) + (j-m)*(j-m)));
			if (rho < m)
			{
				curtheta = atan2((double)(j-m), (double)(i-m));
				if (curtheta < 0) 
					curtheta = curtheta + 2*pi;

				double angel = (1.0/32.0)*pi;
				curtheta = 1 + floor(curtheta/angel);
				cortical_image->imageData[i*cortical_image->widthStep+j] = cortical_matrix[(int)rho][(int)curtheta];
			}
		}
	}


	// interpolation

	for(int i = m-m/2; i < m+m/2; i++){
		for(int j = m-m/2; j < m-m/2; j++)
		{
			rho = sqrt((double)((i-x)*(i-x) + (j-y)*(j-y)));
			if (rho < m)
			{
				if(cortical_image->imageData[i*cortical_image->widthStep+j] == 0)
				{
					cortical_image->imageData[i*cortical_image->widthStep+j] = (cortical_image->imageData[(i-1)*cortical_image->widthStep+(j-1)] +
																				cortical_image->imageData[(i-1)*cortical_image->widthStep+(j+1)] +
																			    cortical_image->imageData[(i+1)*cortical_image->widthStep+(j-1)] +
																			    cortical_image->imageData[(i+1)*cortical_image->widthStep+(j+1)])/4 ;
				}
			}
		}
	}	
	cvNamedWindow("cortical Image",0);
	cvShowImage("cortical Image",cortical_image);									  

	//return cortical_image;
}



#endif