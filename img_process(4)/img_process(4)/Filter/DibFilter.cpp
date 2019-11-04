#include "../StdAfx.h"
#include <Windows.h>
#include "DibFilter.h"
#include "CvvImage.h"

void DibFilterMean(IplImage *src, IplImage *des, int type)
{
	int index_x, index_y, sum[3] = {0,}, mode;
	IplImage *non_maximum_supptression_before = cvCreateImage(cvGetSize(des), des->depth, des->nChannels);
	IplImage *non_maximum_supptression_after = cvCreateImage(cvGetSize(des), des->depth, des->nChannels);
	for (index_y = 0; index_y < src->height; index_y++)
	{
		for (index_x = 0; index_x < src->width; index_x++)
		{
			if(index_x==0 && index_y==0)
			{
				mode = 0;		// 왼쪽 위에
			}
			else if(index_x==src->width-1 && index_y==0)
			{
				mode = 1;		// 오른쪽 위에
			}
			else if(index_x==0 && index_y==src->height-1)
			{
				mode = 2;		// 왼쪽 아래
			}
			else if(index_x==src->width-1 && index_y==src->height-1)
			{
				mode = 3;		// 오른쪽 아래
			}
			else if((index_x!=0 && index_x!=src->width-1) && index_y==0)
			{
				mode = 4;		// 위쪽
			}
			else if(index_x==src->width-1 && (index_y!=0 && index_y!=src->height-1))
			{
				mode = 5;		// 오른쪽
			}
			else if((index_x!=0 && index_x!=src->width-1) && index_y==src->height-1)
			{
				mode = 6;		// 아래쪽
			}
			else if(index_x==0 && (index_y!=0 && index_y!=src->height-1))
			{
				mode = 7;		// 왼쪽
			}
			else
			{
				mode = 8;
			}
			switch(type)
			{
			case 0:
				sum_average(src, sum, index_x, index_y, mode);
				des->imageData[index_y*(des->widthStep) + (index_x*des->nChannels + 1)] = sum[1];
				des->imageData[index_y*(des->widthStep) + (index_x*des->nChannels + 2)] = sum[2];
				des->imageData[index_y*(des->widthStep) + (index_x*des->nChannels + 0)] = sum[0];
				break;
			case 1:
				calc_gaussian(src, sum, index_x, index_y, mode);
				non_maximum_supptression_before->imageData[index_y*(non_maximum_supptression_before->widthStep) + (index_x*non_maximum_supptression_before->nChannels + 0)] = sum[0];
				break;
			}
			memset(sum, 0, sizeof(int)*3);
		}
	}


	if(type == 1)
	{
		Prewitt_filter(non_maximum_supptression_before, non_maximum_supptression_after);
		Strong_Edge(non_maximum_supptression_after, des);
		low_high_value(des);
	}

}
void filter(IplImage *src, IplImage *des)
{
	int index_x, index_y, sum[3] = {0,}, mode;
	for (index_y = 0; index_y < src->height; index_y++)
	{
		for (index_x = 0; index_x < src->width; index_x++)
		{
			if(index_x==0 && index_y==0)
			{
				mode = 0;		// 왼쪽 위에
			}
			else if(index_x==src->width-1 && index_y==0)
			{
				mode = 1;		// 오른쪽 위에
			}
			else if(index_x==0 && index_y==src->height-1)
			{
				mode = 2;		// 왼쪽 아래
			}
			else if(index_x==src->width-1 && index_y==src->height-1)
			{
				mode = 3;		// 오른쪽 아래
			}
			else if((index_x!=0 && index_x!=src->width-1) && index_y==0)
			{
				mode = 4;		// 위쪽
			}
			else if(index_x==src->width-1 && (index_y!=0 && index_y!=src->height-1))
			{
				mode = 5;		// 오른쪽
			}
			else if((index_x!=0 && index_x!=src->width-1) && index_y==src->height-1)
			{
				mode = 6;		// 아래쪽
			}
			else if(index_x==0 && (index_y!=0 && index_y!=src->height-1))
			{
				mode = 7;		// 왼쪽
			}
			else
			{
				mode = 8;
			}
			memset(sum, 0, sizeof(int)*3);
			calc_mode(src, sum, index_x, index_y, mode);
			des->imageData[index_y*(des->widthStep) + (index_x*des->nChannels + 0)] = sum[0];
			des->imageData[index_y*(des->widthStep) + (index_x*des->nChannels + 1)] = sum[1];
			des->imageData[index_y*(des->widthStep) + (index_x*des->nChannels + 2)] = sum[2];
		}
	}

}
void sum_average(IplImage *img, int *sum, int index_x, int index_y,int mode)
{
	int x, y, color_index=0;
	switch(mode)
	{
	case 0:			// 왼쪽 위 모서리
		for(y=0; y<2; y++)
			for(x=0; x<2; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 1:			// 오른쪽 위 모서리
		for(y=0; y<2; y++)
			for(x=-1; x<1; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 2:			// 왼쪽 아래 모서리
		for(y=-1; y<1; y++)
			for(x=0; x<2; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 3:			// 오른쪽 아래 모서리
		for(y=-1; y<1; y++)
			for(x=-1; x<1; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 4:			// 위쪽
		for(y=0; y<2; y++)
			for(x=-1; x<2; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 5:			// 오른쪽
		for(y=-1; y<2; y++)
			for(x=-1; x<1; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 6:			// 아래쪽
		for(y=-1; y<1; y++)
			for(x=-1; x<2; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 7:			// 왼쪽
		for(y=-1; y<2; y++)
			for(x=0; x<2; x++)
			{
				sum[0] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 8:
		for(y=-1; y<2; y++)
			for(x=-1; x<2; x++)
				for(color_index = 0; color_index<3; color_index++)
					sum[color_index] += (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + color_index)]);
		break;
	}
	switch(mode)
	{
	case 0:case 1:case 2:case 3:
		sum[0] /= FILTER_POINT;
		sum[1] /= FILTER_POINT;
		sum[2] /= FILTER_POINT;
		break;
	case 4:case 5:case 6:case 7:
		sum[0] /= FILTER_EDGE;
		sum[1] /= FILTER_EDGE;
		sum[2] /= FILTER_EDGE;
		break;
	case 8:
		sum[0] /= FILTER_SCALE;
		sum[1] /= FILTER_SCALE;
		sum[2] /= FILTER_SCALE;
		break;
	}
}
void calc_mode(IplImage *img, int *sum, int index_x, int index_y, int mode)
{
	int x, y, color_index=0, i_x, i_y;
	int mask[3][3][3];
	memset(mask, -1, sizeof(int)*27);

	switch(mode)
	{
	case 0:case 1:case 2:case 3:
		mask[1][1][0] = 4;
		mask[1][1][1] = 4;
		mask[1][1][2] = 4;
		break;
	case 4:case 5:case 6:case 7:
		mask[1][1][0] = 6;
		mask[1][1][1] = 6;
		mask[1][1][2] = 6;
		break;
	case 8:
		mask[1][1][0] = 9;
		mask[1][1][1] = 9;
		mask[1][1][2] = 9;
	}

	switch(mode)
	{
	case 0:			// 왼쪽 위 모서리
		for(y=0, i_y=1; y<2; y++,i_y++)
			for(x=0, i_x=1; x<2; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 1:			// 오른쪽 위 모서리
		for(y=0, i_y=1; y<2; y++,i_y++)
			for(x=-1, i_x=0; x<1; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 2:			// 왼쪽 아래 모서리
		for(y=-1, i_y=0; y<1; y++,i_y++)
			for(x=0, i_x=1; x<2; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 3:			// 오른쪽 아래 모서리
		for(y=-1, i_y=0; y<1; y++,i_y++)
			for(x=-1, i_x=0; x<1; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 4:			// 위쪽
		for(y=0, i_y=1; y<2; y++, i_y++)
			for(x=-1, i_x=0; x<2; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 5:			// 오른쪽
		for(y=-1, i_y=0; y<2; y++,i_y++)
			for(x=-1, i_x=0; x<1; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 6:			// 아래쪽
		for(y=-1, i_y=0; y<1; y++,i_y++)
			for(x=-1, i_x=0; x<2; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 7:			// 왼쪽
		for(y=-1, i_y=0; y<2; y++,i_y++)
			for(x=0, i_x=1; x<2; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
		
	case 8:
		for(y=-1, i_y=0; y<2; y++,i_y++)
			for(x=-1, i_x=0; x<2; x++, i_x++)
			{
				sum[0] += mask[i_y][i_x][0] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
				sum[1] += mask[i_y][i_x][1] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 1)]);
				sum[2] += mask[i_y][i_x][2] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 2)]);
			}
		break;
	}

	for(int temp=0; temp<3; temp++)
	{
		if(sum[temp] > 255)
			sum[temp] = 255;
		if(sum[temp] < 0)
			sum[temp] = 0;
	}
}
void calc_gaussian(IplImage *img, int *sum, int index_x, int index_y, int mode)
{
	int x, y, color_index=0, i_x, i_y;
	int mask[3][3]={{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

	switch(mode)
	{
	case 0:			// 왼쪽 위 모서리
		for(y=0, i_y=1; y<2; y++,i_y++)
		{
			for(x=0, i_x=1; x<2; x++, i_x++)
			{
				*sum += mask[i_y][i_x] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 1:			// 오른쪽 위 모서리
		for(y=0, i_y=1; y<2; y++,i_y++)
		{
			for(x=-1, i_x=0; x<1; x++, i_x++)
			{
				*sum += mask[i_y][i_x]* (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 2:			// 왼쪽 아래 모서리
		for(y=-1, i_y=0; y<1; y++,i_y++)
		{
			for(x=0, i_x=1; x<2; x++, i_x++)
			{
				*sum += mask[i_y][i_x] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 3:			// 오른쪽 아래 모서리
		for(y=-1, i_y=0; y<1; y++,i_y++)
		{
			for(x=-1, i_x=0; x<1; x++, i_x++)
			{
				*sum += mask[i_y][i_x] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 4:			// 위쪽
		for(y=0, i_y=1; y<2; y++, i_y++)
		{
			for(x=-1, i_x=0; x<2; x++, i_x++)
			{
				*sum += mask[i_y][i_x]* (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 5:			// 오른쪽
		for(y=-1, i_y=0; y<2; y++,i_y++)
		{
			for(x=-1, i_x=0; x<1; x++, i_x++)
			{
				*sum += mask[i_y][i_x] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 6:			// 아래쪽
		for(y=-1, i_y=0; y<1; y++,i_y++)
		{
			for(x=-1, i_x=0; x<2; x++, i_x++)
			{
				*sum += mask[i_y][i_x] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 7:			// 왼쪽
		for(y=-1, i_y=0; y<2; y++,i_y++)
		{
			for(x=0, i_x=1; x<2; x++, i_x++)
			{
				*sum += mask[i_y][i_x] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
		
	case 8:
		for(y=-1, i_y=0; y<2; y++,i_y++)
		{
			for(x=-1, i_x=0; x<2; x++, i_x++)
			{
				*sum += mask[i_y][i_x] * (unsigned char)(img->imageData[(index_y + y)*(img->widthStep) + ((index_x + x)*img->nChannels + 0)]);
			}
		}
		break;
	}

	switch(mode)
	{
	case 0: case 1: case 2: case 3:
		*sum /= 9;
		break;
	case 4: case 5: case 6: case 7:
		*sum /= 12;
		break;
	default:
		*sum /= 16;
		break;
	}

	if(*sum > 255)
		*sum = 255;
	if(*sum < 0)
		*sum = 0;
}
void Prewitt_filter(IplImage *src, IplImage *des)
{
	int index_x, index_y, sum = 0, mode;
	int sum1, sum2;
	double add;

	for (index_y = 1; index_y < src->height-1; index_y++)
	{
		for (index_x = 1; index_x < src->width-1; index_x++)
		{
			sum1 = -(unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + ((index_x-1)*src->nChannels)]) \
				-(unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + (index_x*src->nChannels)]) \
				-(unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + ((index_x+1)*src->nChannels)]) \
				+(unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + ((index_x-1)*src->nChannels)]) \
				+(unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + (index_x*src->nChannels)]) \
				+(unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + ((index_x+1)*src->nChannels)]);

			sum2 = -(unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + ((index_x-1)*src->nChannels)]) \
				-(unsigned char)(src->imageData[(index_y)*(src->widthStep) + ((index_x-1)*src->nChannels)]) \
				-(unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + ((index_x-1)*src->nChannels)]) \
				+(unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + ((index_x+1)*src->nChannels)]) \
				+(unsigned char)(src->imageData[(index_y)*(src->widthStep) + ((index_x+1)*src->nChannels)]) \
				+(unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + ((index_x+1)*src->nChannels)]);

			add = sqrt((double)(sum1*sum1 + sum2*sum2));

			if(add < 0)
				add = 0;
			if(add > 255)
				add = 255;

			des->imageData[index_y*(des->widthStep) + (index_x*des->nChannels)] = add;
		}
	}
}
void Strong_Edge(IplImage *src, IplImage *des)
{
	int front, rear, now, up, down, y_equal_x_u, y_equal_x_d, y_equal_mx_u, y_equal_mx_d;
	int index_x, index_y;

	for (index_y = 1; index_y < src->height - 1; index_y++)
	{
		for (index_x = 1; index_x < src->width - 1; index_x++)
		{
			//is_edge = false;

			front = (unsigned char)(src->imageData[(index_y+0)*(src->widthStep) + ((index_x-1)*src->nChannels)]);
			rear = (unsigned char)(src->imageData[(index_y+0)*(src->widthStep) + ((index_x+1)*src->nChannels)]);
			up = (unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + ((index_x+0)*src->nChannels)]);
			down = (unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + ((index_x+0)*src->nChannels)]);
			now = (unsigned char)(src->imageData[(index_y+0)*(src->widthStep) + ((index_x+0)*src->nChannels)]);
			y_equal_x_u = (unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + ((index_x+1)*src->nChannels)]);
			y_equal_x_d = (unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + ((index_x-1)*src->nChannels)]);
			y_equal_mx_u = (unsigned char)(src->imageData[(index_y-1)*(src->widthStep) + ((index_x-1)*src->nChannels)]);
			y_equal_mx_d = (unsigned char)(src->imageData[(index_y+1)*(src->widthStep) + ((index_x+1)*src->nChannels)]);


			// 강한 에지가 아닌것 앞뒤
			if((front > now) || (rear >= now))
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 0;
			else
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = (unsigned char)(src->imageData[(index_y+0)*(src->widthStep) + ((index_x+0)*src->nChannels)]);

			// 강한 에지가 아닌것 위아래
			if((up > now) || (down >= now))
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 0;
			else
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = (unsigned char)(src->imageData[(index_y+0)*(src->widthStep) + ((index_x+0)*src->nChannels)]);

			// 강한 에지가 아닌것 y=x 대각선
			if((y_equal_x_u > now) || (y_equal_x_d >= now))
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 0;
			else
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = (unsigned char)(src->imageData[(index_y+0)*(src->widthStep) + ((index_x+0)*src->nChannels)]);

			// 강한 에지가 아닌것 y=-x 대각선
			if((y_equal_mx_u > now) || (y_equal_mx_d >= now))
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 0;
			else
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = (unsigned char)(src->imageData[(index_y+0)*(src->widthStep) + ((index_x+0)*src->nChannels)]);



			
			// 강한 에지 인것
			if(front == 255 && rear == 255)
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 255;

			// 강한 에지 인것
			if(up == 255 && down == 255)
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 255;
			
			// 강한 에지 인것
			if(y_equal_x_u == 255 && y_equal_x_d == 255)
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 255;

			// 강한 에지 인것
			if(y_equal_mx_u == 255 && y_equal_mx_d == 255)
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 255;

		}
	}
}
void low_high_value(IplImage *des)
{
	int index_x, index_y;
	int value, low=70, high=250;
	for (index_y = 1; index_y < des->height - 1; index_y++)
	{
		for (index_x = 1; index_x < des->width - 1; index_x++)
		{
			value = (unsigned char)(des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)]);
			
			if((value > low) && (value < high))
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 255.0;
			else 
				des->imageData[(index_y+0)*(des->widthStep) + ((index_x+0)*des->nChannels)] = 0.0;
		}
	}
}
void laplacian_of_gausian(IplImage *src, IplImage *des)
{
	int x, y;
	double sum;

	cvCopy(src, des);

	/* 적용마스크 3 * 3
	0.4038	0.8021	0.4038
	0.8021	-4.8233	0.8021
	0.4038	0.8021	0.4038
	*/
	for(y=1; y<src->height-1; y++)
	{
		for(x=1; x<src->width-1; x++)
		{
			sum = \
				(double)(0.4038 * (double)src->imageData[(y-1)*(des->widthStep) + ((x-1)*des->nChannels)])\
				+ (double)(0.8021 * (double)src->imageData[(y-1)*(des->widthStep) + ((x+0)*des->nChannels)])\
				+ (double)(0.4038 * (double)src->imageData[(y-1)*(des->widthStep) + ((x+1)*des->nChannels)])\

				+ (double)(0.8021 * (double)src->imageData[(y+0)*(des->widthStep) + ((x-1)*des->nChannels)])\
				- (double)(4.8233 * (double)src->imageData[(y+0)*(des->widthStep) + ((x+0)*des->nChannels)])\
				+ (double)(0.8021 * (double)src->imageData[(y+0)*(des->widthStep) + ((x+1)*des->nChannels)])\

				+ (double)(0.4038 * (double)src->imageData[(y+1)*(des->widthStep) + ((x-1)*des->nChannels)])\
				+ (double)(0.8021 * (double)src->imageData[(y+1)*(des->widthStep) + ((x+0)*des->nChannels)])\
				+ (double)(0.4038 * (double)src->imageData[(y+1)*(des->widthStep) + ((x+1)*des->nChannels)]);

			if(sum<0.0)
				sum = 0.0;
			else if(sum>255.0)
				sum = 255.0;

			if(sum>20)
				sum = 255.0;
			else
				sum = 0.0;

			des->imageData[(y+0)*des->widthStep + ((x+0)*des->nChannels)] = (unsigned char)sum;
		}
	}
}