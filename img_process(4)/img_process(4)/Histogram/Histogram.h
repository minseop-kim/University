#pragma once

#ifndef __Histogram_h__
#define __Histogram_h__


#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <afxwin.h>
#include <atltypes.h>

#include "CvvImage.h"

typedef struct _rgbhisto
{
	int b;
	int g;
	int r;
}RGBHISTO;

int HistoGram(IplImage *des, IplImage *src, CRect imgsize, CRect controlsize);
RGBHISTO* RGBHIsto(IplImage *src, CRect imgsize, CRect controlsize);
int Get_MaxValue_RGB(RGBHISTO bgr[256], RGBHISTO *max, RGBHISTO *min);
int Draw_HistoGram(RGBHISTO *hist, IplImage *des, CRect controlsize, int rgbmode);
int Draw_Histo_pictureControl(CDC *cdc, CStatic *piccon, IplImage *des, IplImage *src, CvvImage *cvvImage, RGBHISTO *histo, CRect historect, int mode);
int Draw_Img_PictureControl(CDC *cdc, CStatic *piccon, IplImage *src, CvvImage *cvvImage, CRect img_rect);
int HistoGram_Gray_Strech(IplImage *src, IplImage *des);
int HistoGram_Gray_Equalization(IplImage *src, IplImage *des);

#endif