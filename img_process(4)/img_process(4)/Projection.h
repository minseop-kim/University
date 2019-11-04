#pragma once

#ifndef __Projection_h__
#define __Projection_h__

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <afxwin.h>
#include <atltypes.h>


int Calc_Projection(IplImage *src,int *count, int mode);
int Draw_Projection_Image(IplImage *des, int *count, int mode);
int Process_Projection(IplImage *src, IplImage *des, int mode);

#endif