#include<iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cmath>
#include "Functions.h"

using namespace std;

//// dilation
IplImage* dilation3(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
  IplImage* tmpimg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;

	for(int i = 1; i < tmpheight; i++)
	{
		for(int j = 1; j < tmpwidth; j++)
		{
			if((data[(i - 1) * step + (j - 1)] == 255) 
				|| (data[(i - 1) * step + j] == 255) 
				|| (data[(i - 1) * step + (j + 1)] == 255) 
				|| (data[i * step + (j - 1)] == 255) 
				|| (data[i * step + j] == 255) 
				|| (data[i * step + (j + 1)] == 255) 
				|| (data[(i + 1) * step + (j - 1)] == 255) 
				|| (data[(i + 1)* step + j] == 255) 
				|| (data[(i + 1) * step + (j + 1)] == 255))
			{
				tmpdata[i * tmpstep + j] = 255;
			}
			else
			{
				tmpdata[i * tmpstep + j] = 0;
			}
		}
	}
	return tmpimg;
}


IplImage* padding3(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep; 

  IplImage* tmpimg = cvCreateImage(cvSize(width + 2,height + 2),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;

	for(int i = 0; i < tmpheight; i++)
	{
		for(int j = 0; j < tmpwidth; j++)
		{
			if((i > 0) && (j > 0) && (i < height + 1) && (j < width + 1))
			{
				tmpdata[i * tmpstep + j] = data [(i-1)* step + (j-1)];
			}
			if((i == 0) && (j > 0) && (j < width + 1))
			{
				tmpdata[j] =  data [j];
			}
			if((i == height + 1) && (j > 0) && (j < width + 1))
			{
				tmpdata[i * tmpstep + j] =  tmpdata [(i-1)* tmpstep + j];
			}
			if((j == 0) && (i < height + 2))
			{
				tmpdata[i * tmpstep + j] =  tmpdata [i * tmpstep + (j+1)];
			}
			if((j == width + 1) && (i < height + 2))
			{
				tmpdata[i * tmpstep + j] =  tmpdata [i * tmpstep + (j-1)];
			}
		}
	}

	return tmpimg;
}

//// erosion
IplImage* erosion3(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
  IplImage* tmpimg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;

	for(int i = 1; i < tmpheight; i++)
	{
		for(int j = 1; j < tmpwidth; j++)
		{
			if((data[(i - 1) * step + (j - 1)] == 255) 
				&& (data[(i - 1) * step + j] == 255) 
				&& (data[(i - 1) * step + (j + 1)] == 255) 
				&& (data[i * step + (j - 1)] == 255) 
				&& (data[i * step + j] == 255) 
				&& (data[i * step + (j + 1)] == 255) 
				&& (data[(i + 1) * step + (j - 1)] == 255) 
				&& (data[(i + 1)* step + j] == 255) 
				&& (data[(i + 1) * step + (j + 1)] == 255))
				{
					tmpdata[i * tmpstep + j] = 255;
				}
				else
				{
					tmpdata[i * tmpstep + j] = 0;
				}
			
		}
	}
return tmpimg;
}



IplImage* unpad3(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
IplImage* tmpimg = cvCreateImage(cvSize(width- 2 ,height- 2),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;
	for(int i = 0; i < tmpheight; i++)
	{
		for(int j = 0; j < tmpwidth; j++)
		{
			tmpdata[i * tmpstep + j] = data[(i+1) * step + (j+1)];
		}
	}
	return tmpimg;
}

IplImage* padding5(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep; 

  IplImage* tmpimg = cvCreateImage(cvSize(width + 2,height + 2),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;
	for(int i = 0; i < tmpheight; i++)
	{
		for(int j = 0; j < tmpwidth; j++)
		{
			if((i > 1) && (j > 1) && (i < tmpheight - 2) && (j < tmpwidth - 2))
			{
				tmpdata[i * tmpstep + j] = data [(i-2)* step + (j-2)];
			}
			if((i < 2) && (j > 1) && (j < tmpwidth - 2))
			{
				tmpdata[i * tmpstep + j] =  data [j];
			}
			if((i > height) && (j > 1) && (j < tmpwidth - 2))
			{
				tmpdata[i * tmpstep + j] =  data [(height-1)* step + j];
			}
			if((j < 2) && (i < tmpheight))
			{
				tmpdata[i * tmpstep + j] =  tmpdata [i * tmpstep + 2 ];
			}
			if((j > tmpwidth - 2) && (i < tmpheight))
			{
				tmpdata[i * tmpstep + j] =  tmpdata [i * tmpstep + (tmpwidth - 2)];
			}
		}
	}
	return tmpimg;
}
IplImage* dilation5(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
  IplImage* tmpimg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;
   for(int i = 0; i < tmpheight; i++)
  {
	  for(int j = 0; j < tmpwidth; j++)
	  {
		  tmpdata[i*tmpstep+j] = 0;
	  }
  }
	for(int i = 2; i < tmpheight - 1; i++)
	{
		for(int j = 2; j < tmpwidth - 1; j++)
		{
			if((data[(i - 2) * step + (j - 2)] == 255) 
			|| (data[(i - 2) * step + (j -1)] == 255) 
			|| (data[(i - 2) * step + j] == 255) 
			|| (data[(i -2) * step + (j + 1)] == 255) 
			|| (data[(i - 2) * step + (j + 2)] == 255) 
			|| (data[(i - 1) * step + (j - 2)] == 255) 
			|| (data[(i - 1) * step + (j -1)] == 255) 
			|| (data[(i - 1) * step + j] == 255) 
			|| (data[(i - 1) * step + (j + 1)] == 255) 
			|| (data[(i - 1) * step + (j + 2)] == 255)
			|| (data[(i) * step + (j - 2)] == 255) 
			|| (data[(i) * step + (j -1)] == 255) 
			|| (data[(i) * step + j] == 255) 
			|| (data[(i) * step + (j + 1)] == 255) 
			|| (data[(i) * step + (j + 2)] == 255)
			|| (data[(i + 1) * step + (j - 2)] == 255) 
			|| (data[(i + 1) * step + (j -1)] == 255) 
			|| (data[(i + 1) * step + j] == 255) 
			|| (data[(i + 1) * step + (j + 1)] == 255) 
			|| (data[(i + 1) * step + (j + 2)] == 255)
			|| (data[(i + 2) * step + (j - 2)] == 255) 
			|| (data[(i + 2) * step + (j -1)] == 255) 
			|| (data[(i + 2) * step + j] == 255)
			|| (data[(i + 2) * step + (j + 1)] == 255) 
			|| (data[(i + 2) * step + (j + 2)] == 255))
			{
				tmpdata[i * tmpstep + j] = 255;
			}
			else
			{
				tmpdata[i * tmpstep + j] = 0;
			}
		}
	}
	return tmpimg;
}



// erosion

IplImage* erosion5(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
  IplImage* tmpimg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;
  for(int i = 0; i < tmpheight; i++)
  {
	  for(int j = 0; j < tmpwidth; j++)
	  {
		  tmpdata[i*tmpstep+j] = 0;
	  }
  }
	for(int i = 2; i < tmpheight - 1; i++)
	{
		for(int j = 2; j < tmpwidth - 1; j++)
		{
			if((data[(i - 2) * step + (j - 2)] == 0) 
			|| (data[(i - 2) * step + (j -1)] == 0) 
			|| (data[(i - 2) * step + j] == 0) 
			|| (data[(i -2) * step + (j + 1)] == 0) 
			|| (data[(i - 2) * step + (j + 2)] == 0) 
			|| (data[(i - 1) * step + (j - 2)] == 0) 
			|| (data[(i - 1) * step + (j -1)] == 0) 
			|| (data[(i - 1) * step + j] == 0) 
			|| (data[(i - 1) * step + (j + 1)] == 0) 
			|| (data[(i - 1) * step + (j + 2)] == 0)
			|| (data[(i) * step + (j - 2)] == 0) 
			|| (data[(i) * step + (j -1)] == 0) 
			|| (data[(i) * step + j] == 0) 
			|| (data[(i) * step + (j + 1)] == 0) 
			|| (data[(i) * step + (j + 2)] == 0)
			|| (data[(i + 1) * step + (j - 2)] == 0) 
			|| (data[(i + 1) * step + (j -1)] == 0) 
			|| (data[(i + 1) * step + j] == 0) 
			|| (data[(i + 1) * step + (j + 1)] == 0) 
			|| (data[(i + 1) * step + (j + 2)] == 0)
			|| (data[(i + 2) * step + (j - 2)] == 0) 
			|| (data[(i + 2) * step + (j -1)] == 0) 
			|| (data[(i + 2) * step + j] == 0) 
			|| (data[(i + 2) * step + (j + 1)] == 0) 
			|| (data[(i + 2) * step + (j + 2)] == 0))
			{
				tmpdata[i * tmpstep + j] = 0;
			}
			else
			{
				tmpdata[i * tmpstep + j] = 255;
			}
		}
	}
	return tmpimg;
}


IplImage* unpad5(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
  IplImage* tmpimg = cvCreateImage(cvSize(width - 4 ,height - 4),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;
	for(int i = 0; i < tmpheight; i++)
	{
		for(int j = 0; j < tmpwidth; j++)
		{
			tmpdata[i * tmpstep + j] = data[(i+2) * step + (j+2)];
		}
	}
	return tmpimg;
}
IplImage* close_open3(IplImage* img)
{
  img = padding3(img);
  IplImage* tmpimg = erosion3(img);
  tmpimg = dilation3(tmpimg);
  tmpimg = dilation3(tmpimg);
  tmpimg = erosion3(tmpimg);

  tmpimg = unpad3(tmpimg);
  return tmpimg;
}



IplImage* open_close3(IplImage* img)
{
  img = padding3(img);
  IplImage* tmpimg = dilation3(img);
 tmpimg = erosion3(tmpimg);
  tmpimg = erosion3(tmpimg);
  tmpimg = dilation3(tmpimg);
  tmpimg = unpad3(tmpimg);
  return tmpimg;
}

IplImage* close_open5(IplImage* img)
{
  img = padding5(img);
  IplImage* tmpimg = erosion5(img);
  tmpimg = dilation5(tmpimg);
  tmpimg = dilation5(tmpimg);
  tmpimg = erosion5(tmpimg);

  tmpimg = unpad5(tmpimg);
  return tmpimg;
}

IplImage* open_close5(IplImage* img)
{
  img = padding5(img);
  IplImage* tmpimg = dilation5(img);
  tmpimg = erosion5(tmpimg);
  tmpimg = erosion5(tmpimg);
  tmpimg = dilation5(tmpimg);
  tmpimg = unpad5(tmpimg);
  return tmpimg;
}


IplImage* countor(IplImage* img)
{
  uchar* data = (uchar*) img -> imageData;
  int width = img->width;
  int height = img->height;
  int step = img->widthStep;
  IplImage* tmpimg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
  uchar* tmpdata = (uchar*) tmpimg -> imageData;
  int tmpwidth = tmpimg->width;
  int tmpheight = tmpimg->height;
  int tmpstep = tmpimg->widthStep;
  for(int i = 1; i < height-1; i++)
  {
	  for(int j = 1; j < width-1; j++)
	  {

		  	if((data[(i - 1) * step + (j - 1)] == 255) 
			&& (data[(i - 1) * step + j] == 255) 
			&& (data[(i - 1) * step + (j + 1)] == 255) 
			&& (data[i * step + (j - 1)] == 255) 
			&& (data[i * step + j] == 255) 
			&& (data[i * step + (j + 1)] == 255) 
			&& (data[(i + 1) * step + (j - 1)] == 255) 
			&& (data[(i + 1)* step + j] == 255) 
			&& (data[(i + 1) * step + (j + 1)] == 255))
			{
				tmpdata[i * tmpstep + j] = 0;
			}
			else
			{
				tmpdata[i * tmpstep + j] = data[i * step + j];
			}
	  }
  }
  return tmpimg;
}