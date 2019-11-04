#include "../stdafx.h"
#include "Histogram.h"
#include <math.h>

#define Banollim(x) (floor( x + 0.5))

int HistoGram(IplImage *des, IplImage *src, CRect imgsize, CRect controlsize) // gray
{
	double hist[256]={0,};
	double chanel = 1;
	double scale = 1;
	int x, y, temp;
	double max=0;
	double min=255;

	if(src == NULL)
		return 0;

	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			hist[(unsigned char)src->imageData[(y * src->widthStep + 3*x)]]++;
		}
	}

	for(temp = 0; temp < 256; temp++)
	{
		if(max < hist[temp])
		{
			max = hist[temp];
		}
		if(min > hist[temp])
		{
			min = hist[temp];
		}
	}


	
	for (temp = 0; temp < 256; temp++)
	{
		hist[temp] = (hist[temp]-min) / (max-min) * (double)controlsize.bottom;
	}

	//////////////////////////////////////////////////////////////////////////
	// canvas에 세로 줄을 그림
	// 줄을 canvas의 밑에서부터 그려야하기 때문에 맨 밑의 포인트(pt2)부터 그림

	for (temp = 0; temp < 256; temp++)
	{
		CvPoint pt1 = cvPoint(temp+1, controlsize.bottom - hist[temp]);
		CvPoint pt2 = cvPoint(temp+1, controlsize.bottom);
		if(hist[temp] != 0)
			cvLine(des, pt1, pt2, CV_RGB(0,0,0), 1, 8, 0);
	}

	return 1;
}
RGBHISTO* RGBHIsto(IplImage *src, CRect imgsize, CRect controlsize)
{
	RGBHISTO *bgr = (RGBHISTO*)calloc(256, sizeof(RGBHISTO));
	int x, y, temp, count=0;
	RGBHISTO max={0,};
	RGBHISTO min={255,255,255};

	if(src == NULL)
		return 0;

	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			bgr[(unsigned char)(src->imageData[y*(src->widthStep) + (x*src->nChannels + 0)])].b++;
			bgr[(unsigned char)(src->imageData[y*(src->widthStep) + (x*src->nChannels + 1)])].g++;
			bgr[(unsigned char)(src->imageData[y*(src->widthStep) + (x*src->nChannels + 2)])].r++;
		}
	}

	Get_MaxValue_RGB(bgr, &max, &min);

	for(count = 0; count < 256; count++)
	{
		bgr[count].b = (int)((double)(bgr[count].b-min.b) * controlsize.bottom / (double)(max.b-min.b));
		bgr[count].g = (int)((double)(bgr[count].g-min.g) * controlsize.bottom / (double)(max.g-min.g));
		bgr[count].r = (int)((double)(bgr[count].r-min.r) * controlsize.bottom / (double)(max.r-min.r));
	}

	return bgr;
}
int Get_MaxValue_RGB(RGBHISTO bgr[256], RGBHISTO *max, RGBHISTO *min)
{
	int index1, index2, temp;
	bool ischanged = false;
	if(bgr == NULL)
		return 0;
	
	for(index1 = 0; index1 < 256; index1++)
	{
		if(max->b < bgr[index1].b)
		{
			max->b = bgr[index1].b;
		}
		if(max->g < bgr[index1].g)
		{
			max->g = bgr[index1].g;
		}
		if(max->r < bgr[index1].r)
		{
			max->r = bgr[index1].r;
		}
		if(min->b > bgr[index1].b)
		{
			ischanged = true;
			min->b = bgr[index1].b;
		}
		if(min->g > bgr[index1].g)
		{
			ischanged = true;
			min->g = bgr[index1].g;
		}
		if(min->r > bgr[index1].r)
		{
			ischanged = true;
			min->r = bgr[index1].r;
		}
	}

	if(min->r == 255 && !ischanged)
		min->r = 0;
	if(min->g == 255 && !ischanged)
		min->g = 0;
	if(min->b == 255 && !ischanged)
		min->b = 0;
	return 1;
}
int Draw_HistoGram(RGBHISTO *hist, IplImage *des, CRect controlsize,int rgbmode)
{
	if(hist == NULL)
		return 0;

	int temp;
	CvPoint pt1, pt2;

	for (temp = 0; temp < 256; temp++)
	{
		switch(rgbmode)
		{
		case 0:	// 파란색
			pt1 = cvPoint(temp, controlsize.bottom - hist[temp].b);
			pt2 = cvPoint(temp, controlsize.bottom);
			if(controlsize.bottom - hist[temp].b == controlsize.bottom)
				break;
			cvLine(des, pt1, pt2, CV_RGB(0,0,255), 1, 8, 0);
			break;
		case 1: // 초록색
			pt1 = cvPoint(temp, controlsize.bottom - hist[temp].g);
			pt2 = cvPoint(temp, controlsize.bottom);
			if(controlsize.bottom - hist[temp].g == controlsize.bottom)
				break;
			cvLine(des, pt1, pt2, CV_RGB(0,255,0), 1, 8, 0);
			break;
		case 2: // 빨간색
			pt1 = cvPoint(temp, controlsize.bottom - hist[temp].r);
			pt2 = cvPoint(temp, controlsize.bottom);
			if(controlsize.bottom - hist[temp].r == controlsize.bottom)
				break;
			cvLine(des, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0);
			break;
		}
	}
	
	return 1;
}
int Draw_Histo_pictureControl(CDC *cdc, CStatic *piccon, IplImage *des, IplImage *src, CvvImage *cvvImage, RGBHISTO *histo, CRect historect, int mode)
{
	if(piccon == NULL)
		return 0;

	cdc = piccon->GetDC();
	cvSet(des, CV_RGB(255, 255, 255), 0);
	piccon->GetClientRect(&historect);
	
	Draw_HistoGram(histo, des, historect, mode);
	cvvImage->CopyOf(des);
	cvvImage->DrawToHDC(cdc->GetSafeHdc(), historect);
	piccon->GetDC()->SetStretchBltMode(COLORONCOLOR | HALFTONE);
	piccon->GetDC()->StretchBlt(0,0,historect.right,historect.bottom, cdc, 0,0, 256, 50, SRCCOPY);

	return 1;
}
int Draw_Img_PictureControl(CDC *cdc, CStatic *piccon, IplImage *src, CvvImage *cvvImage, CRect img_rect)
{
	if(src == NULL)
		return 0;

	cdc = piccon->GetDC();
	piccon->GetClientRect(&img_rect);

	cvvImage->CopyOf(src);
	cvvImage->DrawToHDC(cdc->GetSafeHdc(), img_rect);

	return 1;
}
int HistoGram_Gray_Strech(IplImage *src, IplImage *des)
{
	if(src == NULL)
		return 0;

	double S_max = 255, S_min = 0, I_max=-1, I_min=-1;
	int hist[256]={0,};
	double src_color_value, des_color_value;
	int x, y;

	// 소스의 히스토그램 받아오기
	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			hist[(unsigned char)src->imageData[(y * src->widthStep + 3*x)]]++;
		}
	}

	// 원본 히스토그램의 값이 있는 최소인덱스 최대 인덱스를 구한다.
	for(x = 0, y=255; x< 256; x++, y--)
	{
		if((hist[x] != 0) && (I_min == -1))
			I_min = x;
		if((hist[y] != 0) && (I_max == -1))
			I_max = y;
		if((I_min != -1) && (I_max != -1))
			break;
	}
	
	// 원본데이터를 해당 식에 맞게 바꾼다.
	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			src_color_value = (unsigned char)src->imageData[(y * src->widthStep + 3*x)];
			if(src_color_value != 0)
			{
				des_color_value = abs((S_max-S_min) * abs((src_color_value - I_min) / (I_max-I_min))) + S_min;
				des->imageData[(y * des->widthStep + x)] = (unsigned char)des_color_value;
			}
		}
	}
	return 1;
}
int HistoGram_Gray_Equalization(IplImage *src, IplImage *des)
{
	if(src == NULL)
		return 0;

	double S_max = 255, S_min = 0, I_max=-1, I_min=-1;
	double hist[256][2];
	int src_color_value;
	int x, y;

	memset(hist, 0, sizeof(double)*512);
	// 소스의 히스토그램 받아오기
	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			hist[(unsigned char)src->imageData[(y * src->widthStep + 3*x)]][0]++;
		}
	}

	hist[0][1] = hist[0][0];

	// 누적 히스토그램 구하기
	for(y=1; y<256; y++)
	{
		hist[y][1] = hist[y-1][1] + hist[y][0];
	}
	
	// 새로운 그레이 레벨값 구하기
	for(y=0; y<256; y++)
	{
		hist[y][1] = Banollim((hist[y][1] / hist[255][1] * 255));
	}

	// 원본데이터를 새로운 그레이 레벨값으로 구한다.
	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			src_color_value = (unsigned char)src->imageData[(y * src->widthStep + 3*x)];
			if(src_color_value != 0)
				des->imageData[(y * des->widthStep + x)] = hist[src_color_value][1];
		}
	}

	return 1;
}
