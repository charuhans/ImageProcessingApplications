#include<iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cmath>
#include "Functions.h"
using namespace std;



int** connected_4(IplImage* img)
{
	uchar* data = (uchar*) img -> imageData;
	int width = img->width;
	int height = img->height;
	int step = img->widthStep;
	int **blob_4;                   // Here a 2D array is created that stores the region numbers
	blob_4 = new int*[height];
	for(int i = 0 ; i < height; i++)
	{
		blob_4[i] = new int[width];
	}
	for(int i = 0 ; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			blob_4[i][j] = 0;
		}
	}

	  // 4 connected neighborhood

	int b4reg = 1;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(i == 0)
			{
				if(j == 0)
				{
				  if(data[i * step + j] = 255)
					{
						blob_4[i][j] = b4reg;
						b4reg = b4reg + 1;
					} 
				}
				else
				{
					if((data[i * step + j] == 255) && (data[i * step +(j-1)] == 0))
					{
						blob_4[i][j] = b4reg;
						b4reg = b4reg + 1;
					}
					if((data[i, j] == 255) && (data[i * step +(j-1)] == 255))
					{
						blob_4[i][j] = blob_4[i][(j-1)];
					}
						
				 }
			}
			else
			{
				if(j == 0)
				{
					if((data[i * step + j] == 255) && (data[(i-1) * step + j] == 0))
					{
						blob_4[i][j] = b4reg;
						b4reg = b4reg + 1;
					}
					if((data[i * step + j] == 255) && (data[(i-1) * step + j] == 255))
					{
						blob_4[i][j] = blob_4[(i-1)][j];
					}
				}
			   else
			   {
					if((data[i * step + j] == 255) && (data[(i-1) * step + j] == 0) && (data[i * step +(j-1)] == 0))
					{
						blob_4[i][j] = b4reg;
						b4reg = b4reg + 1;
					}
					if((data[i * step + j] == 255) && (data[(i-1) * step + j] == 0) && (data[i * step +(j-1)] == 255))
					{
						blob_4[i][j] = blob_4[i][(j-1)];
					}
					if((data[i * step + j] == 255) && (data[(i-1) * step + j] == 255) && (data[i * step +(j-1)] == 0))
					{
						blob_4[i][j] = blob_4[(i-1)][j];
					}
					if((data[i * step +j ] == 255) && (data[(i-1) * step + j] == 255) && (data[i * step +(j-1)] == 255))
					{
						if(blob_4[(i-1)][j] == blob_4[i][(j-1)])
						{
							blob_4[i][j] = blob_4[(i-1)][j];
						}
						else
						{
							int min = blob_4[(i-1)][j];
							if(blob_4[i][(j -1 )] < min )
							{
								min = blob_4[i][(j -1 )];
							}

							blob_4[i][(j -1 )] = min;
							blob_4[(i-1)][j] = min;
							blob_4[i][j] = min;
						}

					}
				}
			}
		}
	}
	return blob_4;
}

