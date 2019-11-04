#include "../stdafx.h"
#include "Projection.h"
#include "Histogram.h"

int Calc_Projection(IplImage *src, int *count, int mode)
{
	int x, y, temp;
	int pixel_value;

	if(src == NULL)
		return 0;

	switch(mode)
	{
	case 0:
		for (y = 0; y < src->height; y++)
		{
			for (x = 0; x < src->width; x++)
			{
				pixel_value = (unsigned char)(src->imageData[(y * src->widthStep + x)]);
				if(pixel_value == 255)
					count[y]++;
			}
		}
		break;
	case 1:
		for (x = 0; x < src->width; x++)
		{
			for (y = 0; y < src->height; y++)
			{
				pixel_value = (unsigned char)(src->imageData[(y * src->widthStep + x)]);
				if(pixel_value == 255)
					count[x]++;
			}
		}
		break;
	}
	return 1;
}
int Draw_Projection_Image(IplImage *des, int *count, int mode)
{
	int temp;

	switch(mode)
	{
	case 0:
		for (temp = 0; temp < des->height; temp++)
		{
			CvPoint pt1 = cvPoint(0, temp);
			CvPoint pt2 = cvPoint(count[temp]-1, temp);
			if(count[temp] != 0)
				cvLine(des, pt1, pt2, CV_RGB(0,0,0), 1, 8, 0);
		}
		break;
	case 1:
		for (temp = 0; temp < des->width; temp++)
		{
			CvPoint pt1 = cvPoint(temp, des->height);
			CvPoint pt2 = cvPoint(temp, des->height-count[temp]);
			if(count[temp] != 0)
				cvLine(des, pt1, pt2, CV_RGB(0,0,0), 1, 8, 0);
		}
		break;
	}

	return 1;
}
int Process_Projection(IplImage *src, IplImage *des, int mode)
{
	if(src == NULL || des==NULL)
		return 0;

	int *count;

	switch(mode)
	{
	case 0:
		count = (int*)calloc(src->height, sizeof(int));
		break;
	case 1:
		count = (int*)calloc(src->width, sizeof(int));
		break;
	}

	Calc_Projection(src, count, mode);
	cvSet(des, CV_RGB(255, 255, 255), 0);
	Draw_Projection_Image(des, count, mode);


	free(count);

	return 1;
}