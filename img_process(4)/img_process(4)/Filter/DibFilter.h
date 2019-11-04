#pragma once

#ifndef __DibFilter_h__
#define __DibFilter_h__


#include <opencv\cv.h>
#include <opencv\highgui.h>

#ifndef FILTER_SCALE
#define FILTER_SCALE 9		// 3*3 Filter
#define FILTER_EDGE  6		// 3*3 Filter
#define FILTER_POINT 4		// 3*3 Filter
#endif

void DibFilterMean(IplImage *src, IplImage *des, int type);
void sum_average(IplImage *img, int *sum, int index_x, int index_y, int mode);
void filter(IplImage *src, IplImage *des);
void calc_mode(IplImage *img, int *sum, int index_x, int index_y, int mode);
void Prewitt_filter(IplImage *src, IplImage *des);
void calc_gaussian(IplImage *img, int *sum, int index_x, int index_y, int mode);
void Strong_Edge(IplImage *src, IplImage *des);
void low_high_value(IplImage *des);
void laplacian_of_gausian(IplImage *src, IplImage *des);

#endif