//===================================================================================
// 8 connected neighborhood
int** connected_8(IplImage* img)
{
	uchar* data = (uchar*) img -> imageData;
	int width = img->width;
	int height = img->height;
	int step = img->widthStep;
	int **blob_8;
	blob_8 = new int*[height];
	for(int i = 0 ; i < height; i++)
	{
		blob_8[i] = new int[width];
	}
	for(int i = 0 ; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			blob_8[i][j] = 0;
		}
	}

	int b8reg = 1;
	for(int i = 1; i < height; i++)
	{
		for(int j = 1; j < width; j++)
		{
			if(i == 0)
			{
				if(j == 0)
				{
					if(data[i * step + j] = 255)
					{
						blob_8[i][j] = b8reg;
						b8reg = b8reg + 1;
					} 
				}
				else
				{
					if((data[i * step + j] == 255) && (data[i * step +(j-1)] == 0))
					{
						blob_8[i ][ j] = b8reg;
						b8reg = b8reg + 1;
					}
					if((data[i * step + j] == 255) && (data[i * step +(j-1)] == 255))
					{
						blob_8[i ][ j] = blob_8[i ][ (j-1)];
					}
						
				 }
			}
			else
			{
				if(j == 0)
				{
					if((data[i * step + j] == 255) && (data[(i-1) * step + j] == 0))
					{
						blob_8[i ][ j] = b8reg;
						b8reg = b8reg + 1;
					}
					if((data[i * step + j] == 255) && (data[(i-1) * step + j] == 255))
					{
						blob_8[i ][ j] = blob_8[(i-1) ][ j];
					}
				}
			   else
			   {
					if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 0) && (data[i * step +(j-1)] == 0) && (data[(i-1) * step +(j-1)] == 0))
					{
						blob_8[i ][j] = b8reg;
						b8reg = b8reg + 1;
					}
					if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 0) && (data[i * step +(j-1)] == 255) && (data[(i-1) * step +(j-1)] == 0))
					{
						blob_8[i ][ j] = blob_8[i ][ (j-1)];
					}
					if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 255) && (data[i * step +(j-1)] == 0) && (data[(i-1) * step +(j-1)] == 0))
					{
						blob_8[i ][ j] = blob_8[(i-1) ][ j];
					}
							if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 0) && (data[i * step +(j-1)] == 0) && (data[(i-1) * step +(j-1)] == 255))
					{
						blob_8[i ][ j] = blob_8[(i-1) ][ (j-1)];
					}
					if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 255) && (data[i * step +(j-1)] == 255) && (data[(i-1) * step +(j-1)] == 0))
					{
						if(blob_8[(i-1) ][ j] == blob_8[i ][ (j-1)])
						{
							blob_8[i ][ j] = blob_8[(i-1) ][ j];
						}
						else
						{
							int min = blob_8[(i-1) ][ j];
							if(blob_8[i ][ (j -1 )] < min )
							{
								min = blob_8[i ][ (j -1 )];
							}

							blob_8[i ][ (j -1 )] = min;
							blob_8[(i-1) ][ j] = min;
							blob_8[i ][ j] = min;
						}

					}
					if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 0) && (data[i * step +(j-1)] == 255) && (data[(i-1) * step +(j-1)] == 255))
					{
						if(blob_8[(i-1) ][ (j-1)] == blob_8[i ][ (j-1)])
						{
							blob_8[i ][ j] = blob_8[(i-1) ][ (j-1)];
						}
						else
						{
							int min = blob_8[(i-1) ][ (j-1)];
							if(blob_8[i ][ (j -1)] < min )
							{
								min = blob_8[i ][ (j -1 )];
							}

							blob_8[i ][ (j -1 )] = min;
							blob_8[(i-1) ][ (j-1)] = min;
							blob_8[i ][ j] = min;
						}

					}
					if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 255) && (data[i * step +(j-1)] == 0) && (data[(i-1) * step +(j-1)] == 255))
					{
						if(blob_8[(i-1) ][ j] == blob_8[(i-1) ][ (j-1)])
						{
							blob_8[i ][ j] = blob_8[(i-1) ][ j];
						}
						else
						{
							int min = blob_8[(i-1) ][ (j-1)];
							if(blob_8[(i-1) ][ j] < min )
							{
								min = blob_8[(i-1) ][ j];
							}

							blob_8[(i-1) ][ (j -1 )] = min;
							blob_8[(i-1) ][ j] = min;
							blob_8[i ][ j] = min;
						}

					}
					if((data[i * step +j] == 255) && (data[(i-1) * step +j] == 255) && (data[i * step +(j-1)] == 255) && (data[(i-1) * step +(j-1)] == 255))
					{
						if((blob_8[(i-1) ][ j] == blob_8[i ][ (j-1)]) &&(blob_8[i ][ (j-1)] == blob_8[(i-1) ][ (j-1)]))
						{
							blob_8[i ][ j] = blob_8[(i-1) ][ j];
						}
						else
						{
							int min1 = min(blob_8[(i-1) ][ j], blob_8[i ][ (j-1)]);
							int min2 = min(min1, blob_8[(i-1) ][ (j-1)]);

							blob_8[i ][ (j -1 )] = min2;
							blob_8[(i-1) ][ (j -1 )] = min2;
							blob_8[(i-1) ][ j] = min2;
							blob_8[i ][ j] = min2;
						}

					}
				}
			}
		}
	}
	return blob_8;
}
IplImage* connected_regions(IplImage* im, int** blob)
{
	int max = blob[0][0];
	for(int i = 0; i < im->height; i++)
	{
		for(int j = 0; j < im->width; j++)
		{
			if( blob[i][j] >=  max)
			{
				max = blob[i][j];
			}
		}
	}


	int ** A;
	A = new int*[max+1];

	for(int i = 0; i <= max; i++)
	{
		A[i] = new int[4];
	}
	for(int i = 0; i <= max; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			A[i][j] = 0;
		}
	}

	for(int i = 0; i < im->height; i++)
	{
		for(int j = 0; j < im->width; j++)
		{
			if(blob[i][j] > 0 )
			{
				A[blob[i][j]][0] = A[blob[i][j]][0] + 1;
				A[blob[i][j]][1] = A[blob[i][j]][1] + i;
				A[blob[i][j]][2] = A[blob[i][j]][2] + j;
				A[blob[i][j]][3] = A[blob[i][j]][2] + im->imageData[i * im->widthStep + j];
			}
		}
	}
	cout<<endl<<endl<<"Region Number    "<<"Ceteroid        "<<"Area          "<<"Opticcal Density        "<<endl<<endl;
	int region_counter = 0;
	for(int i = 0; i < max; i++)
	{
		if((A[i][1] != 0 || A[i][2] != 0) && A[i][0] > 15)
		{
			int c1 =A[i][1]/A[i][0];
			int c2 =A[i][2]/A[i][0];
			int c3 =A[i][3]/A[i][0];
			
			cout<< i<<"         "<<"("<<c1<<","<<c2<<")        "<<A[i][0]<<"         "<<c3<<endl; 
			region_counter  = region_counter +1;
		}
	}

	cout<<endl<<"Total number of regions is "<<region_counter<<endl;
	IplImage* colorimg = cvCreateImage(cvSize(im->width,im->height),IPL_DEPTH_8U,3);
	  uchar *cdata = (uchar*)colorimg -> imageData;
	  int cheight = colorimg -> height;  
	  int cwidth  = colorimg -> width;  
	  int cstep   = colorimg -> widthStep; 
	  int cchannels = colorimg->nChannels;

for( int i = 0; i < cheight; i++)
		{
			for( int j = 0; j < cwidth; j++)
			{
					cdata[i * cstep + j * cchannels] = 0;
					cdata[i * cstep + j * cchannels + 1] = 0; 
					cdata[i * cstep + j * cchannels + 2] = 0; 
				}
		}
	
	for(int k = 1; k < max; k++)
	{
		int color = rand()%256;
		int color1 = rand()%256;
		int color2 = rand()%256;
		for( int i = 0; i < cheight; i++)
		{
			for( int j = 0; j < cwidth; j++)
			{
				if(blob[i][j] == k)
				{
					cdata[i * cstep + j * cchannels] = color;
					cdata[i * cstep + j * cchannels + 1] = color1; 
					cdata[i * cstep + j * cchannels + 2] = color2; 
				}
			}
		}
	}
	return colorimg;
}